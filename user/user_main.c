/*
	The hello world demo
*/


#include "mem.h"
#include "ESP8266wifi.h"
#define request_http "GET https://api.thingspeak.com/update?api_key=9FZFDZ3FMG7BWWW3&field1=100000&field2=1000000 HTTP/1.0\r\n\r\n"
os_timer_t test_timer;



/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABBBCDDD
 *                A : rf cal
 *                B : at parameters
 *                C : rf init data
 *                D : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR user_rf_pre_init(void)
{
}


LOCAL void ICACHE_FLASH_ATTR
user_tcp_connect_cb(void *arg)
{
    struct espconn *pespconn = arg;
    //os_printf("Connected to server...\r\n");
    //espconn_regist_recvcb(pespconn, user_tcp_recv_cb);
    //espconn_regist_sentcb(pespconn, user_tcp_sent_cb);
    //espconn_regist_disconcb(pespconn, user_tcp_discon_cb);
    espconn_send(pespconn,request_http,data_legnth(request_http));
}

LOCAL void ICACHE_FLASH_ATTR
user_tcp_recon_cb(void *arg, sint8 err)
{
   //error occured , tcp connection broke. user can try to reconnect here.

    os_printf("Reconnect callback called, error code: %d !!! \r\n",err);
}


void timer_cb(void)
{
	const char remote_ip[4]={34,228,105,156};
	ESP_tcp_client_init(remote_ip,80,user_tcp_connect_cb,user_tcp_recon_cb);
	}


void ICACHE_FLASH_ATTR user_init(void)
{
	ESP_wifi_init("Etisalat 4G iModem-6569","22999836",STA_AP_MODE);
	   os_timer_disarm(&test_timer);
	   os_timer_setfn(&test_timer, (os_timer_func_t *)timer_cb, NULL);
	   os_timer_arm(&test_timer, 100, 0);

}

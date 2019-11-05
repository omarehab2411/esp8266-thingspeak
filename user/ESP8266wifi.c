/*
 * ESP8266wifi.c
 *
 *  Created on: Nov 2, 2019
 *      Author: omar ehab
 */

#include "ESP8266wifi.h"

//***********************************
//description:this function is intended to set the wifi connection by taking
//ssid and password of desired network then it will create a station config structure
//this structure contain members that will store ssid and password of desired netwrok
//we will then copy user entered data to that structure and call espressif api to set these configuration
//
// paramteres: pointer to ssid and password user will enter
//              variable to store the mode user want esp to work at
//return: boolean value 1 means esp connected and got ip address
//***********************************

void ESP_wifi_init(char* ssid,char *password,unsigned char mode)
{   static struct station_config wificlient;
    //static struct ip_info ip_address;
	wifi_set_opmode(mode);
	os_memcpy(&wificlient.ssid,ssid,data_legnth(ssid));
	os_memcpy(&wificlient.password,password,data_legnth(password));
	  wifi_station_set_config (&wificlient);
}


//********************************
//description:this function will set the esp to be a tcp server
//to do this we first create a espconn structure variable that will contain
//information about our connection like type tcp/udp local port esp will listen to
// we will also create a protocol define data structure variable called tcp that
//will hold the configuration of the tcp connection.
// paramteres:number of local port esp will listen as a server to usually chosen 80
//            function pointer that will point to the call back function that will be
//            executed when a tcp connection successfully established
//return: none
//*********************************
void  ESP_TCP_SERVER_INIT(unsigned int local_port,espconn_connect_callback ptr2cb)
{ static struct espconn conn1;        //struct for choosing connection type tcp/udp lazm static 8ir kda dol hi2do en dangling pointer
  static esp_tcp tcp1;                //struct that configure tcp settings  lazm static 8ir kda hib2a dangling pointer
  conn1.type=ESPCONN_TCP;
  conn1.state=ESPCONN_NONE;
  tcp1.local_port=local_port;
  conn1.proto.tcp=&tcp1;        //pointer to the tcp structure we created
  espconn_regist_connectcb(&conn1,ptr2cb);
  espconn_accept(&conn1);
}



//********************************************************************
//description:this function make esp as a tcp client by connecting to a remote server
//before calling this function user should create a global os_timer_t structure and create an array that
//contains the remote server IP address this function should only be called in a timer call back
//as calling it in user_init will take more than 1 second and watch dog will reset ESP
//
//parameters:
//1-array that contain the 4 bytes of the remote ip address
//2-the remote port of the server
//3-pointer to the connect call back
//4-pointer to the reconnect call back
//return :none
//********************************************************************

void ESP_tcp_client_init(const char remote_ip[4],unsigned int remote_port,espconn_connect_callback ccb,espconn_reconnect_callback rccb)
{
	struct ip_info ipconfig;
	static struct espconn tcp_client;
	static struct _esp_tcp tcp_client_instance;
	extern os_timer_t test_timer;    //should be creatd by user in the application code
    extern void timer_cb(void);     //timer call back function should be implemented in application code
	//disarm timer first
	 os_timer_disarm(&test_timer);

	if(wifi_station_get_connect_status()==STATION_GOT_IP && ipconfig.ip.addr!=0 )
	{
		tcp_client.proto.tcp=&tcp_client_instance;
		tcp_client.type=ESPCONN_TCP;
		tcp_client.state=ESPCONN_NONE;

		os_memcpy(tcp_client.proto.tcp->remote_ip,remote_ip,4);
		tcp_client.proto.tcp->remote_port=remote_port;
		tcp_client.proto.tcp->local_port=espconn_port();

		 espconn_regist_connectcb(&tcp_client,ccb); // register connect callback
		 espconn_regist_reconcb(&tcp_client,rccb); // register reconnect callback as error handler
		 espconn_connect(&tcp_client);
	}

	else{
		 os_timer_setfn(&test_timer, (os_timer_func_t *)timer_cb, NULL);
		 os_timer_arm(&test_timer, 100, 0);
	}
}



//*****************************
//description:this function will calculate legnth of a given string and return
//the number to the use
//Paramter:pointer to user entered data that he want to measure it's legnth
//return:legnth of data measured
//******************************
unsigned int  data_legnth(char *data)
{unsigned int count=0;
int i=0;
while(data[i]!='\0')
{
i++;
count++;
}
return count;
}




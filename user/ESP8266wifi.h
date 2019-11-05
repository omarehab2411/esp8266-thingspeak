/*
 * ESP8266wifi.h
 *
 *  Created on: Nov 2, 2019
 *      Author: omar ehab
 */

#ifndef USER_ESP8266WIFI_H_
#define USER_ESP8266WIFI_H_

#include <ets_sys.h>
#include <osapi.h>
#include "user_interface.h"
#include <os_type.h>
#include <gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include "espconn.h"



//*********************************
//Macros used in wifi init function
#define STA_MODE    (0x01)   //station mode macro
#define AP_MODE     (0x02)   //soft access point mode
#define STA_AP_MODE (0x03)   //station+soft ap mode
//*********************************

/*****************************************************
//  API PROTOTYPES
******************************************************/

extern void   ESP_wifi_init(char* ssid,char *password,unsigned char mode);
extern void   ESP_TCP_SERVER_INIT(unsigned int local_port,espconn_connect_callback ptr2cb);
extern void ESP_tcp_client_init(const char remote_ip[4],unsigned int remote_port,espconn_connect_callback ccb,espconn_reconnect_callback rccb);
extern unsigned int  data_legnth(char *data);
extern void tcp_connection_cb(void *arg);
//********************************************************


#endif /* USER_ESP8266WIFI_H_ */

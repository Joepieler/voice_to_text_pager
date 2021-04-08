/*
 * ESP8266Interface.hpp
 *
 *  Created on: Mar 23, 2021
 *      Author: BCAMPAGN
 */

#ifndef ESP8266INTERFACE_HPP_
#define ESP8266INTERFACE_HPP_

#define BUFFERSIZE 100
#define CONNECTION_ATTEMPTS 100

#ifdef __cplusplus
extern "C" {

#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <string.h>

class ESP8266Interface {
private:
	char buffer_[BUFFERSIZE];
	UART_HandleTypeDef *ESP8266_;
	char IPaddress_[15];
	int IsOK(uint8_t length);
	int IsOK(uint8_t length, uint32_t TimeoutTime);
public:
	ESP8266Interface(UART_HandleTypeDef *ESP8266);
	virtual ~ESP8266Interface();
	int StartUp(int mode);
	int Reset();
	int DHCP(bool enable, int mode);
	int Connect(const char *wifiname, const char *password);
	int Disconnect();
	int ConnectSocket(const char *type, const char *ipaddress, uint16_t poort);
	int8_t GetRSSI();
	int IsConnected();
	const char * GetIP();
	int8_t scan();
	int Send(int id, const void *datat, uint32_t amount);
	int32_t Receive(int id, void *data, uint32_t amount);
	bool OpenPort(const char *type, uint32_t port);

};

}
#endif
#endif /* ESP8266INTERFACE_HPP_ */

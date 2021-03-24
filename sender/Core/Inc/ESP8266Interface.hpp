/*
 * ESP8266Interface.hpp
 *
 *  Created on: Mar 23, 2021
 *      Author: BCAMPAGN
 */

#ifndef ESP8266INTERFACE_HPP_
#define ESP8266INTERFACE_HPP_

#ifdef __cplusplus
extern "C" {

#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <string.h>

class ESP8266Interface {
private:
	UART_HandleTypeDef *ESP8266_;
	bool IsOK();
public:
	ESP8266Interface(UART_HandleTypeDef *ESP8266);
	virtual ~ESP8266Interface();
	bool StartUp(int mode);
	bool Reset();
	bool DHCP(bool enable, int mode);
	bool Connect(const char *wifiname, const char *password);
	bool Disconnect();
	bool ConnectSocket(const char *type, const char *ipaddress, uint16_t poort);
	int8_t GetRSSI();
	bool IsConnected();
	int8_t scan();
	bool Send(int id, const void *datat, uint32_t amount);
	int32_t Receive(int id, void *data, uint32_t amount);

};

}
#endif
#endif /* ESP8266INTERFACE_HPP_ */

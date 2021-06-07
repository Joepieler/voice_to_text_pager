/*
 * ESP8266.hpp
 *
 *  Created on: Mar 23, 2021
 *      Author: BCAMPAGN
 */

#ifndef ESP8266_HPP_
#define ESP8266_HPP_

#define BUFFERSIZE 100
#define CONNECTION_ATTEMPTS 100

#ifdef __cplusplus
extern "C" {

#include "stm32l4xx_hal.h"
#include <stdio.h>
#include <string.h>


/**
 *  A ESP8266 class to use the ESP8266 via UART.
 */

class ESP8266 {
private:
	/**
	* A private variable.
	* The buffer that the Class use.
	*/
	char buffer_[BUFFERSIZE];

	/**
	* A private variable.
	* The UART Connection to the ESP8266.
	*/
	UART_HandleTypeDef *ESP8266_;


	/**
	* A private variable.
	* The IPaddress of the ESP8266.
	*/
	char IPaddress_[15];


	/**
	* A private function.
	* That waits for the string.
	* @param Timeout in ms.
	* @param Lenght of the total message.
	* @param The string it needs to wait for.
	* @return If the string passes then it returns true else the time passed.
	*/
	bool WaitForString(uint32_t timeout, uint32_t length, const char * string);


	/**
	* A private function.
	* @param Timeout in ms.
	* @param The char it needs to wait for.
	* @return If the char passes then it returns true else the time passed.
	*/
	bool WaitForChar(uint32_t timeout, char value);

public:

	/**
	* A constructor.
	* @param ESP8266 UART port as pointer.
	*/
	ESP8266(UART_HandleTypeDef *ESP8266);

	/**
	* A Destructor
	*/
	virtual ~ESP8266();

	/**
	* A function thats sets the mode of the ESP.
	* @param mode in int (0,1,2).
	* @return if it was successful
	*/
	int StartUp(int mode);

	/**
	* A function that resets the ESP.
	* @return if it was successful
	*/
	int Reset();

	/**
	* A function thats turns on DHCP.
	* @param DHCP on or off (true/false)
	* @param mode in int (0,1,2).
	* @return if it was successful
	*/
	int DHCP(bool enable, int mode);

	/**
	* A function thats says to the ESP connect to WIFI.
	* @param WIFI name.
	* @param password of the WIFI
	* @return if it was successful
	*/
	int Connect(const char *wifiname, const char *password);

	/**
	* A function thats says to the ESP disconnect WIFI.
	* @return if it was successful
	*/
	int Disconnect();

	/**
	* A function thats says to the ESP connect to socket.
	* @param type UDP or TCP name.
	* @param ipaddress of the socket
	* @param port of the socket
	* @return if it was successful
	*/
	int ConnectSocket(const char *type, const char *ipaddress, uint16_t port);

	/**
	* A function thats says to the ESP disconnect from socket.
	* @return if it was successful
	*/
	int DisconnectSocket();

	/**
	* A function thats gets he RSSI(signal strength) from ESP.
	* @return the RSSI of the ESP
	*/
	int8_t GetRSSI();

	/**
	* A function thats ask if the ESP is connected.
	* @return if it is connected.
	*/
	int IsConnected();

	/**
	* A function thats ask for the IP address.
	* @return the IP.
	*/
	const char * GetIP();

	/**
	* A function thats ask for all available networks.
	* @return list of networks.
	*/
	int8_t scan();

	/**
	* A function thats sends the data over the WIFI .
	* @param ID.
	* @param Data pointer
	* @param size of data in bytes
	* @return if it was successful
	*/
	int Send(int id, const void *datat, uint32_t amount);

	/**
	* A function thats sets the received data in the params .
	* @param ID.
	* @param Data pointer
	* @param size of data in bytes
	* @return if it was successful
	*/
	int32_t Receive(int id, void *data, uint32_t & amount);

	/**
	* A function thats sets the poort open to receive data.
	* @param type (UDP/TCP).
	* @param port
	* @return if it was successful
	*/
	bool OpenPort(const char *type, uint32_t port);
};

}
#endif
#endif /* ESP8266_HPP_ */

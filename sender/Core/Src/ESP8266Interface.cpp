/*
 * ESP8266Interface.cpp
 *
 *  Created on: Mar 23, 2021
 *      Author: BCAMPAGN
 */

#include "ESP8266Interface.hpp"

bool ESP8266Interface::IsOK(){
	char ok[] = "OK";
	char response[20];
	HAL_UART_Receive(ESP8266_, (uint8_t *)response, sizeof(response), 1000);
	if(strstr(response, ok) != NULL){
		return 1;
	}
	return 0;
}


ESP8266Interface::ESP8266Interface(UART_HandleTypeDef *ESP8266) {
	// TODO Auto-generated constructor stub
	ESP8266_ = ESP8266;

}

ESP8266Interface::~ESP8266Interface() {
	// TODO Auto-generated destructor stub
}

bool ESP8266Interface::StartUp(int mode){
	char command[] = "AT+CWMODE=%d\r\n";
	char c[sizeof(command)];
	sprintf(c, command, mode);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)c, sizeof(c), HAL_MAX_DELAY);
	return IsOK();
}


bool ESP8266Interface::Reset(){
	char c[] = "AT+RST\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)c, sizeof(c), HAL_MAX_DELAY);
	return IsOK();
}


bool ESP8266Interface::DHCP(bool enable, int mode){
	return 0;
}


bool ESP8266Interface::Connect(const char *wifiname, const char *password){
	char command[] = "AT+CWJAP=\"%s\",\"%s\"\r\n";
	char c[sizeof(wifiname) + sizeof(password) + sizeof(command)];
	sprintf(c, command, wifiname, password);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)c, sizeof(c), HAL_MAX_DELAY);
	return IsOK();
}


bool ESP8266Interface::Disconnect(){
	char command[] = "AT+CWQAP\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	return IsOK();
}


int8_t ESP8266Interface::GetRSSI(){
	return 0;
}


bool ESP8266Interface::IsConnected(){
	return 0;
}


int8_t ESP8266Interface::scan(){
	return 0;
}


bool ESP8266Interface::Send(int id, const void *datat, uint32_t amount){
	return 0;
}


int32_t ESP8266Interface::Receive(int id, void *data, uint32_t amount){
	return 0;
}

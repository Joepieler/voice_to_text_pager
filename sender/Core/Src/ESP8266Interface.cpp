/*
 * ESP8266Interface.cpp
 *
 *  Created on: Mar 23, 2021
 *      Author: BCAMPAGN
 */

#include "ESP8266Interface.hpp"

int ESP8266Interface::IsOK(uint8_t length){
	char response[10+length];
	HAL_UART_Receive(ESP8266_, (uint8_t *)response, sizeof(response), 1);
	if(strstr(response, "OK") != NULL){
		return 0;
	}
	return -1;
}


ESP8266Interface::ESP8266Interface(UART_HandleTypeDef *ESP8266) {
	// TODO Auto-generated constructor stub
	ESP8266_ = ESP8266;

}

ESP8266Interface::~ESP8266Interface() {
	// TODO Auto-generated destructor stub
}

int ESP8266Interface::StartUp(int mode){
	const char command[] = "AT+CWMODE=%d\r\n";
	sprintf(buffer_, command, mode);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, sizeof(command) -2, HAL_MAX_DELAY);
	return IsOK(sizeof(command));
}


int ESP8266Interface::Reset(){
	const char command[] = "AT+RST\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	if(IsOK(sizeof(command)) == 0){
		HAL_Delay(250);
		return 0;
	}
	return -1;
}


int ESP8266Interface::DHCP(bool enable, int mode){
	return 0;
}


int ESP8266Interface::Connect(const char *wifiname, const char *password){
	char command[] = "AT+CWJAP=\"%s\",\"%s\"\r\n";
	uint8_t size = sizeof(command) + sizeof(wifiname) + sizeof(password) + 3;
	sprintf(buffer_, command, wifiname, password);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, size, HAL_MAX_DELAY);

	//check if received
	HAL_UART_Receive(ESP8266_, (uint8_t *)buffer_, size, 1000);
	if(strstr(buffer_, "\r\n") == NULL) return -1;

	HAL_UART_Receive(ESP8266_, (uint8_t *)buffer_, size, 10000);
	if(strstr(buffer_, "CONNECTED") == NULL) return -1;
	return 0;
}


int ESP8266Interface::Disconnect(){
	char command[] = "AT+CWQAP\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	return IsOK(sizeof(command));
}


int ESP8266Interface::ConnectSocket(const char *type, const char *ipaddress, uint16_t poort){
	char mode[] = "AT+CIPMUX=0\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)mode, sizeof(mode), HAL_MAX_DELAY);
	IsOK(sizeof(mode));

	char command[] = "AT+CIPSTART=\"%s\",\"%s\",%d\r\n";
	char c[sizeof(command) + sizeof(type) + sizeof(ipaddress) + 8];
	sprintf(c, command, type, ipaddress, poort);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)c, sizeof(c), HAL_MAX_DELAY);
	HAL_Delay(10);
	return IsOK(sizeof(c));
}


int8_t ESP8266Interface::GetRSSI(){
	return 0;
}


int ESP8266Interface::IsConnected(){
	return 0;
}


int8_t ESP8266Interface::scan(){
	return 0;
}


int ESP8266Interface::Send(int id, const void *data, uint32_t amount){
	char command[] = "AT+CIPSEND=%d\r\n";
	char c[sizeof(command)];
	sprintf(c, command, amount);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)c, sizeof(c)-1, HAL_MAX_DELAY);
	IsOK(sizeof(c)-1);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)data, amount, HAL_MAX_DELAY);
	return IsOK(0);
}


int32_t ESP8266Interface::Receive(int id, void *data, uint32_t amount){
	return 0;
}

/*
 * ESP8266Interface.cpp
 *
 *  Created on: Mar 23, 2021
 *      Author: BCAMPAGN
 */

#include "ESP8266Interface.hpp"

int ESP8266Interface::IsOK(uint8_t length){
	char response[15];
	HAL_UART_Receive(ESP8266_, (uint8_t *)response, sizeof(response), 10);
	if(strstr(response, "OK") != NULL){
		return 0;
	}
	return -1;
}


int ESP8266Interface::IsOK(uint8_t length, uint32_t TimeoutTime){
	char response[10+length];
	HAL_UART_Receive(ESP8266_, (uint8_t *)response, sizeof(response), TimeoutTime);
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
	char response[5];
	while(strstr(response, "OK") == NULL){
		HAL_UART_Receive(ESP8266_, (uint8_t *)response, 5, 1);
	}
	return 0;
}


int ESP8266Interface::Reset(){
	const char command[] = "AT+RST\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	char response[15];
	HAL_UART_Receive(ESP8266_, (uint8_t *)response, sizeof(response), 1);
	if(strstr(response, "OK") != NULL){
		while(strstr(response, "ready") == NULL){
			HAL_UART_Receive(ESP8266_, (uint8_t *)response, 9, 1);
		}
	}
	return 0;
}


int ESP8266Interface::DHCP(bool enable, int mode){
	return 0;
}


int ESP8266Interface::Connect(const char *wifiname, const char *password){
	char command[] = "AT+CWJAP=\"%s\",\"%s\"\r\n";
	char response[16];
	uint8_t size = sizeof(command) + sizeof(wifiname) + sizeof(password) + 3;
	sprintf(buffer_, command, wifiname, password);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, size, HAL_MAX_DELAY);

//	// Wait for WIFICONNECTED
//	while(strstr(response, "CONNECTED") == NULL){
//		HAL_UART_Receive(ESP8266_, (uint8_t *)response, 16, 1);
//	}
//	// Wait for IP
//	while(strstr(response, "GOT") == NULL){
//		HAL_UART_Receive(ESP8266_, (uint8_t *)response, 13, 1);
//	}
	// Wait for OK
	while(strstr(response, "OK") == NULL){
		HAL_UART_Receive(ESP8266_, (uint8_t *)response, 5, 1);
	}
	//Wifi is connected and ready to use
	return 0;
}


int ESP8266Interface::Disconnect(){
	char command[] = "AT+CWQAP\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	return IsOK(sizeof(command));
}


int ESP8266Interface::ConnectSocket(const char *type, const char *ipaddress, uint16_t poort){
	char mode[] = "AT+CIPMUX=0\r\n";
	char response[15];
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)mode, sizeof(mode), HAL_MAX_DELAY);

	// Wait for OK
	while(strstr(response, "OK") == NULL){
		HAL_UART_Receive(ESP8266_, (uint8_t *)response, 5, 1);
	}

	//Clear response because next has also OK
	memset(&response[0], 0, sizeof(response));

	char command[] = "AT+CIPSTART=\"%s\",\"%s\",%d\r\n";
	uint8_t size = sizeof(command) + sizeof(type) + sizeof(ipaddress) + 8;
	sprintf(buffer_, command, type, ipaddress, poort);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, size, HAL_MAX_DELAY);

	//Wait for Connected to server
	while(strstr(response, "OK") == NULL){
		HAL_UART_Receive(ESP8266_, (uint8_t *)response, 15, 1);
	}
	return 0;
}


int8_t ESP8266Interface::GetRSSI(){
	return 0;
}


int ESP8266Interface::IsConnected(){
	return 0;
}

int ESP8266Interface::GetIP(uint8_t IPaddress[4]){
	return 0;
}


int8_t ESP8266Interface::scan(){
	return 0;
}


int ESP8266Interface::Send(int id, const void *data, uint32_t amount){
	const char command[] = "AT+CIPSEND=%lu\r\n";
	uint8_t size = sizeof(command);
	char response[2] = " ";
	sprintf(buffer_, command, amount);

	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, size, HAL_MAX_DELAY);

	// Wait for SEND
	while(response[0] != '>'){
			HAL_UART_Receive(ESP8266_, (uint8_t *)response, 1, 1);
	}

	// Send Data
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)data, amount, HAL_MAX_DELAY);

	//Wait for received message
	while(response[1] != 'O' && response[0] != 'K'){
		response[1] = response[0];
		HAL_UART_Receive(ESP8266_, (uint8_t *)response, 1, 1);
	}
	HAL_UART_Receive(ESP8266_, (uint8_t *)response, 2, 1);

	return 0;
}


int32_t ESP8266Interface::Receive(int id, void *data, uint32_t amount){
	return 0;
}

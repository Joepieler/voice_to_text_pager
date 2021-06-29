/*
 * ESP8266Interface.cpp
 *
 *  Created on: Mar 23, 2021
 *      Author: BCAMPAGN
 */

#include "ESP8266Interface.hpp"

bool ESP8266Interface::WaitForString(uint32_t timeout, uint32_t length, const char * string){
	char response[length] = " ";
	uint32_t i = 0;
	for(i = 0; i < timeout;){
		if(HAL_UART_Receive(ESP8266_, (uint8_t *)response, length, 1) != HAL_TIMEOUT){
			if(strstr(response, string) != NULL){
				return true;
			}
		}else{
			i++;
		}
	}
	if( i > 0){
		return false;
	}
}


bool ESP8266Interface::WaitForChar(uint32_t timeout, char value){
	char a = 0;
	for(uint32_t i = 0; i < timeout;){
			if(HAL_UART_Receive(ESP8266_, (uint8_t *)&a, 1, 1) == HAL_TIMEOUT) i++;
			if(a == value){
				return true;
			}
		}
	return false;
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

	WaitForChar(1,'\n'); //Wait for the Repeat of the message
	return WaitForString(2, 6, "OK"); // Wait for OK;
}


int ESP8266Interface::Reset(){
	const char command[] = "AT+RST\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	WaitForChar(1,'\n'); //Wait for the Repeat of the message
	WaitForString(5, 6, "OK"); // Wait for OK
	return WaitForString(1000, 9, "ready"); // Wait for ready
}


int ESP8266Interface::DHCP(bool enable, int mode){
	return 0;
}


int ESP8266Interface::Connect(const char *wifiname, const char *password){
	char command[] = "AT+CWJAP=\"%s\",\"%s\"\r\n";
	uint8_t size = sizeof(command) + strlen(wifiname) + strlen(password) - 5;
	sprintf(buffer_, command, wifiname, password);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, size, HAL_MAX_DELAY);
	//Response
	WaitForChar(1,'\n'); //Wait for the Repeat of the message
	if(WaitForString(5000, 16, "CONNECTED") == false) return false; // if not connected return false
	if(WaitForString(5000, 13, "IP") == false) return false; // no IP
	return WaitForString(2000, 6, "OK"); // Wait for OK
}


int ESP8266Interface::Disconnect(){
	char command[] = "AT+CWQAP\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	return WaitForString(5, 6, "OK");
}


int ESP8266Interface::ConnectSocket(const char *type, const char *ipaddress, uint16_t poort){
	char mode[] = "AT+CIPMUX=0\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)mode, sizeof(mode), HAL_MAX_DELAY);

	// Wait for OK
	WaitForString(5, 6, "OK");

	//Send Socket connect data
	char command[] = "AT+CIPSTART=\"%s\",\"%s\",%d\r\n";
	uint8_t size = sizeof(command) + strlen(type) + strlen(ipaddress) - 3;
	sprintf(buffer_, command, type, ipaddress, poort);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, size, HAL_MAX_DELAY);

	WaitForChar(1,'\n'); //Wait for the Repeat of the message
	WaitForChar(1,'\n');
	if(WaitForString(1, 9, "CONNECT") == false) return 0;
	return WaitForString(1, 6, "OK");
}

int ESP8266Interface::DisconnectSocket(){
	char command[] = "AT+CIPCLOSE\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);
	WaitForChar(1,'\n');
	if(WaitForString(1, 8, "CLOSED") == false) return 0;
	return WaitForString(1, 6, "OK");
}


int8_t ESP8266Interface::GetRSSI(){
	return 0;
}


int ESP8266Interface::IsConnected(){
	return 0;
}

const char *ESP8266Interface::GetIP(void){
	char command[] = "AT+CIFSR\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)command, sizeof(command), HAL_MAX_DELAY);

	char response[1] = "";

	//wait for repeat
	WaitForChar(1,'\n');

	//get the ip
	WaitForChar(1,'\"');
	response[0] = ' ';

	uint8_t counter = 0;

	while(response[0] != '\"'){
			HAL_UART_Receive(ESP8266_, (uint8_t *)response, 1, 1);
			if(counter < sizeof(IPaddress_)){
				IPaddress_[counter] = response[0];
				counter++;
			}
	}

	//wait for done
	while(HAL_TIMEOUT != HAL_UART_Receive(ESP8266_, (uint8_t *)response, 1, 1)){
	}

	return IPaddress_;
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
	WaitForChar(1,'\n');
	WaitForChar(1,'\n');
	return WaitForString(10, 11, "OK");
}


int32_t ESP8266Interface::Receive(int id, void *data, uint32_t & amount){
	if(HAL_UART_Receive(ESP8266_, (uint8_t *)buffer_, 1, 1) == HAL_TIMEOUT){
		return HAL_TIMEOUT;
	}
	else {
		if(WaitForChar(1,'+')){
			char response[3] = " ";
			HAL_UART_Receive(ESP8266_, (uint8_t *)response, 3, 1);
			if(strstr(response, "IPD") != NULL){

				WaitForChar(1,',');
				WaitForChar(1,',');
				//now comes the size of the data
				uint8_t i = 0;
				char value[4] = "   ";

				for(uint32_t t = 0; t < 10; t++){
					HAL_UART_Receive(ESP8266_, (uint8_t *)buffer_, 1, 1);
					if(buffer_[0] == ':'){
						break;
					}
					value[i]= buffer_[0];
					if( i == 4 ){
						break;
					}
					i++;
				}
				sscanf(value,"%lu", &amount);
				if (value == 0){
					WaitForChar(1,'\n');
					return 0;
				}

				//get the data
				HAL_UART_Receive(ESP8266_, (uint8_t *)data, amount, 5);
				return WaitForChar(5,'\n');
			}
		}
	}
	return 0;
}



bool ESP8266Interface::OpenPort(const char *type, uint32_t port){
	char mode[] = "AT+CIPMUX=1\r\n";
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)mode, sizeof(mode), HAL_MAX_DELAY);
	WaitForChar(1,'\n'); // Wait for repaid command

	// Wait for OK
	if(WaitForString(1, 6, "OK")==false) return false;

	const char command[] = "AT+CIPSERVER=1,%lu\r\n";
	sprintf(buffer_, command, port);
	HAL_UART_Transmit(ESP8266_,  (uint8_t *)buffer_, sizeof(command), HAL_MAX_DELAY);

	WaitForChar(1,'\n'); // Wait for repaid command

	// Wait for OK
	return WaitForString(1, 6, "OK");
}

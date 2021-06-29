/*
 * CommunicationAdaptor.cpp
 *
 *  Created on: 7 jun. 2021
 *      Author: BCAMPAGN
 */

#include "CommunicationAdaptor.hpp"

CommunicationAdaptor::CommunicationAdaptor(ESP8266 *ESP) {
	ESP_ = ESP;
}

CommunicationAdaptor::~CommunicationAdaptor() {
	// TODO Auto-generated destructor stub
}

void CommunicationAdaptor::SendData(const void *data, uint32_t amount){
	ESP_->Send(0, data, amount);
}

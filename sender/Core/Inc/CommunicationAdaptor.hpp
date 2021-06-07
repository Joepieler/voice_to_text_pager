/*
 * CommunicationAdaptor.hpp
 *
 *  Created on: 7 jun. 2021
 *      Author: BCAMPAGN
 */

#ifndef COMMUNICATIONADAPTOR_HPP_
#define COMMUNICATIONADAPTOR_HPP_


#ifdef __cplusplus
extern "C" {
#include "SendTarget.hpp"
#include "ESP8266.hpp"

/**
 *  A Adaptor class for between ESP and recorder.
 */

class CommunicationAdaptor : public SendTarget{
private:
	/**
	* A private variable.
    * The communication device.
    */
	ESP8266 *ESP_;

public:
	/**
    * A constructor.
    * Its needs a ESP pointer.
    * @param A ESP pointer argument.
    */
	CommunicationAdaptor(ESP8266 *ESP);

	/**
    * A destructor.
    */
	virtual ~CommunicationAdaptor();

	/**
    * A Send function that calls Send from the ESP.
    * @param data pointer to the data.
    * @param amount in size in bytes of the data.
    */
	void SendData(const void *data, uint32_t amount);
};


}
#endif
#endif /* COMMUNICATIONADAPTOR_HPP_ */

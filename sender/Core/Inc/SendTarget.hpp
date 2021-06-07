/*
 * SendTarget.hpp
 *
 *  Created on: 7 jun. 2021
 *      Author: BCAMPAGN
 */

#ifndef SENDTARGET_HPP_
#define SENDTARGET_HPP_

#ifdef __cplusplus
extern "C" {
#include <cstdint>


/** A pure adaptor class for between communication and Recorder.
 *  This class is designed for the recorder possibly other wireless
 * technologies may be used without the Recorder class to match.
 */

class SendTarget {
public:

	/**
	* A constructor.
	*/
	SendTarget();

	/**
	* A destructor.
	*/
	virtual ~SendTarget();

	/**
	* A pure virtual Send function.
	* @param data pointer to the data.
	* @param amount in size in bytes of the data.
	*/
	virtual void SendData(const void *data, uint32_t amount)=0;
};


}
#endif
#endif /* SENDTARGET_HPP_ */

/*
 * Recorder.h
 *
 *  Created on: Mar 18, 2021
 *      Author: BCAMPAGN
 */

#ifndef RECORDER_H_
#define RECORDER_H_

#define SAMPLE_RATE 16000
#define MAX_RECORD_TIME 30


#ifdef __cplusplus
extern "C" {
#include "stm32l4xx_hal.h"
#include <Flash.hpp>
#include <SendTarget.hpp>


/**
 *  A Recorder class that is the main class of the recorder.
 */

class Recorder {
private:

	/**
	* A private variable.
	* The timer.
	*/
	TIM_HandleTypeDef *Timer_;

	/**
	* A private variable.
	* The adc of the mic.
	*/
	ADC_HandleTypeDef *Mic_;

	/**
	* A private variable.
	* The target sender.
	*/
	SendTarget *Sender_;

	/**
	* A private variable.
	* The flash.
	*/
	Flash Flash_;

	/**
	* A private variable.
	* The flash.
	*/
	uint8_t * Buffer_;

	/**
	* A private variable.
	* The Counter remembers the size of the audio message.
	*/
	uint64_t Counter_;
public:

	/**
	* A constructor.
	* @param Timer.
	* @param microphone ADC
	* @param adapter sender
	*/
	Recorder(TIM_HandleTypeDef *timer, ADC_HandleTypeDef *mic, SendTarget *sender);

	/**
	* A destructor.
	*/
	virtual ~Recorder();

	/**
	* A main function of the recorder.
	*/
	void main();
};


}
#endif
#endif /* RECORDER_H_ */

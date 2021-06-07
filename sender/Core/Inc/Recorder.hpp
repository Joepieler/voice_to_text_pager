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
#include <ESP8266.hpp>
#include <Flash.hpp>

class Recorder {
private:
	TIM_HandleTypeDef *Timer_;
	ADC_HandleTypeDef *Mic_;
	ESP8266 *ESP_;
	Flash Flash_;
	uint8_t * Buffer_;
	uint64_t Counter_;
public:
	Recorder(TIM_HandleTypeDef *timer, ADC_HandleTypeDef *mic, ESP8266 *ESP);
	virtual ~Recorder();
	void main();
};


}
#endif
#endif /* RECORDER_H_ */

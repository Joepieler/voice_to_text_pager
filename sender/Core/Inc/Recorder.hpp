/*
 * Recorder.h
 *
 *  Created on: Mar 18, 2021
 *      Author: BCAMPAGN
 */

#ifndef RECORDER_H_
#define RECORDER_H_

#define SAMPLE_RATE 16000
#define MAX_RECORD_TIME 5


#ifdef __cplusplus
extern "C" {
#include "stm32l4xx_hal.h"


class Recorder {
private:
	TIM_HandleTypeDef *Timer_;
	ADC_HandleTypeDef *Mic_;
	DAC_HandleTypeDef *Speaker_;
	uint8_t * Buffer_;
	uint64_t Counter_;

public:
	Recorder(TIM_HandleTypeDef *timer, ADC_HandleTypeDef *mic, DAC_HandleTypeDef *speaker);
	virtual ~Recorder();
	void main();
};


}
#endif
#endif /* RECORDER_H_ */

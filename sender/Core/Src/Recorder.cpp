/*
 * Recorder.cpp
 *
 *  Created on: Mar 18, 2021
 *      Author: BCAMPAGN
 */

#include <Recorder.hpp>

Recorder::Recorder(TIM_HandleTypeDef *timer, ADC_HandleTypeDef *mic, DAC_HandleTypeDef *speaker){
	// TODO Auto-generated constructor stub
	Timer_ = timer;
	Mic_ = mic;
	Speaker_ = speaker;
}

Recorder::~Recorder() {
	// TODO Auto-generated destructor stub
}

void Recorder::main(){
	uint32_t timer_val = __HAL_TIM_GET_COUNTER(Timer_);
	Buffer_ = new uint8_t[SAMPLE_RATE * MAX_RECORD_TIME];
	while(true){
		//record if user button is pressed
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET){
			//Record on timer rate 16KHz
			if (__HAL_TIM_GET_COUNTER(Timer_) - timer_val >= 10 && Counter_ < SAMPLE_RATE * MAX_RECORD_TIME){
				HAL_ADC_Start(Mic_);
				Buffer_[Counter_] = HAL_ADC_GetValue(Mic_);
				Counter_++;
				timer_val = __HAL_TIM_GET_COUNTER(Timer_);
			}
		} else if(Counter_ > 0) {
	 		uint64_t i = 0;
			while(i < Counter_){
				if (__HAL_TIM_GET_COUNTER(Timer_) - timer_val >= 10 ){
					HAL_DAC_SetValue(Speaker_, DAC_CHANNEL_1, DAC_ALIGN_8B_R, Buffer_[i]);
					HAL_DAC_Start(Speaker_, DAC_CHANNEL_1);
					i++;
					timer_val = __HAL_TIM_GET_COUNTER(Timer_);
				}
			}
			Counter_ = 0;
		}
	}
}

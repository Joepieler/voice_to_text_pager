/*
 * Recorder.cpp
 *
 *  Created on: Mar 18, 2021
 *      Author: BCAMPAGN
 */

#include <Recorder.hpp>

#define MESSAGESIZE 96
#define TIMER_INTERVAL 160000 / SAMPLE_RATE

Recorder::Recorder(TIM_HandleTypeDef *timer, ADC_HandleTypeDef *mic, ESP8266Interface *ESP){
	// TODO Auto-generated constructor stub
	Timer_ = timer;
	Mic_ = mic;
	ESP_ = ESP;
}

Recorder::~Recorder() {
	// TODO Auto-generated destructor stub
}

void Recorder::main(){
	uint32_t timer_val = __HAL_TIM_GET_COUNTER(Timer_);
	//Buffer_ = new uint8_t[SAMPLE_RATE * MAX_RECORD_TIME];
	uint8_t byts_counter = 0;
	uint8_t tmp = 0;
	uint64_t buffer = 0;
	Counter_ = 0;
	while(true){
		//record if user button is pressed
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET && Counter_ < SAMPLE_RATE * MAX_RECORD_TIME){
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			//Record on timer rate 16KHz
			if (__HAL_TIM_GET_COUNTER(Timer_) - timer_val >= TIMER_INTERVAL){
				timer_val = __HAL_TIM_GET_COUNTER(Timer_);
				HAL_ADC_Start(Mic_);
				tmp = HAL_ADC_GetValue(Mic_);
				buffer = buffer << 8;
				buffer = buffer | tmp;
				byts_counter++;
				if(byts_counter  == 8){
					Flash_.WriteFlash64b(buffer);
					byts_counter = 0;
				}
				Counter_++;
				//timer_val = __HAL_TIM_GET_COUNTER(Timer_);
			}
		} else if(Counter_ > 0) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	 		uint64_t i = 0;
	 		buffer = 0 ;
	 		ESP_->Send(0, "start", 5);
			while(i < Counter_){
				if(Counter_ - i >= MESSAGESIZE){
					uint8_t b[MESSAGESIZE];
					uint32_t buffer2_A = 0;
					uint32_t buffer2_B = 0;
					for(uint8_t x = 0; x < MESSAGESIZE; x+=8){
						Flash_.ReadFlash64b(0x08080000 + x + i, buffer2_B);
						Flash_.ReadFlash64b(0x08080000 + x + 4 + i, buffer2_A);
						b[x] =     buffer2_A >> 24;
						b[x + 1] = buffer2_A >> 16;
						b[x + 2] = buffer2_A >> 8;
						b[x + 3] = buffer2_A;
						b[x + 4] = buffer2_B >> 24;
						b[x + 5] = buffer2_B >> 16;
						b[x + 6] = buffer2_B >> 8;
						b[x + 7] = buffer2_B;
					}
					ESP_->Send(0, b, MESSAGESIZE);
					i +=MESSAGESIZE;
				}
				else
				{
					Counter_ = 0;
					ESP_->Send(0, "end", 3);
					Flash_.ClearFlash();
				}
			}
			Counter_ = 0;
		}
	}
}

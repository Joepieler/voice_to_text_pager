/*
 * RingBuffer.hpp
 *
 *  Created on: Mar 26, 2021
 *      Author: BCAMPAGN
 */

#ifndef RINGBUFFER_HPP_
#define RINGBUFFER_HPP_
#include "stm32l4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER_SIZE 10000

class RingBuffer {
private:
	uint8_t *buffer_;
	uint32_t begin_;
	uint32_t end_;
	uint32_t size_;
public:
	RingBuffer();
	virtual ~RingBuffer();
	bool Push(uint8_t data);
	uint8_t Pop();
	bool pop(uint8_t * data, uint32_t size);
	uint32_t GetSize();
};

#ifdef __cplusplus
}  // end extern "C"
#endif


#endif /* RINGBUFFER_HPP_ */

/*
 * RingBuffer.cpp
 *
 *  Created on: Mar 26, 2021
 *      Author: BCAMPAGN
 */

#include "RingBuffer.hpp"
#include <string.h>

RingBuffer::RingBuffer() {
	// TODO Auto-generated constructor stub
	buffer_ = new uint8_t[BUFFER_SIZE];
	begin_ = 0;
	end_ = 0;
	size_ = 0;
	for(size_t i = 0; i < BUFFER_SIZE; i++){
		buffer_[i] = uint8_t();
	}

}

RingBuffer::~RingBuffer() {
	// TODO Auto-generated destructor stub
	delete[] buffer_;
}


bool RingBuffer::Push(uint8_t data){
	if(size_ < BUFFER_SIZE){
		size_++;
		end_ = end_ < BUFFER_SIZE ? end_ + 1 : 0;
		buffer_[end_] = data;
		return 0;
	}
	return 1;
}

uint8_t RingBuffer::Pop(){
	if(size_ > 0){
		uint8_t value = buffer_[begin_];
		size_--;
		if(begin_ == BUFFER_SIZE - 1){
			begin_ = 0;
		}else{
			begin_++;
		}
		return value;
	}
	return 0;

}

bool RingBuffer::Pop(uint8_t * data, uint32_t size){
	if(size <= size_){
		if(end_ - begin_ >= size){
			memcpy(data, buffer_ + begin_, size);
		}else{
			for(uint32_t i = 0; i < size; i++){
				if(begin_ == BUFFER_SIZE -1 ){
					 begin_ = 0;
				}
				data[i] = buffer_[begin_];
				begin_++;
			}
		}
		size_ = size_ - size;
		return 0;
	}
	return 1;
}

uint32_t RingBuffer::GetSize(){
	return size_;
}

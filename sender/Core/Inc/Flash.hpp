/*
 * Flash.hpp
 *
 *  Created on: Mar 15, 2021
 *      Author: BCAMPAGN
 */

#ifndef INC_FLASH_HPP_
#define INC_FLASH_HPP_

#ifdef __cplusplus
extern "C" {
#include "stm32l4xx_hal.h"

#include <stdint.h>

#define BANK2_BEGIN_ADDRESS 0x08080000
#define BANK2_END_ADDRESS 0x080FFFFF
#define BANK2_BEGIN_PAGE 256
#define BANK2_END_PAGE 511


/**
 *  A Flash class that manages the data on the Flash.
 */

class Flash {
private:

	/**
	* A private variable.
	* The begin address of flash.
	*/
	uint32_t  begin_address_;

	/**
	* A private variable.
	* The size of the used flash.
	*/
	uint32_t  size_ = 0;

public:

	/**
	* A constructor.
	* @param Begin address.
	*/
	Flash(uint32_t beginaddress);

	/**
	* A constructor.
	*/
	Flash();

	/**
	* A destructor.
	*/
	virtual ~Flash();

	/**
	* A function that write data to the begin address + size
	* @param Data.
	* @return If it was successful.
	*/
	uint32_t WriteFlash64b(uint64_t data);

	/**
	* A function that reads the data off the address and sets in the data.
	* @param address of the data.
	* @param data that stands in address of the flash.
	* @return If it was successful.
	*/
	uint32_t ReadFlash64b(uint32_t address, uint32_t &data);

	/**
	* A function that write data to the begin address + size
	* @param Data.
	* @return If it was successful.
	*/
	uint32_t WriteFlash2kb(uint64_t *data);

	/**
	* A function that reads the data off the address and sets in the data.
	* @param address of the data.
	* @param data that stands in address of the flash.
	* @return If it was successful.
	*/
	uint32_t ReadFlash2kb(uint32_t address, uint64_t *data);

	/**
	* A function that clears all the data from begin till the private variable size.
	* @return If it was successful.
	*/
	uint32_t ClearFlash();

	/**
	* A function that clears all the data from begin till the size.
	* @return If it was successful.
	*/
	uint32_t ClearFlash(uint32_t size);
};


}
#endif

#endif /* INC_FLASH_HPP_ */

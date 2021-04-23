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


class Flash {
private:
	uint32_t  begin_address_;
	uint32_t  size_ = 0;
public:
	Flash(uint32_t beginaddress);
	Flash();
	virtual ~Flash();
	uint32_t WriteFlash64b(uint64_t data);
	uint32_t ReadFlash64b(uint32_t address, uint32_t &data);
	uint32_t WriteFlash2kb(uint64_t *data);
	uint32_t ReadFlash2kb(uint32_t address, uint64_t *data);
	uint32_t ClearFlash();
	uint32_t ClearFlash(uint32_t size);
};


}
#endif

#endif /* INC_FLASH_HPP_ */

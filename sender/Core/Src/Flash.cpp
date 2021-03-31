/*
 * Flash.cpp
 *
 *  Created on: Mar 15, 2021
 *      Author: BCAMPAGN
 */

#include <Flash.hpp>

#include <math.h>

uint32_t GetPage(uint32_t address)
{
	//check if address is in range
	if(address >= BANK2_BEGIN_ADDRESS && address <= BANK2_END_ADDRESS){
		return floor((address - BANK2_BEGIN_ADDRESS) / FLASH_PAGE_SIZE) + BANK2_BEGIN_PAGE;

	}
	return -1;
}

Flash::Flash(uint32_t beginaddress):
begin_address_(beginaddress)
{
	ClearFlash(16000 * 30);
}
Flash::Flash():
begin_address_(BANK2_BEGIN_ADDRESS)
{
	ClearFlash(16000 * 30);
}

Flash::~Flash() {
	// TODO Auto-generated destructor stub
}

uint32_t Flash::WriteFlash64b(uint64_t data){
	uint32_t status;
	//First unlock flash
	HAL_FLASH_Unlock();

	//set the data on the flash
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, begin_address_ + size_, data);
	size_ += 8; //because we write a double word (64bits)
	//lock flash again
	HAL_FLASH_Lock();
	//return status
	return status;
}

uint32_t Flash::ReadFlash64b(uint32_t address, uint32_t &data){
	data = *(__IO uint32_t*) address;
	return HAL_OK;
}

uint32_t Flash::WriteFlash2kb(uint64_t *data){
	uint32_t status;
	//First unlock flash
	HAL_FLASH_Unlock();

	//set the data on the flash
	for(uint32_t i = 0; i < 8 ;i++){
		status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FAST, begin_address_ + size_, (uint64_t)&data[32*i]);
		size_ += 32 * 8; //because we write a 32 times a double word (64bits)
	}
	//lock flash again
	HAL_FLASH_Lock();
	//return status
	return status;

}

uint32_t Flash::ReadFlash2kb(uint32_t address, uint64_t *data){
	data = new uint64_t[256];
	for(uint32_t i = 0; i < 256; i++){
		data[i] = (*(__IO uint64_t*) address + (i * 8));
	}
	return HAL_OK;
}

uint32_t Flash::ClearFlash(){
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;

	uint32_t StartPage = GetPage(begin_address_);
	uint32_t EndPageAdress = begin_address_ + size_;
	uint32_t EndPage = GetPage(EndPageAdress);

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page        = StartPage;
	EraseInitStruct.NbPages     = EndPage - StartPage +1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
	 /*Error occurred while page erase.*/
	  return HAL_FLASH_GetError ();
	}

	return HAL_OK;

}

uint32_t Flash::ClearFlash(uint32_t size){
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError;

	uint32_t StartPage = GetPage(begin_address_);
	uint32_t EndPageAdress = begin_address_ + size;
	uint32_t EndPage = GetPage(EndPageAdress);

	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page        = StartPage;
	EraseInitStruct.NbPages     = EndPage - StartPage +1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
	 /*Error occurred while page erase.*/
	  return HAL_FLASH_GetError ();
	}

	return HAL_OK;
}

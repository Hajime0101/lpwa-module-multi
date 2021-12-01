/*
 * eeprom.c
 *
 *  Created on: 2021/06/07
 *      Author: kazuaki
 */

#include <stdio.h>
#include <string.h>
#include "stm32l0xx_hal.h"
#include "eeprom.h"

static void write_Byte_to_eeprom(uint32_t address, uint8_t value) {
	if (!IS_FLASH_DATA_ADDRESS(address)) {
		return;
	}
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address, value);
	HAL_FLASHEx_DATAEEPROM_Lock();
}


//static void write_Halfword_to_eeprom(uint32_t address, uint16_t value) {
//	if (!IS_FLASH_DATA_ADDRESS(address)) {
//		return;
//	}
//	HAL_FLASHEx_DATAEEPROM_Unlock();
//	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_HALFWORD, address, value);
//	HAL_FLASHEx_DATAEEPROM_Lock();
//}

static void write_Word_to_eeprom(uint32_t address, uint32_t value) {
	if (!IS_FLASH_DATA_ADDRESS(address)) {
		return;
	}
	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_WORD, address, value);
	HAL_FLASHEx_DATAEEPROM_Lock();
}

static uint8_t read_Byte_from_eeprom(uint32_t address) {
	return (*(__IO uint8_t*) address);
}

//static uint16_t read_Halfword_from_eeprom(uint32_t address) {
//	return (*(__IO uint16_t*) address);
//}

static uint32_t read_Word_from_eeprom(uint32_t address) {
	return (*(__IO uint32_t*) address);
}



// EEPROMの初期化
void ConfigClearEEPROM(void){
	uint32_t *p;
	// Product
	p = (uint32_t *)MEMORY_MAGIC_STR;
	write_Word_to_eeprom(MEMORY_MAGIC,   *p++);
	write_Word_to_eeprom(MEMORY_MAGIC+4, *p);
	// DeviceID
	write_Word_to_eeprom(MEMORY_DEVICE_ID, 0);
	// TX Repeat
	write_Byte_to_eeprom(MEMORY_TX_REPEAT, 3);
	// CONFIG_FEATURE
	int i;
	for( i=0 ; i<0x40 ; i+=4 ){
		write_Word_to_eeprom(MEMORY_FEATURES+i, 0);
	}
}

void ConfigReadVersion(uint8_t product[]){
	// Product
	uint32_t *p = (uint32_t *)product;
	*p++ = read_Word_from_eeprom(MEMORY_MAGIC);
	*p   = read_Word_from_eeprom(MEMORY_MAGIC+4);
}

uint32_t ConfigReadDeviceID(void) {
	return read_Word_from_eeprom(MEMORY_DEVICE_ID) & 0x00FFFFFF;
}

void ConfigWriteDeviceID(uint32_t id) {
	if ((id & 0x00FFFFFF) != ConfigReadDeviceID()) {
		write_Word_to_eeprom(MEMORY_DEVICE_ID, id & 0x00FFFFFF);
	}
}

uint32_t ConfigReadUplinkID(void){
	return read_Word_from_eeprom(MEMORY_UPLINK_ID) & 0x00FFFFFF;
}

void ConfigWriteUplinkID(uint32_t id){
	if ((id & 0x00FFFFFF) != ConfigReadUplinkID()) {
		write_Word_to_eeprom(MEMORY_UPLINK_ID, id & 0x00FFFFFF);
	}
}


// Broadcast の Seq
static uint16_t config_value_seq = 0;

uint16_t ConfigReadSeq(void){
	return config_value_seq;
}

void ConfigWriteSeq(uint16_t seq) {
	if (seq >= 65535){
		seq = 0;
	}
	config_value_seq = seq;
}

// TXの繰り返し回数
uint8_t ConfigReadTxRepeat(void)
{
	uint8_t t = read_Byte_from_eeprom(MEMORY_TX_REPEAT);
	if( t<0 || t>=8 ) t = 1;
	return t;
}

void ConfigWriteTxRepeat(uint8_t t)
{
	write_Byte_to_eeprom(MEMORY_TX_REPEAT, t);
}


// 機能のフラグ（ATFコマンド関連）
// no が機能番号
uint8_t ConfigReadFeature(uint8_t no)
{
	if( no<FCONF_LOWER_BOUNDARY || no>FCONF_HIGHER_BOUNDARY ) return 0;
	return read_Byte_from_eeprom(MEMORY_FEATURES+no);
}

void ConfigWriteFeature(uint8_t no, uint8_t value)
{
	if( no<FCONF_LOWER_BOUNDARY || no>FCONF_HIGHER_BOUNDARY ) return;
	write_Byte_to_eeprom(MEMORY_FEATURES+no, value);
}

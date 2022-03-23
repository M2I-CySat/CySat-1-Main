/*
 * helper_functions.h
 *
 *  Created on: Feb 7, 2020
 *      Author: student
 */

#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

#include  <stdarg.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "CySatPacketProtocol.h"

HAL_StatusTypeDef debug_printf(char* format, ...);
int debugSerialReceive(uint8_t* data);
void debug_HAL_Status(HAL_StatusTypeDef status);
void FloatToUnsigned8bitArray(uint8_t bytes[4], float float_value);
uint8_t generateCySatChecksum(CySat_Packet_t packet);
uint8_t validateCySatChecksum(CySat_Packet_t packet);

int8_t getBitMask_8b(uint8_t word, uint8_t bitNum);
int8_t getBitMask_16b(uint16_t word, uint8_t bitNum);
int8_t getBitMask_32b(uint32_t word, uint8_t bitNum);
uint8_t setBitMask_8b(uint8_t word, uint8_t bitNum);
uint16_t setBitMask_16b(uint16_t word, uint8_t bitNum);
uint32_t setBitMask_32b(uint32_t word, uint8_t bitNum);
uint8_t resetBitMask_8b(uint8_t word, uint8_t bitNum);
uint16_t resetBitMask_16b(uint16_t word, uint8_t bitNum);
uint32_t resetBitMask_32b(uint32_t word, uint8_t bitNum);

void hexToAscii(uint8_t hex, uint8_t* ascii_out);
void asciiToHex(uint8_t* ascii, uint8_t* hex_out);
uint32_t crc32(uint8_t* message);

#endif /* HELPER_FUNCTIONS_H_ */
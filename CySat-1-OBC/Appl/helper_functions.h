/*
 * helper_functions.h
 *
 *  Created on: Feb 7, 2020
 *      Author: student
 */

#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

#include "OBC.h"
#include  <stdarg.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

HAL_StatusTypeDef debug_printf(char* format, ...);

/**
 * Flash the Green LED located on the OBC
 * @param count - The number of cycles to flash
 * @param period - The amount of time in milliseconds between LED states
 */
void debug_led_green(int count, int period);

/**
 * Flash the Amber LED located on the OBC
 * @param count - The number of cycles to flash
 * @param period - The amount of time in milliseconds between LED states
 */
void debug_led_amber(int count, int period);

int debugSerialReceive(uint8_t* data);
void debug_HAL_Status(HAL_StatusTypeDef status);
uint16_t FloatToUnsigned16bits(float float_value);
uint32_t crc32(uint8_t* message, int length, uint8_t* crc_ascii);
#endif /* HELPER_FUNCTIONS_H_ */

/*
 * Payload.h
 *
 *  Created on: Oct 2, 2021
 *  Updated on: Nov 1, 2021
 *      Author: Alexis Aurandt
 */

#ifndef PAYLOAD_H_
#define PAYLOAD_H_

#include "stm32f4xx_hal.h"
#include "MCU_Init.h"
#include <stdbool.h>

#define PAYLOAD_UART_TIMEOUT 10000

/*************************** CALIBRATION FUNCTIONS *****************************/
HAL_StatusTypeDef GET_PAYLOAD_POWER_STATUS(bool*);
HAL_StatusTypeDef SET_PAYLOAD_RF_BANDWIDTH(float);
HAL_StatusTypeDef SET_PAYLOAD_CALIBRATION1(float);
HAL_StatusTypeDef SET_PAYLOAD_CALIBRATION2(float);
HAL_StatusTypeDef SET_PAYLOAD_DC_GAIN(float);
HAL_StatusTypeDef SET_PAYLOAD_FFTSIZE(uint16_t);
HAL_StatusTypeDef SET_PAYLOAD_FREQUENCY(float);
HAL_StatusTypeDef SET_PAYLOAD_INTEGRAL(float);
HAL_StatusTypeDef SET_PAYLOAD_SAMPLE_RATE(float);
HAL_StatusTypeDef SET_PAYLOAD_SPAVG(float);
HAL_StatusTypeDef RESET_PAYLOAD_TO_DEFAULTS(bool*);
HAL_StatusTypeDef GET_PAYLOAD_VALUES(float*, float*, float*, float*, float*,
                                    float*, float*, float*, float*);

/*************************** MEASUREMENT FUNCTIONS *****************************/
HAL_StatusTypeDef TAKE_MEASUREMENT(uint16_t);
HAL_StatusTypeDef DAT_FILE_TRANSFER();
HAL_StatusTypeDef KELVIN_FILE_TRANSFER();

/*************************** HELPER FUNCTIONS **********************************/
HAL_StatusTypeDef PAYLOAD_WRITE(uint8_t command, uint8_t* in_data_ptr, uint8_t in_byte);
HAL_StatusTypeDef PAYLOAD_READ(uint8_t command, uint8_t* out_data_ptr, uint8_t out_byte);
int convert_to_int(uint8_t*, int);
void convert_to_bytes(uint8_t* data, int num, int length);

#endif /* PAYLOAD_H_ */

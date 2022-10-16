/*
 * ADCS.h
 *
 *  Created on: Nov 21, 2019
 *  Updated on: March 14, 2021
 *      Author: Bryan Friestad, Xiangzhu Yan, and Alexis Aurandt
 */

#ifndef ADCS_H_
#define ADCS_H_

#include "stm32f4xx_hal.h"
#include "MCU_Init.h"
#include "helper_functions.h"

#define ADCS_UART_TIMEOUT 1000

/*************************** TELECOMMAND FUNCTIONS **********************************/
HAL_StatusTypeDef TC_2(uint32_t, uint16_t);
HAL_StatusTypeDef TC_7(uint8_t);
HAL_StatusTypeDef TC_10(uint8_t);
HAL_StatusTypeDef TC_11(uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
HAL_StatusTypeDef TC_13(uint8_t,uint8_t,uint16_t);
HAL_StatusTypeDef TC_14(uint8_t);
HAL_StatusTypeDef TC_15(float,float,float);
HAL_StatusTypeDef TC_17(int16_t);
HAL_StatusTypeDef TC_33(float,float,float);
HAL_StatusTypeDef TC_34(float,float,float,float,float,float);
HAL_StatusTypeDef TC_35(float,float,float,float,float,float);
HAL_StatusTypeDef TC_36(float,float,float,uint8_t);
HAL_StatusTypeDef TC_44(float,float,float,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t,uint8_t);
HAL_StatusTypeDef TC_45(double,double,double,double,double,double,double,double);
HAL_StatusTypeDef TC_63();
HAL_StatusTypeDef TC_64();

/*************************** TELEMETRY REQUEST FUNCTIONS **********************************/
HAL_StatusTypeDef TLM_140(uint32_t*,uint16_t*);
HAL_StatusTypeDef TLM_146(float*, float*, float*);
HAL_StatusTypeDef TLM_147(float*, float*, float*);
HAL_StatusTypeDef TLM_150(float*, float*, float*);
HAL_StatusTypeDef TLM_151(float*, float*, float*);
HAL_StatusTypeDef TLM_153(float*, float*, float*);
HAL_StatusTypeDef TLM_154(float*, float*, float*);
HAL_StatusTypeDef TLM_155(float*, float*, float*);
HAL_StatusTypeDef TLM_156(int16_t*);
HAL_StatusTypeDef TLM_157(float*, float*, float*);
HAL_StatusTypeDef TLM_158(int16_t*,int16_t*,int16_t*);
HAL_StatusTypeDef TLM_168(uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*);
HAL_StatusTypeDef TLM_169(uint8_t*,uint8_t*,uint8_t*,uint8_t*);
HAL_StatusTypeDef TLM_170(int16_t*,int16_t*,int16_t*);
HAL_StatusTypeDef TLM_172(float*,float*,float*);
HAL_StatusTypeDef TLM_197(uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*,uint8_t*);
HAL_StatusTypeDef TLM_199(float*,float*,float*);

/*************************** HELPER FUNCTIONS **********************************/
HAL_StatusTypeDef ADCS_TELEMETRY(uint8_t command, uint8_t* data_ptr, uint8_t out_byte);
HAL_StatusTypeDef ADCS_TELECOMMAND(uint8_t command[], uint8_t in_byte);

#endif

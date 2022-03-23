/*
 * UHF.h
 *
 *  Created on: Feb 11, 2020
 *  Updated on: March 19, 2021
 *      Author: Kyle Muehlenthaler and Alexis Aurandt
 */

#ifndef UHF_H_
#define UHF_H_

#include "stm32f4xx_hal.h"
#include "MCU_Init.h"

#define UHF_I2C_ADDRESS 0x22
#define UHF_UART_TIMEOUT 1000

/*************************** BEACON FUNCTIONS **********************************/
HAL_StatusTypeDef START_BEACON();
HAL_StatusTypeDef END_BEACON();
HAL_StatusTypeDef SET_BEACON_PERIOD(uint16_t);
HAL_StatusTypeDef SET_BEACON_TEXT(uint8_t*,uint8_t);
HAL_StatusTypeDef SET_DESTINATION_CALLSIGN(uint8_t*);
HAL_StatusTypeDef SET_SOURCE_CALLSIGN(uint8_t*);

/*************************** PIPE FUNCTIONS ************************************/
HAL_StatusTypeDef START_PIPE();
HAL_StatusTypeDef END_PIPE();
HAL_StatusTypeDef SET_PIPE_TIMEOUT(uint8_t);

/*************************** STATUS FUNCTIONS **********************************/
HAL_StatusTypeDef GET_UHF_STATUS(uint8_t*);
HAL_StatusTypeDef GET_UHF_UPTIME(uint32_t*);
HAL_StatusTypeDef GET_UHF_NUM_TRANSMITTED_PACKETS(uint32_t*);
HAL_StatusTypeDef GET_UHF_NUM_RECEIVED_PACKETS(uint32_t*);
HAL_StatusTypeDef GET_UHF_NUM_RECEIVED_PACKETS_WITH_ERRORS(uint32_t*);
HAL_StatusTypeDef RESTORE_UHF_DEFAULTS();
HAL_StatusTypeDef GET_UHF_TEMP(float*);
HAL_StatusTypeDef SET_UHF_LOW_POWER_MODE();

/*************************** ANTENNA FUNCTIONS *********************************/
HAL_StatusTypeDef DEPLOY_ANTENNA(uint8_t);
HAL_StatusTypeDef CONFIGURE_ANTENNA();
HAL_StatusTypeDef GET_ANTENNA_STATUS(uint8_t* status);

/*************************** HELPER FUNCTIONS **********************************/
HAL_StatusTypeDef UHF_WRITE(uint8_t command[], uint8_t in_byte);
HAL_StatusTypeDef UHF_READ(uint8_t command[], uint8_t* data_ptr, uint8_t in_byte, uint8_t out_byte);

#endif /* UHF_H_ */

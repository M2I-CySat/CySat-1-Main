/* UHF.h 
*  Author: Kyle Muehlenthaler
*  Feb 11, 2020*/

#ifndef UHF_H_
#define UHF_H_

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef UHF_WRITE(I2C_HandleTypeDef handle, uint8_t command[], uint8_t* data_ptr, uint8_t in_byte, uint8_t out_byte);
HAL_StatusTypeDef START_PIPE(I2C_HandleTypeDef handle);
HAL_StatusTypeDef PIPE_END(I2C_HandleTypeDef handle);

HAL_StatusTypeDef START_BEACON(I2C_HandleTypeDef handle);
//HAL_StatusTypeDef SET_BEACON_EOL(I2C_HandleTypeDef handle);
//HAL_StatusTypeDef SET_BEACON_HELLO(I2C_HandleTypeDef handle);
HAL_StatusTypeDef SET_BEACON_TEXT(I2C_HandleTypeDef handle, uint8_t* text, uint16_t size, uint8_t* data_ptr);
HAL_StatusTypeDef SET_BEACON_PERIOD(I2C_HandleTypeDef handle, uint32_t period_int, uint8_t* data_ptr);
HAL_StatusTypeDef BEACON_END(I2C_HandleTypeDef handle);

//HAL_StatusTypeDef SET_PIPE_TIMEOUT_LONG(I2C_HandleTypeDef handle);
HAL_StatusTypeDef READ_SOFTWARE_VERSION(I2C_HandleTypeDef handle, uint8_t* data);
//HAL_StatusTypeDef CHECK_TEMP(I2C_HandleTypeDef handle);

HAL_StatusTypeDef DEPLOY_ANTENNA(I2C_HandleTypeDef handle, uint8_t* data_ptr);
uint8_t HEALTH_CHECKS(I2C_HandleTypeDef handle, uint8_t* data);
HAL_StatusTypeDef GET_UHF_STATUS(I2C_HandleTypeDef handle, uint8_t* data);

#define UHF_I2C_ADDRESS         0x22 /*Possibly change to 0x23*/

#endif /* UHF_H_ */

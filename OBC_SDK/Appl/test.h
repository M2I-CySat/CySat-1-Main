/*
 * test.h
 *
 *  Created on: Nov 1, 2021
 *  Updated on: Nov 1, 2021
 *      Author: Alexis Aurandt
 */

#ifndef TEST_H_
#define TEST_H_

#include "stm32f4xx_hal.h"
#include "MCU_Init.h"
#include <stdbool.h>

/****************************** PAYLOAD TESTS *********************************/
bool PAYLOAD_TEST_SETTING_VALUES();
bool PAYLOAD_TEST_RETRIEVING_DATA();

#endif /* TEST_H_ */

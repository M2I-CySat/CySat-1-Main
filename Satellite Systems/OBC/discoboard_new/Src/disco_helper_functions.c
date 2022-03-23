/*
 * disco_helper_functions.c
 *
 *  Created on: October 12, 2020
 *      Author: Alexis Aurandt
 */

#include "disco_helper_functions.h"

void turnGreenLedOn(){
	HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);
}
void turnGreenLedOff(){
	HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_RESET);
}
void turnRedLedOn(){
	HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_SET);
}
void turnRedLedOff(){
	HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_RESET);
}

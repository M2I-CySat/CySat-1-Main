/*
 * ADCSFunctions.h
 *
 *  Created on: Feb 19, 2020
 *  Updated on: November 18, 2021
 *      Authors: Alexis Aurandt and Xiangzhu Yan
 */
#ifndef ADCSFUNCTIONS_H_
#define ADCSFUNCTIONS_H_

#include <ADCS.h>
#include "stm32f4xx_hal.h"
#include "MCU_Init.h"
#include "main.h"
#include <stdbool.h>

typedef enum
{
  NO_ERROR,
  FAULT_COMMAND_SPEED,
  FAULT_PITCH_ANGLE,
  FAULT_Y_RATE,
} y_ramp_result_t;

void Disable_ADCS(void);
void Detumbling_To_Y_Thomson(void);
void Magnetometer_Deployment(void);
void Magnetometer_Calibration(void);
bool Angular_Rate_And_Pitch_Angle_Estimation(void);
y_ramp_result_t Y_Wheel_Ramp_Up_Test(void);
void Y_Momentum_Activation(void);
void Sun_And_Nadir_Commissioning(void);

#endif

/*
 * BatteryCapacity.h
 *
 *  Created on: Mar 14, 2020
 *      Author: Bryan Friestad
 */

#ifndef BATTERYCAPACITY_H_
#define BATTERYCAPACITY_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

#define INITIAL_BATTERY_CAPACITY 20.8 * 3600 //20.8 WH * 3600 sec/H = 74880 Joules

//float EPS_Battery_Capacity = INITIAL_BATTERY_CAPACITY;
//#define getEPS_Battery_Capactity() EPS_Battery_Capacity

typedef struct BatteryMeasurement{
    float batt_voltage; //EPS battery bus voltage in volts
    float batt_current; //EPS battery bus current in amps
    uint64_t time; //time in microseconds since 1970
    //TODO: maybe need to add the following values to the measurement
    /*
     * 5v bus voltage
     * 5v bus current
     * 3.3v bus voltage
     * 3.3v bus current
     * Solar array X axis voltage
     * Solar array X+ axis current
     * Solar array X- axis current
     * Solar array Y axis voltage
     * Solar array Y+ axis current
     * Solar array Y- axis current
     * Solar array Z axis voltage
     * Solar array Z+ axis current
     * Solar array Z- axis current
     */
} BatteryMeasurement_t;

#define MAX_STORED_BATT_MEASUREMENTS 16

//BatteryMeasurement_t EPS_Battery_History[MAX_STORED_BATT_MEASUREMENTS];

float calculateEnergyDischarge(BatteryMeasurement_t e1, BatteryMeasurement_t e2);
float calculateEnergyRecharge(BatteryMeasurement_t e1, BatteryMeasurement_t e2);

void testEnergyDischarge();

int8_t addNewBatteryMeasurement(BatteryMeasurement_t e1);

int8_t updateCapacityCallback(I2C_HandleTypeDef handle);

#endif /* BATTERYCAPACITY_H_ */

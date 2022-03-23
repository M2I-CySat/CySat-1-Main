/*
 * BatteryCapacity.h
 *
 *  Created on: Mar 14, 2020
 *      Author: Bryan Friestad
 */

#ifndef BATTERYCAPACITY_H_
#define BATTERYCAPACITY_H_

#include "stm32f4xx_hal.h"

#define INITIAL_BATTERY_CAPACITY 20.8 * 3600 //20.8 WH * 3600 sec/H = 74880 Joules

//float EPS_Battery_Capacity = INITIAL_BATTERY_CAPACITY;
//#define getEPS_Battery_Capactity() {EPS_Battery_Capacity}

typedef struct BatteryMeasurement{
    float batt_voltage; //EPS battery bus voltage in volts
    float batt_current; //EPS battery bus current in amps
    uint64_t time; //time in microseconds since 1970
} BatteryMeasurement_t;

#define MAX_STORED_BATT_MEASUREMENTS 16

BatteryMeasurement_t EPS_Battery_History[MAX_STORED_BATT_MEASUREMENTS];

float calculateEnergyDischarge(BatteryMeasurement_t e1, BatteryMeasurement_t e2);
float calculateEnergyRecharge(BatteryMeasurement_t e1, BatteryMeasurement_t e2);

void testEnergyDischarge();

int8_t addNewBatteryMeasurement(BatteryMeasurement_t e1);

int8_t updateCapacityCallback(I2C_HandleTypeDef handle); //calling this function gets a new measurement set, calculates discharge/recharge and logs something 

#endif /* BATTERYCAPACITY_H_ */

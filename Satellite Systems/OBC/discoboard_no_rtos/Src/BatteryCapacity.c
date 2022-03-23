/* BatteryCapacity.c
 * Created: March 3, 2020
 * Author: Bryan Friestad
 */

#include "BatteryCapacity.h"
#include "helper_functions.h"

float calculateEnergyDischarge(BatteryMeasurement_t e1, BatteryMeasurement_t e2){
	float dv, di;
	uint64_t dt;

	dt = e2.time - e1.time;
	dv = e2.batt_voltage - e1.batt_voltage;
	di = e2.batt_current - e1.batt_current;

	float batt_bus_discharge = (dv * di) / 3.0f
				 + (dv * e1.batt_current + di * e1.batt_voltage) / 2.0f
				 + (e1.batt_current * e1.batt_voltage);
	batt_bus_discharge *= dt;

	return -(batt_bus_discharge);
}

void testEnergyDischarge(){
	BatteryMeasurement_t e1, e2;
	
	e1.batt_voltage = 3.9f;
	e1.batt_current = 0.1f;
	e1.time = 12;

	e2.batt_voltage = 3.75f;
    e2.batt_current = 0.25f;
    e2.time = 72;

	debug_printf("Calculated value: %f should be about -40.05", calculateEnergyDischarge(e1, e2));

	return;
}

float calculateEnergyRecharge(BatteryMeasurement_t e1, BatteryMeasurement_t e2){
	return -1;
}

int8_t addNewBatteryMeasurement(BatteryMeasurement_t e1){
	return -1;
}

int8_t updateCapacityCallback(I2C_HandleTypeDef handle){
	return -1;
}

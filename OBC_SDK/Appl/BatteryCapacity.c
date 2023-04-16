/* BatteryCapacity.c
 * Created: March 3, 2020
 * Author: Bryan Friestad
 */

#include "BatteryCapacity.h"
#include "helper_functions.h"
#include "EPS.h"

/**
  * @brief Takes in two battery measurements and determines the total energy consumed between those two
  * time measurements. Implementation of this code is based on formulae derived in the /doc/EPS/Battery_Capacity_Formulas.pdf
  * @param e1: the earlier of the two measurements
  * @param e2: the later of the two measurements
  * @retval Returns the difference in joules between the two measurements (should be a negative value)
  */
float calculateEnergyDischarge(BatteryMeasurement_t e1, BatteryMeasurement_t e2){
	float dv, di;
	uint64_t dt;

	//dt = (e2.time - e1.time) / 1000000; //to convert from microseconds to seconds
	// Value is now in seconds by default I believe
	dt= (e2.time-e1.time); // dt in seconds
	dv = e2.batt_voltage - e1.batt_voltage;
	di = e2.batt_current - e1.batt_current;

	float batt_bus_discharge = (dv * di) / 3.0f
				 + (dv * e1.batt_current + di * e1.batt_voltage) / 2.0f
				 + (e1.batt_current * e1.batt_voltage);
	batt_bus_discharge *= dt;

	return -(batt_bus_discharge);
}

//Test function
void testEnergyDischarge(){
	BatteryMeasurement_t e1, e2;

	e1.batt_voltage = 3.9f;
	e1.batt_current = 0.1f;
	e1.time = 12000000; //12 million microseconds

	e2.batt_voltage = 3.75f;
    e2.batt_current = 0.25f;
    e2.time = 72000000; //72 million microseconds

	debug_printf("Calculated value: %f should be about -40.05", calculateEnergyDischarge(e1, e2));
}

/**
  * TODO: IMPLEMENT
  * @brief Calculates the amount of energy regenerated by the solar cells between two given
  * battery measurements.
  * @param e1: the earlier of the two measurements
  * @param e2: the later of the two measurements
  * @retval Returns he difference in joules between the two measurements (should be a positive value)
  */
float calculateEnergyRecharge(BatteryMeasurement_t e1, BatteryMeasurement_t e2){
	return -1;
}

/**
  * TODO: IMPLEMENT
  * @brief Adds a new measurement to the start of the EPS measurement history array, popping off the oldest
  * @param e1: the new measurement to add to the history
  * @retval Whether the operation was a success or not. 0 is good, -1 if bad
  */
int8_t addNewBatteryMeasurement(BatteryMeasurement_t e1){

    // TODO: Remove or comment out Z+ code as Z+ panel does not exist

    float temp_batt_val=0; //TODO: Move to startup variable declaration section before launch

    // Anything that is set to -1 isn't done yet but I'm working on it -Steven
    // These EPS functions return based on success and write the return value to a pointer, so I had to work around that a bit

    // Battery Voltage And Current
	READ_EPS_BATTERY_VOLTAGE(&temp_batt_val);
	e1.batt_voltage=temp_batt_val;

	READ_EPS_BATTERY_CURRENT(&temp_batt_val);
	e1.batt_current=temp_batt_val;

	// Current Time
	e1.time=time(NULL); // NOT COMPLETE

	// 3 and 5 Volt Buses
	//READ_EPS_5V_VOLTAGE // THERE ARE NO 5 VOLT VOLTAGE READINGS
	e1.bus5_voltage=5;

	READ_EPS_5V_CURRENT(&temp_batt_val);
	e1.bus5_current=temp_batt_val;

	e1.bus3_voltage=3.3;

	READ_EPS_3V_CURRENT(&temp_batt_val);
	e1.bus3_current=temp_batt_val;;


	// Solar Panel Voltages And Currents
	READ_EPS_SOLAR_X_VOLTAGE(&temp_batt_val);
	e1.solarX_voltage=temp_batt_val;
	READ_EPS_SOLAR_X_POS_CURRENT(&temp_batt_val);
	e1.solarXPos_current=temp_batt_val;
	READ_EPS_SOLAR_X_NEG_CURRENT(&temp_batt_val);
	e1.solarXNeg_current=temp_batt_val;


	READ_EPS_SOLAR_Y_VOLTAGE(&temp_batt_val);
	e1.solarY_voltage=temp_batt_val;
	READ_EPS_SOLAR_Y_POS_CURRENT(&temp_batt_val);
	e1.solarYPos_current=temp_batt_val;
	READ_EPS_SOLAR_Y_NEG_CURRENT(&temp_batt_val);
	e1.solarYNeg_current=temp_batt_val;


	READ_EPS_SOLAR_Z_VOLTAGE(&temp_batt_val);
	e1.solarZ_voltage=temp_batt_val;
	READ_EPS_SOLAR_Z_POS_CURRENT(&temp_batt_val);
	e1.solarZPos_current=temp_batt_val;
	READ_EPS_SOLAR_Z_NEG_CURRENT(&temp_batt_val);
	e1.solarZNeg_current=temp_batt_val;



	printf("Debug Console Test");




	return -1;
}

/**
  * TODO: IMPLEMENT
  * @brief Calling this function gets a new measurement from the EPS, calculates discharge & recharge, adds the new
  * measurement to the history, adjusts the total battery capacity using the calculated discharge & recharge, then
  * logs the occurance to the system logs
  * @param handle: The I2C upon which you want to speak with the EPS module
  * @retval The status of the various opeations, negative values should represent failures and positive ones should be good
  */
int8_t updateCapacityCallback(I2C_HandleTypeDef handle){
    //comm with EPS to generate new BatteryMeasurement_t new_measurement
    //addNewBatteryMeasurement(new_measurement);
    //float total_delta_capacity = calculateEnergyRecharge(EPS_Battery_History[1], EPS_Battery_History[0]) + calculateEnergyDischarge(EPS_Battery_History[1], EPS_Battery_History[0]);
    //EPS_Battery_Capacity = min(EPS_Battery_Capacity + total_delta_capacity, INITIAL_BATTERY_CAPACITY);
    //system log something
	return -1;
}
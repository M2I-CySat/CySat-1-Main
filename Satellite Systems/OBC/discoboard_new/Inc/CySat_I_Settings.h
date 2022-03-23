/*
 * CySat_I_Settings.h
 *
 *  Created on: Mar 9, 2020
 *      Author: bryan friestad
 */

#ifndef CYSAT_I_SETTINGS_H_
#define CYSAT_I_SETTINGS_H_

#include "EPS.h"
//#include "MCU_Init.h"

//UHF Transceiver is enabled by EPS general purpose output 5
#define turn_On_UHF_Transceiver() { enable_EPS_Output_5(hi2c1); }
#define turn_Off_UHF_Transceiver() { disable_EPS_Output_5(hi2c1); }

//Boost Board is enabled by EPS general purpose output 1
#define turn_On_Boost_Board() { enable_EPS_Output_1(hi2c1); }
#define turn_Off_Boost_Board() { disable_EPS_Output_1(hi2c1); }

//SDR is enabled by EPS general purpose output 3
#define turn_On_SDR() { enable_EPS_Output_3(hi2c1); }
#define turn_Off_SDR() { disable_EPS_Output_3(hi2c1); }

#define INITIAL_BEACON_TEXT "Hello, Earth! I am ISU's CySat-I"
#define INITIAL_BEACON_TEXT_SIZE 32
#define INITIAL_BEACON_PERIOD 30

#endif /* CYSAT_I_SETTINGS_H_ */

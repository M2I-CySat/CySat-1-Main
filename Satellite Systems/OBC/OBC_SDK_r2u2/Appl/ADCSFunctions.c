/*
 * ADCSFunctions.c
 *
 *  Created on: Feb 19, 2020
 *  Updated on: November 18, 2021
 *      Authors: Alexis Aurandt and Xiangzhu Yan
 */

#include <ADCSFunctions.h>
#include "AppTasks.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool MAGNETOMETER_FAULT = 0;

/*
 * @brief Disable ADCS
 */
void Disable_ADCS(){
    osMutexWait(ADCS_Active_Mutex, 500);
    ADCS_ACTIVE = 0;
    osMutexRelease(ADCS_Active_Mutex);
}
/*
 * @brief Find the initial angular rates and detumble towards Y_Thomson spin.
 *        The final result should be x and z rates around 0 deg/s and a y-rate of -2 deg/s.
 */
void Detumbling_To_Y_Thomson(void){
    // Enable ADCS
    TC_10(1);
    // CubeControl Signal and Motor are on, all others are off
    TC_11(1,1,0,0,0,0,0,0,0);
    // Set Estimation Mode to Magnetometer Rate Filter
    TC_14(2);

    float estimated_x, estimated_y, estimated_z;
    float rate_x, rate_y, rate_z;
    float mag_x, mag_y, mag_z;
    bool converged = 0;

    // Initial Rate Estimation
    do{
        osDelay(10000); //Check rate every 10s
        // Estimated Angular Rates in deg/s
        TLM_147(&estimated_x, &estimated_y, &estimated_z);
        // Rate Sensor Rates in deg/s
        TLM_155(&rate_x, &rate_y, &rate_z);
        // Magnetometer Measurement in uT
        TLM_151(&mag_x, &mag_y, &mag_z);

        if((abs((int)rate_x) > 35) || (abs((int)rate_y) > 35) || (abs((int)rate_z) > 35)){
            if((abs((int)rate_x) > 75) || (abs((int)rate_y) > 75) || (abs((int)rate_z) > 75)){
                TC_36(0.0,0.0,0.0,2);
                osDelay(10000);
                TLM_155(&rate_x, &rate_y, &rate_z);
                if((abs((int)rate_x) > 150) || (abs((int)rate_y) > 150) || (abs((int)rate_z) > 150)){
                    TC_36(0.0,0.0,0.0,4);
                    osDelay(10000);
                    TLM_155(&rate_x, &rate_y, &rate_z);
                    if((abs((int)rate_x) > 300) || (abs((int)rate_y) > 300) || (abs((int)rate_z) > 300)){
                        TC_36(0.0,0.0,0.0,12);
                    }
                }
            }
            TC_14(1); //Spinning too fast, so must use MEMS rate sensing
            converged = 0;
            break;
        }
        else if(MAGNETOMETER_FAULT){
            //Note: Should check with R2U2 if magnetometer is behaving properly or switch to MEMS rate sensing
            // CubeControl Motor are on, all others are off
            TC_11(0,1,0,0,0,0,0,0,0);
        }
        converged = 1;
    }while((abs((int)(estimated_x - rate_x)) > 1) || (abs((int)(estimated_y - rate_y)) > 1) || (abs((int)(estimated_z - rate_z)) > 1));
    //Note: Should check if estimated rates are somehow mixed up between x, y, and z TC 34 can fix this
    //Note: Add timeout

    // Fast Detumbling
    do{
        // Rate Sensor Rates in deg/s
        TLM_155(&rate_x, &rate_y, &rate_z);
        if((abs((int)rate_x)>100) || (abs((int)rate_y)>100) || (abs((int)rate_z)>100)){
            TC_13(8,1,600); //Very Fast Spin Detumbling
            osDelay(60000);
            // Note: Should check R2U2 if we are detumbling and not spinning up more
        }
        else if((abs((int)rate_x)>30) || (abs((int)rate_y)>30) || (abs((int)rate_z)>30)){
            TC_13(9,1,600); //Fast Spin Detumbling
            osDelay(60000);
            // Note: Should check R2U2 if we are detumbling and not spinning up more
        }
    }while((abs((int)rate_x)>30) || (abs((int)rate_y)>30) || (abs((int)rate_z)>30));

    if(!converged){
        // CySat is spinning slow enough for the Magnetometer Rate Filter
         TC_14(2);

        // Waiting for RKF to converge
        while((abs((int)(estimated_x - rate_x)) > 1) || (abs((int)(estimated_y - rate_y)) > 1) || (abs((int)(estimated_z - rate_z)) > 1)){
            osDelay(10000); //Check rate every 10s
            // Estimated Angular Rates in deg/s
            TLM_147(&estimated_x, &estimated_y, &estimated_z);
            // Rate Sensor Rates in deg/s
            TLM_155(&rate_x, &rate_y, &rate_z);
            // Magnetometer Measurement in uT
            TLM_151(&mag_x, &mag_y, &mag_z);

            if(1){
                //Note: Should check with R2U2 if magnetometer is behaving properly or switch to MEMS rate sensing
                // CubeControl Motor are on, all others are off
                TC_11(0,1,0,0,0,0,0,0,0);
            }
        }
    }

    TC_13(2,1,600);
    osDelay(60000);
    // Note: Should check R2U2 if we are detumbling and not spinning up more

    //Detumbling towards Y Thompson(x and z should reach rates around 0 deg)
    TC_13(2,1,0xFFFF);
    while((abs((int)rate_x)>1) || (abs((int)rate_z)>1) || (abs((int)rate_y)>3)){
        osDelay(10000);
        TLM_155(&rate_x, &rate_y, &rate_z);
        //Note: If detumbling is not effective or poor performance, then maybe the magnetometer deployed of
        //      its own accord.
    }

    //Final Detumbling Command
    TC_13(2,1,0);
}

/*
 * @brief Deploy the Magnetometer
 * WARNING: THIS CAN ONLY HAPPEN ONCE IN SPACE!!!!!
 * Notes: Deployment should occur while ground contact with the satellite has been established...?
 */
void Magnetometer_Deployment(void){
    // Enable ADCS
    TC_10(1);
    // CubeControl Signal and Motor are on, all others are off
    TC_11(1,1,0,0,0,0,0,0,0);
    // Set Estimation Mode to Magnetometer Rate Filter
    TC_14(2);

    //delay one minute
    osDelay(60000);


    // No Control Mode
    TC_13(0,1,0);

    int16_t i_sampled_x, i_sampled_y, i_sampled_z;
    int16_t sampled_x, sampled_y, sampled_z;

    // Sampled RAW Magnetometer values(initial values)
    TLM_170(&i_sampled_x, &i_sampled_y, &i_sampled_z);

    debug_printf("Deploying Magnetometer!\r\n");

    // First attempt at deploying Magnetometer
    TC_7(10);

    //5 second delay
    osDelay(5000);

    // Sampled RAW Magnetometer values
    TLM_170(&sampled_x, &sampled_y, &sampled_z);

    // Magnetometer RAW measurements experienced a step change indicating success
    if((abs((int)(i_sampled_x - sampled_x))<1) && (abs((int)(i_sampled_y - sampled_y))<1) && (abs((int)(i_sampled_z - sampled_z))<1)){
        // Second attempt at deploying Magnetometer
        TC_7(5);
        osDelay(8000);
        TLM_170(&sampled_x, &sampled_y, &sampled_z);
        if((abs((int)(i_sampled_x - sampled_x))<1) && (abs((int)(i_sampled_y - sampled_y))<1) && (abs((int)(i_sampled_z - sampled_z))<1)){
            // Third attempt at deploying Magnetometer
            TC_7(10);
            osDelay(13000);
            TLM_170(&sampled_x, &sampled_y, &sampled_z);
        }
    }

    if((abs((int)(i_sampled_x - sampled_x))>0) && (abs((int)(i_sampled_y - sampled_y))>0) && (abs((int)(i_sampled_z - sampled_z))>0)){
        // Set Magnetometer Mounting Configuration
        // Note: Will need to find alpha, beta, and gamma prior to deployment
        TC_33(0.0, 0.0, 0.0);

        // Save Configuration
        TC_63();
    }
}

/*
 * @brief Calibrate the Magnetometer
 * Notes: REQUIRES GROUND PROCESSING!!!
 */
void Magnetometer_Calibration(){
    //Enable ADCS
    TC_10(1);
    // CubeContrl signals and motors on, and everthing else off
    TC_11(1,1,0,0,0,0,0,0,0);
    // Set control mode to Detumbling
    TC_13(1);
    //delay one orbit
    osDelay(5400000); //Assuming 90 minute orbit from Angular_Rate_And_Pitch_Angle_Estimation()
    //TODO rest of steps are on ground.
}

/*
 * @brief Turn on pitch estimation and verify
 * @return Success of the pitch angle estimation mode
 * Prerequisite: Y-Thomson spin state was held for 1 orbit
 */
bool Angular_Rate_And_Pitch_Angle_Estimation(){
    // Enable ADCS
    TC_10(1);
    // CubeControl Signal and Motor are on, all others are off
    TC_11(1,1,0,0,0,0,0,0,0);
    // Set Estimation Mode to Magnetometer Rate Filter with pitch estimation
    TC_14(3);
    // Detumble Y-Thomson Mode
    TC_13(2,1,0xFFFF);

    float estimated_x, estimated_y, estimated_z;
    float estimated_roll, estimated_pitch, estimated_yaw;
    float last_estimated_pitch = 0;
    bool result = 0;

    osDelay(4860000); //Delay for 4860 seconds or 90% of the 90 minute orbit

    for(int i = 0; i < 54; i++){
        osDelay(10000); //Check rate every 10s
        // Estimated Angular Rates in deg/s
        TLM_147(&estimated_x, &estimated_y, &estimated_z);
        // Estimated Attitude Angles in degrees
        TLM_146(&estimated_roll, &estimated_pitch, &estimated_yaw);
        //To-Do:
        last_estimated_pitch = estimated_pitch;
    }
    return result;
}

/*
 * @brief Turn on pitch estimation and verify
 * @return Success of the pitch angle estimation mode
 * Prerequisite: Y-Thomson spin state was held for 1 orbit
 * Prerequisite: Satellite is in view of the ground station for TLE information
 * Prerequisite: Need to know what speed to command the Y-Wheel!!!
 */
y_ramp_result_t Y_Wheel_Ramp_Up_Test(){
    HAL_StatusTypeDef status;
    // Enable ADCS
    status = TC_10(1);
    // CubeControl Signal, CubeControl Motor, and Motor Power are on, all others are off
    status = TC_11(1,1,0,0,0,0,0,1,0);
    // Set Estimation Mode to Magnetometer Rate Filter with pitch estimation
    status = TC_14(3);
    // No Control Mode
    status = TC_13(0,1,0);
    // Command the Y-Wheel to rotate at ??? RPM
    osDelay(5000);
    status = TC_17(-4167);

    float estimated_x, estimated_y, estimated_z;
    float estimated_roll, estimated_pitch, estimated_yaw;
    int16_t speed_y;
    float last_estimated_pitch = 0;
    y_ramp_result_t result = NO_ERROR;

    osDelay(60000); // Delay for 1 minute

    for(int i = 0; i < 60; i++){ // Check pitch change for last minute
        osDelay(1000);
        // Estimated Attitude Angles in degrees
        status = TLM_146(&estimated_roll, &estimated_pitch, &estimated_yaw);
        if(i!=0 && abs((int)(estimated_pitch-last_estimated_pitch))>2){
            result = FAULT_PITCH_ANGLE;
        }
        last_estimated_pitch = estimated_pitch;
    }

    // Estimated Angular Rates in deg/s
    status = TLM_147(&estimated_x, &estimated_y, &estimated_z);
    // Wheel speed in RPM
    status = TLM_156(&speed_y);
    if(speed_y < -4200 || speed_y > -4125){
        result = FAULT_COMMAND_SPEED;
    }
    else if(abs((int)estimated_y) < .5)
        result = FAULT_Y_RATE;

    // Command the Y-Wheel to stop
    status = TC_17(0);
    // Detumble Y-Thomson Mode
    status = TC_13(2,1,0xFFFF);

    osDelay(30000); // Delay for 30 seconds for wheel to slow down

    // Estimated Angular Rates in deg/s
    status = TLM_147(&estimated_x, &estimated_y, &estimated_z);
    // Wheel speed in RPM
    status = TLM_156(&speed_y);
    if(speed_y != 0){
        result = FAULT_COMMAND_SPEED;
        debug_printf("Didn't stop.\r\n");
    }
    else if(estimated_y < -3 || estimated_y > -1){
        result = FAULT_Y_RATE;
    }

    return result;
}

/*
 * @brief Commission Y Momentum Mode
 * Prerequisite: Y-Thomson spin state was held for 1 orbit
 * Prerequisite: Magnetometer rate filter with pitch estimation mode has been active for 1 minute
 */
void Y_Momentum_Activation(){
    //To-Do: Need to obtain these from ground station
    uint32_t sec;
    uint16_t millisec;
    double inclination, eccentricity, right_ascension, argument, b_star, mean_motion, mean_anomaly, epoch;
    // Enable ADCS
    TC_10(1);
    // Set Current Unix Time
    TC_2(sec, millisec);
    // Set Current TLEs
    TC_45(inclination, eccentricity, right_ascension, argument, b_star, mean_motion, mean_anomaly, epoch);
    // Save to orbit parameters to flash memory
    TC_64();
    // Set Estimation Paramters 2 (To-Do: must find defaults for other parameters that currently equal 1)
    TC_44(1.0, 1.0, 1.0, 0, 0, 0,1,1,1);
    // Set Estimation Mode to Magnetometer Rate Filter with pitch estimation
    TC_14(3);
    // Y-Momentum stabilized - Initial Pitch Acquisition
    TC_13(3,1,1200);

    float estimated_x, estimated_y, estimated_z;
    float estimated_roll, estimated_pitch, estimated_yaw;
    int16_t speed_y;
    float latitude, longitude, altitude;

    // Estimated Angular Rates in deg/s
    TLM_147(&estimated_x, &estimated_y, &estimated_z);
    // Estimated Attitude Angles in degrees
    TLM_146(&estimated_roll, &estimated_pitch, &estimated_yaw);
    // Wheel speed in RPM
    TLM_156(&speed_y);
    // Satellite Position LLH
    TLM_150(&latitude, &longitude, &altitude);
    //To-Do

}

/*
 * @brief Sun and Nadir Comissioning
 * Prerequisite: Y-momentum control with stable attitude
 */
void Sun_And_Nadir_Commissioning(){
    uint8_t cubeControl_signal, cubeControl_motor, cubeSense_power, cubeStar_power;
    uint8_t cubeWheel1_power, cubeWheel2_power, cubeWheel3_power, motor_power, GPS_power;
    // Current Power Selections
    TLM_197(&cubeControl_signal, &cubeControl_motor, &cubeSense_power, &cubeStar_power, &cubeWheel1_power,
            &cubeWheel2_power, &cubeWheel3_power, &motor_power, &GPS_power);
    // Enable CubeSense power and all others are defaults
    TC_11(cubeControl_signal, cubeControl_motor, 1, cubeStar_power, cubeWheel1_power, cubeWheel2_power, cubeWheel3_power, motor_power, GPS_power);
    // Set Estimation Paramters 2 (To-Do: must find defaults for other parameters that currently equal 1)
    TC_44(1.0, 1.0, 1.0, 0, 0, 1,1,1,1);
    // Set Estimation Mode to Full-State EKF
    TC_14(5);
}

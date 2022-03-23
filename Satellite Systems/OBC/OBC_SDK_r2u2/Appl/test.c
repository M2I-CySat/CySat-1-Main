/*
 * Test.c
 *
 *  Created on: Nov 1, 2021
 *  Updated on: Nov 1, 2021
 *      Author: Alexis Aurandt
 */

#include <Payload.h>

/****************************** PAYLOAD TESTS *********************************/
/*
 * @brief Tests that we can correctly set the parameter values
 */
bool PAYLOAD_TEST_SETTING_VALUES(){
    HAL_StatusTypeDef status;
    bool power_status, default_status;
    float bandwidth, calib_1, calib_2, gain, fftsize, frequency, integral, rate, spavg;
    status = GET_PAYLOAD_POWER_STATUS(&power_status);
    if(status != HAL_OK || power_status != true)
        return false;
    status = RESET_PAYLOAD_TO_DEFAULTS(&default_status);
    if(status != HAL_OK || default_status != true)
        return false;
    status = GET_PAYLOAD_VALUES(&bandwidth, &calib_1, &calib_2, &gain, &fftsize,
                                            &frequency, &integral, &rate, &spavg);
    if(status != HAL_OK)
        return false;
    else if(bandwidth != 15000000 || calib_1 != 4075.5 || !(calib_2 >= -343 && calib_2 <= -342) ||
            gain != 1 || fftsize != 8192 || !(frequency >= 1410000000 && frequency <= 1420000000) || integral != 2
            || rate != 10000000 || spavg != 1)
        return false;

    status = SET_PAYLOAD_RF_BANDWIDTH(37500000);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_CALIBRATION1(5000.78);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_CALIBRATION2(-467.79);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_DC_GAIN(5);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_FFTSIZE(9162);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_FREQUENCY(500000000);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_INTEGRAL(3);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_SAMPLE_RATE(5300000);
    if(status != HAL_OK)
        return false;
    status = SET_PAYLOAD_SPAVG(3);
    if(status != HAL_OK)
        return false;
    status = GET_PAYLOAD_VALUES(&bandwidth, &calib_1, &calib_2, &gain, &fftsize,
                                                &frequency, &integral, &rate, &spavg);
    if(status != HAL_OK)
        return false;
    else if(bandwidth != 37500000 || !(calib_1 >= 5000 && calib_1 <= 5001) || !(calib_2 >= -468 && calib_2 <= -467) ||
            gain != 5 || fftsize != 9162 || !(frequency >=499999999 && frequency <= 500000001) || integral != 3
            || rate != 5300000 || spavg != 3)
        return false;
    status = RESET_PAYLOAD_TO_DEFAULTS(&default_status);
    if(status != HAL_OK || default_status != true)
        return false;
    status = GET_PAYLOAD_VALUES(&bandwidth, &calib_1, &calib_2, &gain, &fftsize,
                                                &frequency, &integral, &rate, &spavg);
    if(status != HAL_OK)
        return false;
    else if(bandwidth != 15000000 || calib_1 != 4075.5 || !(calib_2 >= -343 && calib_2 <= -342) ||
            gain != 1 || fftsize != 8192 || !(frequency >= 1410000000 && frequency <= 1420000000) || integral != 2
            || rate != 10000000 || spavg != 1)
        return false;
    return true;
}

/*
 * @brief Tests that we can successfully retrieve data from the payload
 */
bool PAYLOAD_TEST_RETRIEVING_DATA(){
    HAL_StatusTypeDef status;
    status = TAKE_MEASUREMENT(10);
    if(status != HAL_OK)
        return false;
    status = DAT_FILE_TRANSFER();
    if(status != HAL_OK)
        return false;
    status = KELVIN_FILE_TRANSFER();
    if(status != HAL_OK)
        return false;
    return true;
}

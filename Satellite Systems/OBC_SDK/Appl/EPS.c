/*
 * EPS.c
 *
 *  Code is implemented according to the EnduroSat EPS I2C User Manual revision 2.2
 *
 *  Created on: Dec 4, 2019
 *  Last Updated: April 16, 2021
 *  Author: Bryan Friestad and Alexis Aurandt
 */

#include "EPS.h"
#include "AppTasks.h"
#include "helper_functions.h"

/***************************************** BATTERY FUNCTIONS (read commands 1 - 4) **********************************************************************/

/**
  * @brief Reads the voltage level of the EPS battery voltage bus and stores the value in the pointed to address
  * @param volt_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATTERY_VOLTAGE(float* volt_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 1; //battery voltage command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*volt_ptr) = 0.0023394775f * (float) raw_voltage; //multiply by the conversion coeff
    //should not be greater than 0.0023394775 * 4095 = 9.58 V
    return status;
}

/**
  * @brief Reads the current level of the EPS battery bus and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATTERY_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 2; //battery current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0030517578f * (float) raw_value; //multiply by the conversion coeff
    //should not be greater than 0.0030517578f * 4095 = 12.497 A
    return status;
}

/**
  * @brief Reads the current level of the EPS BCR bus and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BCR_VOLTAGE(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 3; //battery current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0023394775f * (float) raw_value; //multiply by the conversion coeff
    //should not be greater than 0.0023394775f * 4095 = 9.58 V
    return status;
}

/**
  * @brief Reads the current level of the EPS BCR bus and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BCR_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 4; //battery current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0015258789f * (float) raw_value; //multiply by the conversion coeff
    //should not be greater than 0.0015258789f * 4095 = 6.25 A
    return status;
}

/***************************************** SOLAR PANEL FUNCTIONS (read commands 5 - 13) **********************************************************************/

/**
  * @brief Reads the voltage of the X-axis in volts and stores the value in the pointed to address
  * @param volt_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_X_VOLTAGE(float* volt_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 5; //solar panel x-axis voltage command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*volt_ptr) = 0.0024414063f * (float) raw_voltage; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the voltage of the X- facing solar panels in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_X_NEG_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 6; //solar panel x- current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the voltage of the X+ facing solar panels in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_X_POS_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 7; //solar panel x+ current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the voltage of the Y-axis in volts and stores the value in the pointed to address
  * @param volt_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_Y_VOLTAGE(float* volt_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 8; //solar panel y-axis voltage command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*volt_ptr) = 0.0024414063f * (float) raw_voltage; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the voltage of the Y- facing solar panels in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_Y_NEG_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 9; //solar panel y- current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the voltage of the Y+ facing solar panels in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_Y_POS_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 10; //solar panel y+ current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    return status;
}


/**
  * @brief Reads the voltage of the Y-axis in volts and stores the value in the pointed to address
  * @param volt_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_Z_VOLTAGE(float* volt_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 11; //solar panel z-axis voltage command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*volt_ptr) = 0.0024414063f * (float) raw_voltage; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the voltage of the Z- facing solar panels in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_Z_NEG_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 12; //solar panel z- current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the voltage of the Z+ facing solar panels in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SOLAR_Z_POS_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 13; //solar panel z+ current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    return status;
}


/***************************************** VOLTAGE BUS CURRENT FUNCTIONS (read commands 14 - 15) **********************************************************************/


/**
  * @brief Reads the current of the EPS 3.3v bus in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_3V_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 14; //3.3v bus current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0020345052f * (float) raw_current; //multiply by the conversion coeff
    return status;
}

/**
  * @brief Reads the current of the EPS 5v bus in amps and stores the value in the pointed to address
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_5V_CURRENT(float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 15; //5v bus current command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0020345052f * (float) raw_current; //multiply by the conversion coeff
    return status;
}

/***************************************** TEMPURATURE FUNCTIONS (read commands 18 - 22 & 37 - 40) **********************************************************************/

/**
  * @brief Reads the temperature of the EPS microcontroller in degrees Celsius and stores the value in the pointed to address
  * @param temp_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_MCU_TEMP(float* temp_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 18; //MCU temperature command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*temp_ptr) = (((float) raw_value * 0.0006103516f) - 0.986f) / 0.00355f;
    return status;
}

/**
  * @brief Reads the temperature of the EPS battery cell 1 in degrees Celsius and stores the value in the pointed to address
  * @param temp_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATT_1_TEMP(float* temp_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 19; //battery cell 1 temperature command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*temp_ptr) = 0.00390625f * (float) raw_value;
    return status;
}

/**
  * @brief Reads the temperature of the EPS battery cell 2 in degrees Celsius and stores the value in the pointed to address
  * @param temp_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATT_2_TEMP(float* temp_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 20; //battery cell 2 temperature command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*temp_ptr) = 0.00390625f * (float) raw_value;
    return status;
}

/**
  * @brief Reads the temperature of the EPS battery cell 3 in degrees Celsius and stores the value in the pointed to address
  * @param temp_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATT_3_TEMP(float* temp_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 21; //battery cell 3 temperature command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*temp_ptr) = 0.00390625f * (float) raw_value;
    return status;
}

/**
  * @brief Reads the temperature of the EPS battery cell 4 in degrees Celsius and stores the value in the pointed to address
  * @param temp_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATT_4_TEMP(float* temp_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 22; //battery cell 4 temperature command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*temp_ptr) = 0.00390625f * (float) raw_value;
    return status;
}

/***************************************** INPUT/OUTPUT CONDITION FUNCTIONS (read commands 23 - 25) **********************************************************************/

/**
  * @brief Reads raw return value of the Input Condition command and stores the value in the pointed to address
  * @param cond_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_INPUT_CONDITION(uint16_t *cond_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 23; //input conditions command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*cond_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/**
  * @brief Reads raw return value of the Output Condition 1 command and stores the value in the pointed to address
  * @param cond_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_OUTPUT_CONDITION1(uint16_t *cond_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 24; //output conditions command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*cond_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/**
  * @brief Reads raw return value of the Output Condition 2 command and stores the value in the pointed to address
  * @param cond_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_OUTPUT_CONDITION2(uint16_t *cond_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 24; //output conditions command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*cond_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/***************************************** COUNTER FUNCTIONS (read commands 25 - 28 & 48) **********************************************************************/

/**
  * @brief Reads the current count of EPS power cycles and stores the value in the pointed to address
  * @param count_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_POWER_ON_CYCLES(uint16_t *count_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 26; //power on cycles count command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*count_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/**
  * @brief Reads the current count of EPS under-voltage occurrences and stores the value in the pointed to address
  * @param count_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_UNDER_VOLT_COUNT(uint16_t *count_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 27; //under voltage count command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*count_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/**
  * @brief Reads the current count of EPS short-circuit occurrences and stores the value in the pointed to address
  * @param count_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_SHORT_CIRCUIT_COUNT(uint16_t *count_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 28; //short circuit count command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*count_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/**
  * @brief Reads the current count of EPS over-temp occurrences and stores the value in the pointed to address
  * @param count_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_OVER_TEMP_COUNT(uint16_t *count_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 29; //over temperature count command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*count_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/**
  * @brief Reads the current count of EPS battery charge cycles and stores the value in the pointed to address
  * @param count_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_CHARGE_CYCLES(uint16_t *count_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 48; //charging cycles count command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*count_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/***************************************** OUTPUT DEFAULT FUNCTIONS (read commands 43 + 44) **********************************************************************/
/**
  * @brief Reads the default values of LUPs and fast charge modes and stores the value in the pointed to address
  * @param default_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_DEFAULTS1(uint16_t *default_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 43; //output 1 default value command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*default_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}

/**
  * @brief Reads the default values of OUT1-OUT6 and stores the value in the pointed to address
  * @param default_ptr: The address to store the returned uint16_t value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_DEFAULTS2(uint16_t *default_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 44; //output 3 default value command
    uint8_t data[2];
    status = EPS_READ(command, data);
    if(status != HAL_OK){
        return status;
    }
    (*default_ptr) = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    return status;
}


/********************************************** WRITE FUNCTIONS **********************************************************************************/
HAL_StatusTypeDef enable_EPS_Vbatt_Bus(){
    return EPS_WRITE(1, 1); //turn command 1 to the ON state
}

HAL_StatusTypeDef disable_EPS_Vbatt_Bus(){
    return EPS_WRITE(1, 2); //turn command 1 to the FORCE OFF state
}

HAL_StatusTypeDef enable_EPS_BCR_Bus(){
    return EPS_WRITE(2, 1); //turn command 2 to the ON state
}

HAL_StatusTypeDef disable_EPS_BCR_Bus(){
    return EPS_WRITE(2, 0); //turn command 2 to the OFF state
}

HAL_StatusTypeDef enable_EPS_5v_Bus(){
    return EPS_WRITE(4, 1); //turn command 4 to the ON state
}

HAL_StatusTypeDef disable_EPS_5v_Bus(){
    return EPS_WRITE(4, 2); //turn command 4 to the FORCE OFF state
}

HAL_StatusTypeDef enable_EPS_LUP_3v(){
    return EPS_WRITE(5, 0); //turn command 5 to the ON state
}

HAL_StatusTypeDef disable_EPS_LUP_3v(){
    return EPS_WRITE(5, 1); //turn command 5 to the OFF state
}

HAL_StatusTypeDef enable_EPS_LUP_5v(){
    return EPS_WRITE(6, 0); //turn command 6 to the ON state
}

HAL_StatusTypeDef disable_EPS_LUP_5v(){
    return EPS_WRITE(6, 1); //turn command 6 to the OFF state
}

HAL_StatusTypeDef enable_EPS_Fast_Charge1(){
    return EPS_WRITE(8, 1); //turn command 8 to the ON state (HIGH = Fast?, LOW = Slow?)
}

HAL_StatusTypeDef disable_EPS_Fast_Charge1(){
    return EPS_WRITE(8, 0); //turn command 8 to the ON state (HIGH = Fast?, LOW = Slow?)
}

HAL_StatusTypeDef enable_EPS_Fast_Charge2(){
    return EPS_WRITE(9, 1); //turn command 9 to the ON state (HIGH = Fast?, LOW = Slow?)
}

HAL_StatusTypeDef disable_EPS_Fast_Charge2(){
    return EPS_WRITE(9, 0); //turn command 9 to the OFF state (HIGH = Fast?, LOW = Slow?)
}

HAL_StatusTypeDef enable_Boost_Board(){
    return EPS_WRITE(10, 1); //turn command 10 to the ON state
}

HAL_StatusTypeDef disable_Boost_Board(){
    return EPS_WRITE(10, 0); //turn command 10 to the OFF state
}

HAL_StatusTypeDef enable_Payload(){
    return EPS_WRITE(11, 1); //turn command 11 to the ON state
}

HAL_StatusTypeDef disable_Payload(){
    return EPS_WRITE(11, 0); //turn command 11 to the OFF state
}

HAL_StatusTypeDef enable_EPS_Output_3(){
    return EPS_WRITE(12, 1); //turn command 12 to the ON state
}

HAL_StatusTypeDef disable_EPS_Output_3(){
    return EPS_WRITE(12, 0); //turn command 12 to the OFF state
}

HAL_StatusTypeDef enable_UHF(){
    return EPS_WRITE(14, 1); //turn command 14 to the ON state
}

HAL_StatusTypeDef disable_UHF(){
//    osThreadTerminate(myUHFTask);
//    osThreadDef(myUHFTask, UHF_Task, osPriorityNormal, 0, 512);
//    osThreadCreate(osThread(myUHFTask), NULL);
    return EPS_WRITE(14, 0); //turn command 14 to the OFF state
}

HAL_StatusTypeDef enable_EPS_Output_6(){
    return EPS_WRITE(15, 1); //turn command 15 to the ON state
}

HAL_StatusTypeDef disable_EPS_Output_6(){
    return EPS_WRITE(15, 0); //turn command 15 to the OFF state
}

HAL_StatusTypeDef enable_EPS_Batt_Heater_1(){
    return EPS_WRITE(16, 1); //turn command 16 to the ON state
}

HAL_StatusTypeDef disable_EPS_Batt_Heater_1(){
    return EPS_WRITE(16, 0); //turn command 16 to the OFF state
}

HAL_StatusTypeDef enable_EPS_Batt_Heater_2(){
    return EPS_WRITE(17, 1); //turn command 17 to the ON state
}

HAL_StatusTypeDef disable_EPS_Batt_Heater_2(){
    return EPS_WRITE(17, 0); //turn command 17 to the OFF state
}

HAL_StatusTypeDef enable_EPS_Batt_Heater_3(){
    return EPS_WRITE(18, 1); //turn command 18 to the ON state
}

HAL_StatusTypeDef disable_EPS_Batt_Heater_3(){
    return EPS_WRITE(18, 0); //turn command 18 to the OFF state
}

/********************************************** HELPER FUNCTIONS **********************************************************************************/
/**
  * @brief Sends a write command to the EPS module over a given I2C instance and reads back a response
  * @param command : the command to be used on the transceiver
  * @param data_ptr : A pointer to allow information from the health check to be viewed.
  * @retval Returns if the hal function call succeeded, or if it failed
  */
HAL_StatusTypeDef EPS_READ(uint8_t command, uint8_t* data_ptr){
    osMutexWait(EPS_I2C_Mutex, 2500);

    HAL_StatusTypeDef status = HAL_ERROR;
    status = HAL_I2C_Master_Transmit(&hi2c1, (uint16_t) (EPS_I2C_ADDRESS << 0x1), &command, 1, EPS_I2C_TIMEOUT);
    if(status != HAL_OK){
        osMutexWait(Num_I2C_Errors_Mutex, 500);
        NUM_I2C_ERRORS++;
        osMutexRelease(Num_I2C_Errors_Mutex);
        osMutexRelease(EPS_I2C_Mutex);
        return status;
    }
    status = HAL_I2C_Master_Receive(&hi2c1, (uint16_t) (EPS_I2C_ADDRESS << 0x1), data_ptr, 2, EPS_I2C_TIMEOUT);

    if(status != HAL_OK){
        osMutexWait(Num_I2C_Errors_Mutex, 500);
        NUM_I2C_ERRORS++;
        osMutexRelease(Num_I2C_Errors_Mutex);
    }

    osMutexRelease(EPS_I2C_Mutex);
    return status;
}

/**
  * @brief Sends a write command to the EPS module over a given I2C instance
  * @param state: the state to set the output specified by the command to (0 or 1)
  * @retval Returns if the hal function call succeeded, or if it failed
  */
HAL_StatusTypeDef EPS_WRITE(uint8_t command, uint8_t state){
    osMutexWait(EPS_I2C_Mutex, 1500);

    HAL_StatusTypeDef status = HAL_OK;
    uint8_t data[2];
    data[0] = command;
    data[1] = state;
    status = HAL_I2C_Master_Transmit(&hi2c1, (EPS_I2C_ADDRESS << 0x1), data, 2, EPS_I2C_TIMEOUT); //Possibly size needs to be 3, and data should maybe be replaced with data buffer

    if(status != HAL_OK){
        osMutexWait(Num_I2C_Errors_Mutex, 500);
        NUM_I2C_ERRORS++;
        osMutexRelease(Num_I2C_Errors_Mutex);
        if(status == HAL_ERROR){
            debug_led_green(50,50);
        }
        if(status == HAL_BUSY){
            debug_led_amber(50,50);
        }
        if(status == HAL_TIMEOUT){
            debug_led_green(10,50);
            debug_led_amber(10,50);
            debug_led_green(10,50);
            debug_led_amber(10,50);
            debug_led_green(10,50);
            debug_led_amber(10,50);
        }


    }

    osMutexRelease(EPS_I2C_Mutex);
    return status;
}

/*
 * EPS.c
 *
 *  Code is implemented according to the EnduroSat EPS I2C User Manual revision 2.0
 *  TODO: Must update code to match revision 2.2, which matches the firmware and hardware
 *  updates made by EnduroSat when we sent the module back to their facilties before Spring Break 2020
 *
 *  TODO: Add more "READ" functions which return the values (via pass-by-pointer) rather than
 *  simply printing them out on the Debug UART line
 *
 *  Created on: Dec 4, 2019
 *  Last Updated: May 3, 2020
 *  Author: Bryan Friestad
 */

#include "EPS.h"
#include "MCU_Init.h"
#include "helper_functions.h"

/***************************************** BATTERY FUNCTIONS (read commands 1 - 4) **********************************************************************/

/**
  * @brief Prints the current voltage of the EPS battery voltage bus in volts
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATTERY_VOLTAGE(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    float voltage;

    status = READ_EPS_BATTERY_VOLTAGE(handle, &voltage);
    if(status != HAL_OK)
        return status;

    debug_printf("Battery Voltage: %f V", voltage);
    return status;
}

/**
  * @brief Reads the voltage level of the EPS battery voltage bus and stores the value in the pointed to address
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @param volt_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATTERY_VOLTAGE(I2C_HandleTypeDef handle, float* volt_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 1; //battery voltage command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*volt_ptr) = 0.0023394775f * (float) raw_voltage; //multiply by the conversion coeff
    //should not be greater than 0.0023394775 * 4095 = 9.58
    return status;
}

/**
  * @brief Prints the current of the EPS battery bus in amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATTERY_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    float current;

    status = READ_EPS_BATTERY_CURRENT(handle, &current);
    if(status != HAL_OK)
        return status;

    debug_printf("Battery Current: %f A", current);
    return status;
}

/**
  * @brief Reads the current level of the EPS battery bus and stores the value in the pointed to address
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BATTERY_CURRENT(I2C_HandleTypeDef handle, float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 2; //battery current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0030517578f * (float) raw_value; //multiply by the conversion coeff
    //should not be greater than 0.0030517578f * 4095 = 12.497 A
    return status;
}

/**
  * @brief Prints the voltage of the EPS BCR bus in volts
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BCR_VOLTAGE(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    float voltage;

    status = READ_EPS_BCR_VOLTAGE(handle, &voltage);
    if(status != HAL_OK)
        return status;

    debug_printf("BCR Voltage: %f V", voltage);
    return status;
}

/**
  * @brief Reads the voltage level of the EPS BCR bus and stores the value in the pointed to address
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @param volt_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BCR_VOLTAGE(I2C_HandleTypeDef handle, float* volt_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 3; //bcr voltage command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*volt_ptr) = 0.0023394775f * (float) raw_voltage; //multiply by the conversion coeff
    //should not be greater than 0.0023394775 * 4095 = 9.58v
    return status;
}

/**
  * @brief Prints the current of the EPS BCR bus in amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BCR_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    float current;

    status = READ_EPS_BCR_CURRENT(handle, &current);
    if(status != HAL_OK)
        return status;

    debug_printf("BCR Current: %f A", current);
    return status;
}

/**
  * @brief Reads the current level of the EPS BCR bus and stores the value in the pointed to address
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @param curr_ptr: The address to store the returned float value at
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_BCR_CURRENT(I2C_HandleTypeDef handle, float* curr_ptr){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 4; //bcr current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*curr_ptr) = 0.0015258789f * (float) raw_current; //multiply by the conversion coeff
    //should not be more than 0.0015258789 * 4095 = 6.248 A
    return status;
}

/**
  * @brief Prints whether or not the EPS is fast charging or not
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_FAST_CHARGE(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 42; //battery fast charge status command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    if(raw){
        debug_printf("Battery fast charge is ON: %d", raw);
    }
    else{
        debug_printf("Battery fast charge is OFF: %d", raw);
    }
    return status;
}


/***************************************** SOLAR PANEL FUNCTIONS (read commands 5 - 13) **********************************************************************/

/**
  * @brief Prints the voltage of the X-axis solar panels in volts
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_X_VOLTAGE(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 5; //solar panel x-axis voltage command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_voltage = 0.0024414063f * (float) raw_voltage; //multiply by the conversion coeff
    debug_printf("X-axis Solar Panel voltage: %f V", final_voltage);
    return status;
}

/**
  * @brief Prints the current of the X- facing solar panels in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_X_NEG_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 6; //solar panel x- current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("X- Solar Panel current: %f A", final_current);
    return status;
}

/**
  * @brief Prints the current of the X+ facing solar panels in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_X_POS_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 7; //solar panel x+ current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("X+ Solar Panel current: %f A", final_current);
    return status;
}

/**
  * @brief Prints the voltage of the Y-axis solar panels in volts
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_Y_VOLTAGE(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 8; //solar panel y-axis voltage command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_voltage = 0.0024414063f * (float) raw_voltage; //multiply by the conversion coeff
    debug_printf("Y-axis Solar Panel voltage: %f V", final_voltage);
    return status;
}

/**
  * @brief Prints the current of the Y- facing solar panels in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_Y_NEG_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 9; //solar panel y- current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("Y- Solar Panel current: %f A", final_current);
    return status;
}

/**
  * @brief Prints the current of the Y+ facing solar panels in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_Y_POS_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 10; //solar panel y+ current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("Y+ Solar Panel current: %f A", final_current);
    return status;
}

/**
  * @brief Prints the voltage of the Z-axis solar panels in volts
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_Z_VOLTAGE(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 11; //solar panel z-axis voltage command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_voltage = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_voltage = 0.0024414063f * (float) raw_voltage; //multiply by the conversion coeff
    debug_printf("Z-axis Solar Panel voltage: %f V", final_voltage);
    return status;
}

/**
  * @brief Prints the current of the Z- facing solar panels in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_Z_NEG_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 12; //solar panel z- current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("Z- Solar Panel current: %f A", final_current);
    return status;
}

/**
  * @brief Prints the current of the Z+ facing solar panels in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOLAR_Z_POS_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 13; //solar panel z+ current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0006103516f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("Z+ Solar Panel current: %f A", final_current);
    return status;
}


/***************************************** VOLTAGE BUS CURRENT FUNCTIONS (read commands 14 - 15) **********************************************************************/

/**
  * @brief Prints the current of the EPS 3.3v bus in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_3V_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 14; //3.3v bus current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0020345052f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("3.3v BUS current: %f A", final_current);
    return status;
}

/**
  * @brief Prints the current of the EPS 5v bus in Amps
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_5V_CURRENT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 15; //5v bus current command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_current = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_current = 0.0020345052f * (float) raw_current; //multiply by the conversion coeff
    debug_printf("5v BUS current: %f A", final_current);
    return status;
}


/***************************************** LUP FUNCTIONS (read commands 16 - 17) **********************************************************************/

/**
  * @brief Prints the status of the EPS 3.3v LUP output (on/off)
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_3V_LUP_STATUS(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 16; //3.3v LUP output status command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    if(raw){
        debug_printf("3.3v LUP output is ON: %d", raw);
    }
    else{
        debug_printf("3.3v LUP output is OFF: %d", raw);
    }
    return status;
}

/**
  * @brief Prints the status of the EPS 5v LUP output (on/off)
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_5V_LUP_STATUS(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 17; //5v LUP output status command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    if(raw){
        debug_printf("5v LUP output is ON: %d", raw);
    }
    else{
        debug_printf("5v LUP output is OFF: %d", raw);
    }
    return status;
}


/***************************************** TEMPURATURE FUNCTIONS (read commands 18 - 22 & 37 - 40) **********************************************************************/

/**
  * @brief Prints the temperature of the EPS microcontroller in degrees Celsius. Note: with old
  * firmware and hardware, the returned value is not accurate.
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_MCU_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 18; //MCU temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = (((float) raw_value * 0.0006103516f) - 0.986f) / 0.00355f;
    debug_printf("MCU Temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS battery cell 1
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_1_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 19; //battery cell 1 temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery cell 1 temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS battery cell 2
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_2_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 20; //battery cell 2 temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery cell 2 temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS battery cell 3
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_3_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 21; //battery cell 3 temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery cell 3 temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS battery cell 4
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_4_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 22; //battery cell 4 temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery cell 4 temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS external temp sensor 5
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_5(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 37; //external temperature sensor 5 command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("External sensor 5 temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS external temp sensor 6
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_6(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 38; //external temperature sensor 6 command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("External sensor 6 temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS external temp sensor 7
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_7(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 39; //external temperature sensor 7 command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("External sensor 7 temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the temperature of the EPS external temp sensor 8
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_EXTERN_TEMP_8(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 40; //external temperature sensor 8 command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("External sensor 8 temperature is %f Celsius", final_temp);
    return status;
}


/***************************************** INPUT/OUTPUT CONDITION FUNCTIONS (read commands 23 - 24) **********************************************************************/

/**
  * @brief Prints the raw return value of the Input Condition command in hexadecimal
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_INPUT_CONDITION(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 23; //input conditions command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Input conditions (flags): 0x%X", raw_value);
    return status;
}

/**
  * @brief Returns the flags of the Input Condition command to the given pointer
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @param flags: A pointer to the location you want to store the output
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_INPUT_FLAGS(I2C_HandleTypeDef handle, uint16_t* flags){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 23; //input conditions command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*flags) = raw_value;
    return status;
}

/**
  * @brief Returns whether or not the EPS 5v LUP output is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_LUP_5v_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_INPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 0); //Get the bit 0 mask. Returns 1 if high, 0 if else
}

/**
  * @brief Returns whether or not the EPS 3.3v LUP output is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_LUP_3v_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_INPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 1); //get the bit 1 mask
}

/**
  * @brief Returns whether or not the EPS 3.3v power is good
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_3v_Power_Good(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_INPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 2); //get the bit 2 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS 5v power is good
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_5v_Power_Good(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_INPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 3); //bit 3 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS self-lock is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_SelfLock_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_INPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }
    //TODO: may need to change this to be inverted
    return getBitMask_16b(flags, 4); //bit 4 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS battery is done charging
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Charge_Complete(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_INPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 5); //bit 5 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS is currently charging
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Charging(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_INPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 6); //bit 6 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Prints the raw return value of the Output Condition command in hexadecimal
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_OUTPUT_CONDITION(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 24; //output conditions command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Output conditions (flags): 0x%X", raw_value);
    return status;
}

/**
  * @brief Returns the flags of the Output Condition command to the given pointer
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @param flags: A pointer to the location you want to store the output
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef READ_EPS_OUTPUT_FLAGS(I2C_HandleTypeDef handle, uint16_t* flags){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 24; //output conditions command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    (*flags) = raw_value;
    return status;
}

/**
  * @brief Returns whether or not the EPS is currently self-locked
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Self_Locked(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 0); //bit 0 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS battery bus is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Battery_Bus_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 1); //bit 1 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS BCR bus is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_BCR_Bus_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 2); //bit 2 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS 3.3v bus is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_3v_Bus_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 3); //bit 3 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS 5v bus is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_5v_Bus_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 4); //bit 4 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS is currently fast-charging
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Fast_Charge(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 5); //bit 5 mask. Returns 1 if high, else returns 0
}

/**
  * @brief Returns whether or not the EPS charger is shutdown
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Charger_Shutdown(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 6); //bit mask 6. High level means it is OFF.
}

/**
  * @brief Returns whether or not the EPS Output 1 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Out1_En(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 7); //bit mask 7.
}

/**
  * @brief Returns whether or not the EPS Output 2 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Out2_En(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 8); //bit mask 8.
}

/**
  * @brief Returns whether or not the EPS Output 3 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Out3_En(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 9); //bit mask 9.
}

/**
  * @brief Returns whether or not the EPS Output 4 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Out4_En(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 10); //bit mask 10.
}

/**
  * @brief Returns whether or not the EPS Output 5 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Out5_En(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 11); //bit mask 11.
}

/**
  * @brief Returns whether or not the EPS Output 6 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Out6_En(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 12); //bit mask 12.
}

/**
  * @brief Returns whether or not the EPS battery heater 1 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Batt_Heater_1_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 13); //bit mask 13.
}

/**
  * @brief Returns whether or not the EPS battery heater 2 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Batt_Heater_2_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 14); //bit mask 14.
}

/**
  * @brief Returns whether or not the EPS battery heater 3 is enabled
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns -1 if the read failed, 0 if the the flag is LOW, 1 if the flag is HIGH
  */
int8_t Is_EPS_Batt_Heater_3_On(I2C_HandleTypeDef handle){
    uint16_t flags;
    HAL_StatusTypeDef status = READ_EPS_OUTPUT_FLAGS(handle, &flags);
    if(status != HAL_OK){
        return -1;
    }

    return getBitMask_16b(flags, 15); //bit mask 15.
}

/***************************************** COUNTER FUNCTIONS (read commands 25 - 28 & 48) **********************************************************************/

/**
  * @brief Prints the current count of EPS power cycles
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_POWER_ON_CYCLES(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 25; //power on cycles count command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Number of Power ON cycles: %d", raw_value);
    return status;
}

/**
  * @brief Prints the current count of EPS under-voltage occurances
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_UNDER_VOLT_COUNT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 26; //under voltage count command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Count of 'Under Voltage' conditions: %d", raw_value);
    return status;
}

/**
  * @brief Prints the current count of EPS short-circuit occurances
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SHORT_CIRCUIT_COUNT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 27; //short circuit count command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Count of 'Short Circuit' conditions: %d", raw_value);
    return status;
}

/**
  * @brief Prints the current count of EPS over-temperature occurances
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_OVER_TEMP_COUNT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 28; //over temperature count command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Count of 'Over Temperature' conditions: %d", raw_value);
    return status;
}

/**
  * @brief Prints the current count of EPS battery charge cycles
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_CHARGE_CYCLES(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 48; //charging cycles count command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Number of charging cycles: %d", raw_value);
    return status;
}


/***************************************** TEMPERATURE OUTLIER FUNCTIONS (read commands 29 - 36) **********************************************************************/

/**
  * @brief Prints the maximum EPS battery cell 1 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_1_MAX_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 29; //battery cell 1 maximum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 1, sensor 1 maximum temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the minimum EPS battery cell 1 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_1_MIN_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 33; //battery cell 1 minimum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 1, sensor 1 minimum temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the maximum EPS battery cell 2 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_2_MAX_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 30; //battery cell 2 maximum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 1, sensor 2 maximum temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the minimum EPS battery cell 2 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_2_MIN_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 34; //battery cell 2 minimum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 1, sensor 2 minimum temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the maximum EPS battery cell 3 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_3_MAX_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 31; //battery cell 3 maximum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 2, sensor 1 maximum temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the minimum EPS battery cell 3 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_3_MIN_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 35; //battery cell 3 minimum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 2, sensor 1 minimum temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the maximum EPS battery cell 4 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_4_MAX_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 32; //battery cell 4 maximum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 2, sensor 2 maximum temperature is %f Celsius", final_temp);
    return status;
}

/**
  * @brief Prints the minimum EPS battery cell 4 temperature
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_BATT_4_MIN_TEMP(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 36; //battery cell 4 minimum temperature command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    float final_temp = 0.00390625f * (float) raw_value;
    debug_printf("Battery pack 2, sensor 2 minimum temperature is %f Celsius", final_temp);
    return status;
}


/***************************************** SOFTWARE FUNCTION (read command 41) **********************************************************************/

/**
  * @brief Prints the current EPS microcontroller software/firmware verion
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_SOFTWARE_VERSION(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 41; //software version command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t version = ((uint16_t) data[0] << 8) + (uint16_t) data[1];
    debug_printf("Software Version: 0x%X", version);
    return status;
}


/***************************************** OUTPUT DEFAULT FUNCTIONS (read commands 43 - 47) **********************************************************************/

/**
  * @brief Prints the default/startup state of the EPS Output 1 line 
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_OUT_1_DEFAULT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 43; //output 1 default value command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Output 1 default value: %d", raw_value);
    return status;
}

/**
  * @brief Prints the default/startup state of the EPS Output 3 line 
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_OUT_3_DEFAULT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 44; //output 3 default value command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Output 3 default value: %d", raw_value);
    return status;
}

/**
  * @brief Prints the default/startup state of the EPS Output 4 line 
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_OUT_4_DEFAULT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 45; //output 4 default value command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Output 4 default value: %d", raw_value);
    return status;
}

/**
  * @brief Prints the default/startup state of the EPS Output 5 line 
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_OUT_5_DEFAULT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 46; //output 5 default value command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Output 5 default value: %d", raw_value);
    return status;
}

/**
  * @brief Prints the default/startup state of the EPS Output 6 line 
  * @param handle: The I2C handle you want to attempt to talk to the EPS upon
  * @retval Returns if all of the hal function calls succeeded, or if any of them failed
  */
HAL_StatusTypeDef PRINT_EPS_OUT_6_DEFAULT(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = HAL_ERROR; //status initial state is error
    uint8_t command = 47; //output 6 default value command
    uint8_t data[2];
    status = EPS_READ(handle, command, data);
    if(status != HAL_OK){
        return status;
    }
    uint16_t raw_value = ((uint16_t) data[0] << 8) + (uint16_t) data[1]; //convert 2 separate bytes to unsigned 16 bits
    debug_printf("Output 6 default value: %d", raw_value);
    return status;
}

/********************************************** WRITE FUNCTIONS **********************************************************************************/
/*** For some really dumb reason, the documentation is indexed starting at 1, but the commands really start at 0 *********************************/
/*** This is reflected by using the correct command number as according to the documentation, but subtracting 1  *********************************/
/*** NOTE: THIS WILL BE REFLECTED IN THE NEWEST DOCUMENTATION THAT MATCHES OUR FIRMWARE                          *********************************/

int8_t enable_EPS_SW_Self_Lock(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 1 - 1, 1); //turn command 1 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_SW_Self_Lock(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 1 - 1, 0); //turn command 1 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Vbatt_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 2 - 1, 1); //turn command 2 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Vbatt_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 2 - 1, 0); //turn command 2 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_BCR_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 3 - 1, 1); //turn command 3 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_BCR_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 3 - 1, 0); //turn command 3 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_3v_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 4 - 1, 1); //turn command 4 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_3v_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 4 - 1, 0); //turn command 4 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_5v_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 5 - 1, 1); //turn command 5 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_5v_Bus(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 5 - 1, 0); //turn command 5 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_LUP_5v(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 6 - 1, 0); //turn command 6 to the ON state (high level ON Output1 and OFF LUP5v)
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_LUP_5v(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 6 - 1, 1); //turn command 6 to the OFF state (high level ON Output1 and OFF LUP5v)
    if(status == HAL_OK)
        return 0;
    return status;
}

/******************************* COMMAND 7 IS NOT USED *****************************************/

int8_t enable_EPS_LUP_3v(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 8 - 1, 0); //turn command 8 to the ON state (high level ON Output6 and OFF LUP3.3v)
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_LUP_3v(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 8 - 1, 1); //turn command 8 to the OFF state (high level ON Output6 and OFF LUP3.3v)
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Fast_Charge(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 9 - 1, 1); //turn command 9 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Fast_Charge(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 9 - 1, 0); //turn command 9 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Charge(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 10 - 1, 0); //turn command 10 to the ON state (HIGH = OFF)
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Charge(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 10 - 1, 1); //turn command 10 to the OFF state (HIGH = OFF)
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Output_1(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 11 - 1, 1); //turn command 11 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Output_1(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 11 - 1, 0); //turn command 11 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Output_2(I2C_HandleTypeDef handle){
    return disable_EPS_LUP_5v(handle);
}

int8_t disable_EPS_Output_2(I2C_HandleTypeDef handle){
    return enable_EPS_LUP_5v(handle);
}

int8_t enable_EPS_Output_3(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 13 - 1, 1); //turn command 13 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Output_3(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 13 - 1, 0); //turn command 13 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Output_4(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 14 - 1, 1); //turn command 14 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Output_4(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 14 - 1, 0); //turn command 14 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Output_5(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 15 - 1, 1); //turn command 15 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Output_5(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 15 - 1, 0); //turn command 15 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Output_6(I2C_HandleTypeDef handle){
    return disable_EPS_LUP_3v(handle);
}

int8_t disable_EPS_Output_6(I2C_HandleTypeDef handle){
    return enable_EPS_LUP_3v(handle);
}

int8_t enable_EPS_Batt_Heater_3(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 17 - 1, 1); //turn command 17 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Batt_Heater_3(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 17 - 1, 0); //turn command 17 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Batt_Heater_1(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 18 - 1, 1); //turn command 18 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Batt_Heater_1(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 18 - 1, 0); //turn command 18 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t enable_EPS_Batt_Heater_2(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 19 - 1, 1); //turn command 19 to the ON state
    if(status == HAL_OK)
        return 0;
    return status;
}

int8_t disable_EPS_Batt_Heater_2(I2C_HandleTypeDef handle){
    HAL_StatusTypeDef status = EPS_WRITE(handle, 19 - 1, 0); //turn command 19 to the OFF state
    if(status == HAL_OK)
        return 0;
    return status;
}

/********************************************** HELPER FUNCTIONS **********************************************************************************/

HAL_StatusTypeDef EPS_READ(I2C_HandleTypeDef handle, uint8_t command, uint8_t* data_ptr){
        HAL_StatusTypeDef status = HAL_ERROR;
        status = HAL_I2C_Master_Transmit(&handle, (uint16_t) (EPS_I2C_ADDRESS << 0x1), &command, 1, EPS_I2C_TIMEOUT);
        if(status != HAL_OK){
            return status;
        }
        status = HAL_I2C_Master_Receive(&handle, (uint16_t) (EPS_I2C_ADDRESS << 0x1), data_ptr, 2, EPS_I2C_TIMEOUT);
        return status;
}

/**
  * @brief Sends a write command to the EPS module over a given I2C instance
  * @param handle: the i2c handle upon which to communicate
  * @param command: the write command number to perform
  * @param state: the state to set the output specified by the command to (0 or 1)
  * @retval Returns if the hal function call succeeded, or if it failed
  */
HAL_StatusTypeDef EPS_WRITE(I2C_HandleTypeDef handle, uint8_t command, uint8_t state){
        HAL_StatusTypeDef status = HAL_ERROR;
        uint8_t data[2];
        data[0] = command;
        data[1] = state;
        status = HAL_I2C_Master_Transmit(&handle, (uint16_t) (EPS_I2C_ADDRESS << 0x1), data, 2, EPS_I2C_TIMEOUT);
        return status;
}

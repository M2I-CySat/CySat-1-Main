/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file DAT_Inputs.c
* @brief Input data (data from sensors)
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @author            Kolio
* @version           1.0.0
* @date              2018.07.04
*
* @copyright         (C) Copyright Endurosat
*
*                    Contents and presentations are protected world-wide.
*                    Any kind of using, copying etc. is prohibited without prior permission.
*                    All rights - incl. industrial property rights - are reserved.
*
* @history
* @revision{         1.0.0  , 2018.07.04, author Kolio, Initial revision }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "DAT_Inputs.h"
#include "sensor.h"

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal defines */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL TYPES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal types definition */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No External variables definition */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) VARIABLES DEFINITION/DECLARATION 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal variables definition/declaration */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DECLARATION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal routines declaration */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL (NONE STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/**
 * @brief     
 * @param[in] FilterSelect - boolean: 1 - on/ 0 - off
 * @return    none
 *
 * This method enable or disable filtering of the sensor inputs
 */
void Filter_Enable (uint8_t FilterSelect) {
}

/**
 * @brief     Filter
 * @param[in] FilterSelect - boolean: 1 - on/ 0 - off
 * @return    none
 *
 * This method enable or disable filtering of the sensor inputs
 */
ES_ReturnType Filter (uint16_t Depth, uint16_t *RawValue, uint16_t *Result) {
  ES_ReturnType ESRetValue = E_OK;
  
  if (NULL_PTR == RawValue) {
    ESRetValue = E_INV_ARG;
  }
  return ESRetValue;
}

/**
 * @brief     Get magnitometer filtered value
 * @param[in] B_measured - raw value form the sensor, B_filtered_old - previous filtered value
 * @param[out] B_filtered
 * @return    status in ES_ReturnType
 *
 * Standart low pass filter is used to eliminate noise as recommended in Design of Attitude Control System .. page 30, ch 2.6.1
 */
Compass_Axis_t Magnitometer_Filtered (Compass_Axis_t *B_measured, Compass_Axis_t *B_filtered_old)  {
  double const a1 = 0.8;
  double const b1 = 1 - a1; 
  Compass_Axis_t B_filtered;
  //int     i;  
  
  //for     (i = 0; i < NO_OF_MTORQ_IDS; i++) 
  //{
    B_filtered.AXIS_X =  a1 * B_filtered_old->AXIS_X + b1 * B_measured->AXIS_X;
    B_filtered.AXIS_Y =  a1 * B_filtered_old->AXIS_Y + b1 * B_measured->AXIS_Y;
    B_filtered.AXIS_Z =  a1 * B_filtered_old->AXIS_Z + b1 * B_measured->AXIS_Z;    
  //}
  
  return B_filtered;
}

ES_ReturnType Magnetorquer_Value_Convert (MagnetometersType mtorq, uint16_t *RawValue, uint16_t *Result) {
  ES_ReturnType ESRetValue = E_OK;
  
  if (NULL_PTR == RawValue) {
    ESRetValue = E_INV_ARG;
  }  
  
  return ESRetValue;
}

/**
 * @brief     Magnetometer init
 * @param[in] 
 * @param[out] 
 * @return    
 *
 * 
 */
ES_ReturnType Magnitometers_Init (Compass_mode_t CompassMode,
                                  Compass_Measuring_mode_t CompassMeasuringMode,
                                  Compass_ID_Def_t CompassIDDef,
                                  Compass_Set_Range_t CompassSetRange,
                                  Compass_Set_Bandwidth_t CompassSetBandwidth,
                                  Compass_Samples_Averaged_t Compass_Samples_Averaged  
                                  ) {
                                    
    ES_ReturnType ESRetValue = E_OK;
    
    
    
  if (!HMC5883L_SetMode(type_mode_table[CompassMode]))
    ESRetValue = E_INV_CFG;     // Set the driver default mode    
    
    
  if (HMC5883L_SetRange(range_table [CompassSetRange].reserved_val)  == SEN_ERROR) 
      ESRetValue = E_INV_CFG;     // Set the driver default ext value range in tesla
  
  
  if (HMC5883L_SetRegA (samples_aver_table[Compass_Samples_Averaged] | band_table[CompassSetBandwidth] | meas_mode_table[CompassMeasuringMode]) == SEN_ERROR)
          ESRetValue = E_INV_CFG;     // Set configuratrion
  
 
    return ESRetValue;
}


ES_ReturnType Magnitometers_LIS3MDL_Init(uint8_t Dev_No) {
    uint8_t val;  
    ES_ReturnType ESRetValue = E_OK; 

    
    //set ODR (turn ON device)
  
 // turn on device in continues measuring 
  if ( !(LIS3MDL_MAG_W_SystemOperatingMode (Dev_No, LIS3MDL_MAG_MD_CONTINUOUS)))
    return E_INV_CFG;    
 // update output value 10 times per sec - prity enough for our detumbling algo 
  if ( !(LIS3MDL_MAG_W_OutputDataRate (Dev_No, LIS3MDL_MAG_DO_10Hz)))
    return E_INV_CFG;
 // check status   
  if ( !(LIS3MDL_MAG_R_WHO_AM_I_(Dev_No, &val )))
    return E_INV_CFG;
  
  if (val ^ 0x3D)     // invalid configuratuion if device magnetometer answered with id differ than 0x3D
    return E_INV_CFG;
  
  // Here range of the magnetometer is configured, user set to different - check LIS3MDL_MAG_FS_t enumerator!
  if ( !(LIS3MDL_MAG_W_FullScale(Dev_No, LIS3MDL_MAG_FS_16Ga )))
    return E_INV_CFG;    
  
  return ESRetValue;
}

ES_ReturnType Magnitometers_LIS3MDL_Read_Data (Compass_Axis_t *MagOutData, uint8_t Dev_No) {
  
  int16_t pDataRaw[3];
  double sensitivity = 1; 

  ES_ReturnType ESRetValue = E_OK;   
  
  uint8_t regValue[6] = {0, 0, 0, 0, 0, 0};
  
  if ( !(LIS3MDL_MAG_Get_Magnetic(Dev_No, (uint8_t*)regValue )))
   return E_INV_CFG;   

  /* Format the data. */
  pDataRaw[0] = ( ( ( ( int16_t )regValue[1] ) << 8 ) + ( int16_t )regValue[0] );
  pDataRaw[1] = ( ( ( ( int16_t )regValue[3] ) << 8 ) + ( int16_t )regValue[2] );
  pDataRaw[2] = ( ( ( ( int16_t )regValue[5] ) << 8 ) + ( int16_t )regValue[4] );  
  
  MagOutData->AXIS_X = ( (double)pDataRaw[0] * sensitivity );
  MagOutData->AXIS_Y = ( (double)pDataRaw[1] * sensitivity );
  MagOutData->AXIS_Z = ( (double)pDataRaw[2] * sensitivity );
  
  return ESRetValue;    
  
}

/**
 * @brief     Get magnitometer data and convert to Tesla
 * @param[in] none
 * @param[out] MagOutData - pointer to data array x,y,x double in Tesla
 * @return    status in ES_ReturnType
 *
 * 
 */
ES_ReturnType Magnitometers_Read_Data (Compass_Axis_t *MagOutData, Compass_Set_Range_t CompassSetRange){
  ES_ReturnType ESRetValue = E_OK;

   // AxesRaw_t mag_data = {{1011},{64555},{63999}};
                    
      AxesRaw_t mag_data;
  
  if  (NULL_PTR == MagOutData) {  
    ESRetValue = E_INV_ARG;
  }  
  if (HMC5883L_GetAccAxesRaw(&mag_data) == SEN_SUCCESS)
  {
      MagOutData->AXIS_X = ((double) (mag_data.AXIS_X)) * range_table [CompassSetRange].range_units / 2048;
      MagOutData->AXIS_Y = ((double) (mag_data.AXIS_Y)) * range_table [CompassSetRange].range_units / 2048;      
      MagOutData->AXIS_Z = ((double) (mag_data.AXIS_Z)) * range_table [CompassSetRange].range_units / 2048;         
  }
  else  ESRetValue = E_INV_CFG;
      
  return ESRetValue;  
}


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal routines definition */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

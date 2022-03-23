/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file AppTasks.c
* @brief Implementation for a task about common purposes
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @author            Vasil Milev
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
* @revision{         1.0.0  , 2018.07.04, author Vasil Milev, Initial revision }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "AppTasks.h"
#include "fatfs.h"
#include "ESTTC.h"
#include "User_types.h"
#include "LIS3MDL_MAG_driver.h"
#include "DAT_Inputs.h"
#include "Panels.h"
#include "version.h"

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

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
uint8_t save_log_timer = 0;
uint16_t save_log_counter = 0;
FIL tFil_Temp_Lum;
/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) VARIABLES DEFINITION/DECLARATION 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DECLARATION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL (NONE STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void StartDefaultTask(void const * argument)
{

  //----------------------------sensors init start
      #ifdef HMC5883
          Magnitometers_Init (E_SINGLE,
                            E_NORMAL,
                            E_COMBINED,
                            E_810uT,
                            E_30HZ,
                            E_MEAS_AVG_8);
      #else
          if (Magnitometers_LIS3MDL_Init(LIS3MDL_MAG_I2C_ADDRESS_LOW) == E_OK)
              fprintf(SYSCON,"MAG1_INIT_OK\r");
          else
              fprintf(SYSCON, "  MAG1 fail\r");
          if (Magnitometers_LIS3MDL_Init(LIS3MDL_MAG_I2C_ADDRESS_HIGH) == E_OK)
              fprintf(SYSCON,"MAG2_INIT_OK\r");
          else
              fprintf(SYSCON, "  MAG2 fail\r");
      #endif

      //Inizialize ACC Sensor 1
      if (AIS328DQ_Init(AIS328DQ_1_MEMS_I2C_ADDRESS) == SEN_SUCCESS)
        fprintf(SYSCON,"ACC1_INIT_OK\r");
      else
        I2C2_Reset();

      //Inizialize ACC Sensor 2
      if (AIS328DQ_Init(AIS328DQ_2_MEMS_I2C_ADDRESS) == SEN_SUCCESS)
        fprintf(SYSCON,"ACC2_INIT_OK\r");
      else
        I2C2_Reset();


      Panels_Init();

      if (SetMagnetorque(PAN_X_M, 10, 1) == SEN_SUCCESS)
        fprintf(SYSCON,"TRQ%u_SET_10%%\r", TRQ_1);
      if (SetMagnetorque(PAN_Y_M, 50, 1) == SEN_SUCCESS)
        fprintf(SYSCON,"TRQ%u_SET_50%%\r", TRQ_2);
      if (SetMagnetorque(PAN_Z_M, 90, 1) == SEN_SUCCESS)
        fprintf(SYSCON,"TRQ%u_SET_90%%\r", TRQ_3);

      //Inizialize GYR Sensor X
      if (ADIS16265_Init(PAN_X_M) == SEN_SUCCESS)
        fprintf(SYSCON,"GYR%u_INIT_OK\r", GYR_1);
      else
        fprintf(SYSCON,"GYR%u_INIT_FAIL\r", GYR_1);

      //Inizialize GYR Sensor Y
      if (ADIS16265_Init(PAN_Y_M) == SEN_SUCCESS)
        fprintf(SYSCON,"GYR%u_INIT_OK\r", GYR_2);
      else
        fprintf(SYSCON,"GYR%u_INIT_FAIL\r", GYR_2);

      //Inizialize GYR Sensor Z
      if (ADIS16265_Init(PAN_Z_M) == SEN_SUCCESS)
        fprintf(SYSCON,"GYR%u_INIT_OK\r", GYR_3);
      else
        fprintf(SYSCON,"GYR%u_INIT_FAIL\r", GYR_3);

      //----------------------------sensors init end


      for( ; ; )
      {
          GREEN_LED_ON();
          osDelay(50);
          GREEN_LED_OFF();


          osDelay(1000);
      }

}



/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

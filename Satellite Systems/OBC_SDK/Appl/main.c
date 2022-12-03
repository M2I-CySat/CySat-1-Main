/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file main.c
* @brief 
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
#include "main.h"
#include "MCU_Init.h"
#include "fatfs.h"
#include "version.h"
#include "Payload.h"

#include "helper_functions.h"
#include "EPS.h"
#include "UHF.h"
#include "test.h"

#include "AppTasks.h"

#include  <errno.h>
#include  <sys/unistd.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define INITIAL_WAIT (30 * 60 * 1000) // waits 30 minutes
uint8_t data[1];
uint8_t GroundStationRxBuffer[7];
uint32_t GroundStationRxDataLength;

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
Sat_Flags_t SatFlags;
uint32_t calendar_format;
FILE *COMM = COM1;
FILE *PAYLOAD = COM4;
FILE *SYSCON = COM6;

osMutexId EPS_I2C_Mutex;
osMutexId UART_Mutex;
osMutexId Num_I2C_Errors_Mutex;
osMutexId Battery_Capacity_Mutex;
osMutexId ADCS_Active_Mutex;
osMutexId Low_Power_Mode_Mutex;
osMutexId UHF_UART_Mutex;

osThreadId myUHFTxTask;
osThreadId myADCSTask;
osThreadId myMainTask;
osThreadId myUHFRxTask;

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
void I2C2_Reset (void) {
   hi2c2.Instance->CR1 |= I2C_CR1_SWRST;
      osDelay(100);
      hi2c2.Instance->CR1 &= ~I2C_CR1_SWRST;
      HAL_I2C_DeInit(&hi2c2);
      hi2c2.State = HAL_I2C_STATE_RESET;
      MX_I2C2_Init();
}

// This function must be called once before launch to setup the EPS ready for launch
void init_Satelite(void){
    disable_EPS_Vbatt_Bus();
    disable_EPS_BCR_Bus();
    disable_EPS_5v_Bus();
    disable_EPS_LUP_5v();
    disable_EPS_LUP_3v();
    disable_Boost_Board();
    disable_Payload();
    disable_EPS_Output_3();
    disable_UHF();
    disable_EPS_Output_6();
    disable_EPS_Batt_Heater_1();
    disable_EPS_Batt_Heater_2();
    disable_EPS_Batt_Heater_3();
}

/**
 * CySat 1 Mission Execution
 */
int main(void)
{
    //SCB->VTOR = APPL_ADDRESS;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    HAL_Delay(INITIAL_WAIT); // Delay for the specified 30 minutes

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_UART4_Init();
    MX_USART6_UART_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();

    // Commands the start of data reception because I can't define it in AppTasks.c without having to mess around with #including stuff
    //HAL_UART_Receive_IT(&huart6,GroundStationRxBuffer, 4);
    //HAL_UART_Receive_IT(&huart1,GroundStationRxBuffer, 4);

    /* Initialize task threads */
    osThreadDef(myMainTask, Main_Task, osPriorityRealtime, 0, 512);
    osThreadCreate(osThread(myMainTask), NULL);

    osThreadDef(myUHFRxTask, UHF_Rx_Task, osPriorityNormal, 0, 512);
    osThreadCreate(osThread(myUHFRxTask), NULL);

    osThreadDef(myUHFTxTask, UHF_Tx_Task, osPriorityNormal, 0, 512);
    osThreadCreate(osThread(myUHFTxTask), NULL);

    osThreadDef(myADCSTask, ADCS_Task, osPriorityNormal, 0, 1024);
    osThreadCreate(osThread(myADCSTask), NULL);

    osThreadDef(myBatteryCapacityTask, BatteryCapacity_Task, osPriorityNormal, 0, 256);
    osThreadCreate(osThread(myBatteryCapacityTask), NULL);

    /* Start scheduler */
    osKernelStart();

    // Enable Transparent Mode
    // TODO: Send command to UHF transceiver to enable transparent mode

    // Detumbling Sequence
    // TODO: Detumbling functions (ADCS) go here
    debug_printf("Beginning detumbling sequence");

    // Ground station will receive beacon, send "Beacon Shut Off" request
    // TODO: OBC will shut off beacon when it receives ground station command
    // TODO: OBC will confirm shutoff

    /** Ground station will send "Initial Health Check Request" command
    * TODO: Create health checks:
    * EPS, ADCS, SDR, OBC, UHF transceiver
    */

   // HAL_Delay(15000); // Delay for 15 seconds to allow ADCS to boot-up in application mode

    /* Receive via STM UART */
    GroundStationRxDataLength = 4;
    HAL_UART_Receive_IT(&huart6, (uint8_t*) &GroundStationRxBuffer, 4);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: debug_printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
#ifdef DEBUG_ENABLE
  while(1)
  {
      debug_led_green(5, 2000);
      debug_led_amber(5, 2000);
  }
#endif
  /* USER CODE END Error_Handler */
}

void vApplicationMallocFailedHook( void )
{
    Error_Handler();
}


void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    Error_Handler();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart6){ //OBC connected to Payload/USB
        if(handleCySatPacket(parseCySatPacket(GroundStationRxBuffer)) == -1){ //error occurred
            sendErrorPacket();
        }
        HAL_UART_Receive_IT(&huart6,GroundStationRxBuffer, 4);
    }
    if(huart == &huart1){ //I think this is for the UHF transceiver but I'm not sure -Steven
        if(handleCySatPacket(parseCySatPacket(GroundStationRxBuffer)) == -1){ //error occurred
            sendErrorPacket();
        }
        HAL_UART_Receive_IT(&huart1,GroundStationRxBuffer, 4);
    }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
    if(hi2c == &hi2c1){ //OBC connected to EPS, UHF, and ADCS
        debug_printf("Got I2C Connection!");
        HAL_I2C_Master_Receive_IT(&hi2c1, 0x18 << 1, data, 1);
    }
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

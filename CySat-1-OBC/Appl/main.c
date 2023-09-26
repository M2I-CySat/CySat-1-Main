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
#define DEBUG_WAIT (1 * 2 * 1000) // waits 2 seconds
uint8_t GroundStationPacketLength = 255;



uint8_t* start_of_rx_buffer;
uint8_t GroundStationRxBuffer[256];
uint8_t* start_of_rx_buffer = &GroundStationRxBuffer;

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

uint8_t GroundStationRxBuffer[256];

uint32_t calendar_format;

FILE *COMM = COM1;
FILE *PAYLOAD = COM4;
FILE *SYSCON = COM6;

/* Mutexes */
osMutexId EPS_I2C_Mutex;
osMutexId UART_Mutex;
osMutexId Num_I2C_Errors_Mutex;
osMutexId Battery_Capacity_Mutex;
osMutexId ADCS_Active_Mutex;
osMutexId Low_Power_Mode_Mutex;
osMutexId UHF_UART_Mutex;

/* Threads */
osThreadId myUHFTxRxTask;
osThreadId myADCSTask;
osThreadId myMainTask;

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

/**
 * Reset I2C State
 */
void I2C2_Reset(void) {
    hi2c2.Instance->CR1 |= I2C_CR1_SWRST;
    osDelay(100);
    hi2c2.Instance->CR1 &= ~I2C_CR1_SWRST;
    HAL_I2C_DeInit(&hi2c2);
    hi2c2.State = HAL_I2C_STATE_RESET;
    MX_I2C2_Init();
}

//

/**
 * Initialize the satellite before launch
 * This function must be called once before launch to setup the EPS ready for launch
 */
void shutdown_EPS(void) {
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

HAL_StatusTypeDef startup_EPS() {
	debug_printf("Inside EPS Startup");
	HAL_StatusTypeDef HalStatus = HAL_OK;
	uint8_t status[10];
	status[0] = enable_EPS_Vbatt_Bus();
	status[1] = enable_EPS_BCR_Bus();
	status[2] = enable_EPS_5v_Bus();
	status[3] = enable_EPS_LUP_5v();
	status[4] = enable_EPS_LUP_3v();
	status[5] = enable_Boost_Board();
	status[6] = enable_UHF();
	status[7] = enable_EPS_Batt_Heater_1();
	status[8] = enable_EPS_Batt_Heater_2();
	status[9] = enable_EPS_Batt_Heater_3();
	debug_printf("EPS STATUS: ");
	for (int i = 0; i<10; i++){
		debug_printf_no_newline("%d",status[i]);
		if (status[i] != HAL_OK){
			HalStatus = HAL_ERROR;
			debug_printf("Startup EPS Error %d",i);
		}
	}
	return HalStatus;
}

/**
 * CySat 1 Mission Execution
 */
int main(void) {
    //SCB->VTOR = APPL_ADDRESS;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* TODO: Uncomment before launch: Delay for the specified 30 minutes required by NASA */
	HAL_Delay(DEBUG_WAIT);
    // HAL_Delay(INITIAL_WAIT);

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_UART4_Init();
    MX_USART6_UART_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();

    /* Awake message */
	debug_printf("This is Cy-Sat 1 from Iowa State University\nBEEP BEEP BOOP BOOP Systems Starting!\nWait period starting");

	/* Initialize Random Number Generator */
	srand(291843);

    /*
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    * TRHEADS INITIALIZATION - Tasks specified in AppTasks.c
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */
    osThreadDef(myMainTask, Main_Task, osPriorityRealtime, 0, 12288); // System initialization
    osThreadCreate(osThread(myMainTask), NULL);

    osThreadDef(myUHFTxRxTask, UHF_TxRx_Task, osPriorityNormal, 0, 512); // UHF Receive/Transmit
    osThreadCreate(osThread(myUHFTxRxTask), NULL);

    osThreadDef(myADCSTask, ADCS_Task, osPriorityNormal, 0, 1024); // ADCS
    osThreadCreate(osThread(myADCSTask), NULL);

    osThreadDef(myBatteryCapacityTask, BatteryCapacity_Task, osPriorityNormal, 0, 256); // Batteries
    // TODO: Uncomment osThreadCreate(osThread(myBatteryCapacityTask), NULL);

    /*
     *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * CALLBACKS INITIALIZATION - Callbacks from STM Drivers
     *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    /* FINAL TASK: Start scheduler */
    osKernelStart();
}

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/**
 * On receive of UART data from specified UART module. No longer handles the information as I can't get it to work.
 * Called when a packet is received from a UART device
 * @param huart - the specified UART module
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	// Amber LED is being used to indicate packet handling, green is being used to indicate activity in the main loop
	// Due to a long series of issues, only reception is handled by the callback. Handling is handled in main loop.
	// Issues include delay functions not working inside the callback and UHF returning junk upon transparent mode enable, but only in the callback
    AMBER_LED_ON();
    if (huart == &huart1) {
    	debug_printf("Packet received on UART 1 (UHF)");
    	//HAL_UART_Receive_IT(&huart1, start_of_rx_buffer, GroundStationPacketLength);
    }else if (huart == &huart6) {
    	debug_printf("Packet received on UART 6 (Payload)");
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart1) {
		debug_printf("UART error. Data: %s", GroundStationRxBuffer);
		//HAL_UART_Receive_IT(&huart1, start_of_rx_buffer, GroundStationPacketLength);
	}
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
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

/**
 *
 * @param hi2c
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	uint8_t data[1];

    if (hi2c == &hi2c1) { //OBC connected to EPS, UHF, and ADCS
        debug_printf("I2C connection established!");
        HAL_I2C_Master_Receive_IT(&hi2c1, 0x18 << 1, data, 1);
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
	debug_printf("Error Handler triggered");
#ifdef DEBUG_ENABLE
    while(1)
    {
        debug_led_green(5, 2000);
        debug_led_amber(5, 2000);
    }
#endif
    /* USER CODE END Error_Handler */
}

/**
 * Error: x
 */
void vApplicationMallocFailedHook(void) {
    Error_Handler();
}

/**
 * Error: x
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    Error_Handler();
}

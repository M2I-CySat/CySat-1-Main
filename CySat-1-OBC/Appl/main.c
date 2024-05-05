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



// RAM-Sat branch of CySat OBC code
// Steven Scheuermann's attempt at making the satellite only use RAM instead of the SD Card
// Becuase Steven was a fool and let a loose 5V wire near the OBC destroying its capability to read SD Cards
// Don't be like Steven

// Flight RBFs:
// * Uncomment 30 minute delay                        - DONE
// * Uncomment antenna deployment code                - DONE
// * Uncomment magnetometer deployment code           - DONE
// * Uncomment ADCS tasks code                        - DONE
// * Revert testing duration for restart task         - DONE
// * Uncomment Backup antenna deployment function     - DONE
// * Revert testing duration for voltage watchdog     - N/A, Watchdog disabled as it causes problems
// * Comment out debug_printf stuff                   - DONE



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
#include  <string.h>

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
//#define INITIAL_WAIT (30 * 60 * 1000) // waits 30 minutes
//#define DEBUG_WAIT (1 * 1 * 1000) // waits 1 second
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
uint8_t METaddress[256];
uint8_t DATaddress[10800];
uint8_t KELaddress[1080];
uint8_t TESaddress[65536];
uint8_t HCKaddress[2048];

uint32_t DATlength;
uint32_t KELlength;
uint32_t METlength;
uint32_t TESlength;
uint32_t HCKlength;

uint16_t HCKposition;
uint16_t METposition;

uint8_t sat_charged;
uint8_t ADCS_poll;
uint8_t MAIN_poll;

int MESnum;
uint8_t TESnum;




uint32_t calendar_format;

FILE *COMM = COM1;
FILE *PAYLOAD = COM4;
FILE *SYSCON = COM6;

/* Mutexes */
//osMutexId EPS_I2C_Mutex;
//osMutexId UART_Mutex;
//osMutexId Num_I2C_Errors_Mutex;
//osMutexId Battery_Capacity_Mutex;
//osMutexId ADCS_Active_Mutex;
//osMutexId Low_Power_Mode_Mutex;
//osMutexId UHF_UART_Mutex;

/* Threads */
osThreadId myRestartTask;
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
    disable_LNAs();
    disable_EPS_Batt_Heater_1();
    disable_EPS_Batt_Heater_2();
    disable_EPS_Batt_Heater_3();
}

HAL_StatusTypeDef startup_EPS() {
	debug_printf("Inside EPS Startup");
	HAL_StatusTypeDef HalStatus = HAL_OK;
	uint8_t status[13];
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
	status[10] = disable_Payload();
	status[11] = disable_LNAs();
	status[12] = enable_EPS_3v3_Bus();
	debug_printf("EPS STATUS: ");
	for (int i = 0; i<12; i++){
		debug_printf_no_newline("%d",status[i]);
		if (status[i] != HAL_OK){
			HalStatus = HAL_ERROR;
			debug_printf("Startup EPS Error %d",i);
		}
	}
	return HalStatus;
}

void recover_SDR() {
	disable_Payload();
	disable_LNAs();
	disable_EPS_Output_3();
}

HAL_StatusTypeDef HCKappend(char* content){
    memcpy(&HCKaddress[HCKposition],&content[0],strlen(content));
    HCKposition+=strlen(content);
    HCKlength = HCKposition;
    return HAL_OK;
}

HAL_StatusTypeDef METappend(char* content){
    memcpy(&METaddress[METposition],&content[0],strlen(content));
    METposition+=strlen(content);
    METlength = METposition;
    return HAL_OK;
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


    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_UART4_Init();
    MX_USART6_UART_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();

    /* Awake message */
	//debug_printf("This is Cy-Sat 1 from Iowa State University\n\rBEEP BEEP BOOP BOOP Systems Starting!\n\rWait period starting");
	/* Initialize Random Number Generator */
	srand(291843);
	//debug_printf("Random Number Generator Initialized");

	//startupfile = f_open(&fil, "STARTUP.TXT", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS)


//    /* TODO: Uncomment before launch: Delay for the specified 30 minutes required by NASA */
//	HAL_Delay(DEBUG_WAIT);
//	//osDelay(DEBUG_WAIT);
//    // HAL_Delay(INITIAL_WAIT);
//	debug_printf("Post wait");
    /*
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    * TRHEADS INITIALIZATION - Tasks specified in AppTasks.c
    *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    */
    osThreadDef(myMainTask, Main_Task, osPriorityHigh, 0, 11000); // System initialization
    osThreadCreate(osThread(myMainTask), NULL);

    osThreadDef(myRestartTask, Restart_Task, osPriorityRealtime, 0, 512); // Automatic Restart On Freeze
    osThreadCreate(osThread(myRestartTask), NULL);

    osThreadDef(myADCSTask, ADCS_Task, osPriorityNormal, 0, 1024); // ADCS
    osThreadCreate(osThread(myADCSTask), NULL);
    // Battery capacity task has been commented out for flight due to its tendency to freeze the satellite
    //osThreadDef(myBatteryCapacityTask, BatteryCapacity_Task, osPriorityNormal, 0, 256); // Batteries
    //osThreadCreate(osThread(myBatteryCapacityTask), NULL);

    /*
     *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     * CALLBACKS INITIALIZATION - Callbacks from STM Drivers
     *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
    /* FINAL TASK: Start scheduler */
    //debug_printf("Starting AppTasks.c");
    osKernelStart();
	shutdown_EPS();
	NVIC_SystemReset();
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
    //AMBER_LED_ON();
    if (huart == &huart1) {
    	//debug_printf("Packet received on UART 1 (UHF)");
    	//HAL_UART_Receive_IT(&huart1, start_of_rx_buffer, GroundStationPacketLength);
    }else if (huart == &huart6) {
    	//debug_printf("Packet received on UART 6 (Payload)");
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	if (huart == &huart1) {
		//debug_printf("UART error. Data: %s", GroundStationRxBuffer);
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
        //debug_printf("I2C connection established!");
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
	//debug_printf("Error Handler triggered");
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

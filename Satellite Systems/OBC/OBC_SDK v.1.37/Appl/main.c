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
#include "ESTTC.h"
#include "AppTasks.h"

#include "helper_functions.h"
#include "EPS.h"
#include "UHF.h"
#include "CySat_I_Settings.h"
#include "BatteryCapacity.h"

#include  <errno.h>
#include  <sys/unistd.h>
#include  <stdarg.h>


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define INITIAL_WAIT 1*60*1000 //waits 1 minute

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


int main(void)
{
    //  SCB->VTOR = APPL_ADDRESS;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_FMC_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_I2C3_Init();
    MX_SDIO_SD_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_SPI6_Init();
    MX_TIM5_Init();
    MX_UART4_Init();
    MX_USART1_UART_Init();
    MX_USART6_UART_Init();
    MX_RTC_Init(); //TODO: look into this
    MX_FATFS_Init();
  
    SatFlags.Detumbling = 0;
    SatFlags.Auto_Control = 0;
    SatFlags.Infinity_Detumbling = 0;
    SatFlags.AD_is_Running = 0;
    SatFlags.Antenna_is_Released = 0;
    SatFlags.Battery_Low = 0;
    SatFlags.MelodyEnabled = 0;
    SatFlags.CameraReady = 0;
    SatFlags.MakePicture = 0;
    SatFlags.Gyros_ON_OFF = 0;
    SatFlags.BeaconEnabled = 0;

    /* Waits for the required initial inactivity time period */
    HAL_Delay(INITIAL_WAIT);
    AMBER_LED_OFF();

    //TODO: deploy boom magnetometer
    //TODO: enable ADCS orbit correction (may be after delay)

    //TODO: send power to other components
    turn_On_UHF_Transceiver(); //no explanation needed

    HAL_Delay(5000);
    uint8_t data;
    //DEPLOY_ANTENNA(hi2c1);//deploy antenna
    //SatFlags.Antenna_is_Released = 1;
    debug_printf("Setting beacon text: ");
    debug_HAL_Status(SET_BEACON_TEXT(hi2c1, INITIAL_BEACON_TEXT, INITIAL_BEACON_TEXT_SIZE, &data)); //set beacon message to default
    debug_printf("Setting beacon period: ");
    debug_HAL_Status(SET_BEACON_PERIOD(hi2c1, INITIAL_BEACON_PERIOD, &data)); //set beacon period
    debug_printf("Starting beacon: ");
    debug_HAL_Status(START_BEACON(hi2c1)); //Enable Beacon
    SatFlags.BeaconEnabled = 1;

    /* Create the thread(s) */
    /* definition and creation of defaultTask */

    //osThreadDef(myStartDefaultTask, StartDefaultTask, osPriorityLow, 0, 6*128);
    //osThreadCreate(osThread(myStartDefaultTask), NULL);

    //ESTTC_InitTask();

    /* Start scheduler */
    //osKernelStart();

    //TODO: enter PIPE mode and set timeout to 255 seconds

    uint8_t message[260];

    //initial phase loop
    while (1)
    {
        GREEN_LED_XOR(); //so we know it is breathing
        // TODO: check for UHF message
        if(debugSerialReceive(message) >= 5){
            CySat_Packet_t p = parseCySatPacket(message);
            if(validateCySatChecksum(p)){
                if(handleCySatPacket(p) == -1){
                    //log invalid subsystem type
                }
                AMBER_LED_ON();
            }
            else{
                continue; //TODO: send error message and log
            }
        }
        else{
            AMBER_LED_OFF(); //No message received
        }
    }

    //main operating loop
    while(1){

    }

    //end of life loop
    while(1){

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
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

/**
 * @brief This function takes in a newly received CySat Packet and acts upon the command within
 * @param packet: the packet to handle
 * @retval A zero indicates success, -1 means the command or subtype is unrecognized, else means failure
 */
int handleCySatPacket(CySat_Packet_t packet){

    switch(packet.Subsystem_Type){

    case CSPP_OBC_SUBTYPE: //OBC
        if(packet.Command == 0x01){ //Ping request command
            CySat_Packet_t outgoingPacket;
            outgoingPacket.Subsystem_Type = CSPP_OBC_SUBTYPE; //OBC module
            outgoingPacket.Command = 0x00; //Ping response
            outgoingPacket.Data_Length = 0x0C; //length of the message
            uint8_t d[] = "Hello Earth!";
            outgoingPacket.Data = d;
            outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);

            return sendCySatPacket(outgoingPacket); //send the response
        }
        else if(packet.Command == 0x03){ //Request beacon shutoff
            CySat_Packet_t outgoingPacket;
            outgoingPacket.Subsystem_Type = CSPP_OBC_SUBTYPE; //OBC module
            outgoingPacket.Command = 0x02; //Beacon shutoff response

            if(SatFlags.BeaconEnabled){
                //TODO: command UHF to shutoff beacon
                SatFlags.BeaconEnabled = 0;
                outgoingPacket.Data_Length = 0x0F;
                uint8_t d[] = "Beacon Shut Off";
                outgoingPacket.Data = d;
                outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
            }
            else{
                outgoingPacket.Data_Length = 0x12;
                uint8_t d[] = "Beacon Already Off";
                outgoingPacket.Data = d;
                outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);
            }

            return sendCySatPacket(outgoingPacket); //send the response
        }
        else if(packet.Command == 0x05){ //Basic Health Check Request
            CySat_Packet_t outgoingPacket;
            outgoingPacket.Subsystem_Type = CSPP_OBC_SUBTYPE; //OBC module
            outgoingPacket.Command = 0x04; //enter main operating phase response
            outgoingPacket.Data_Length = 0x3; //length of the message, one byte for each subsystem (EPS, OBC, UHF)
            uint8_t d[3]; //generally, a zero means a failure or unused.
            d[0] = 0x00; //OBC, all described elsewhere
            d[0] = 0x00; //EPS
            d[0] = 0x00; //UHF
            outgoingPacket.Data = d;
            outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);

            return sendCySatPacket(outgoingPacket); //send the response
        }
        else if(packet.Command == 0x07){ //Enter Main Operating Phase Request
            CySat_Packet_t outgoingPacket;
            outgoingPacket.Subsystem_Type = CSPP_OBC_SUBTYPE; //OBC module
            outgoingPacket.Command = 0x06; //enter main operating phase response
            outgoingPacket.Data_Length = 0x1; //length of the message
            uint8_t d[1] = "\0"; //0 means failure
            outgoingPacket.Data = d;
            outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);

            return sendCySatPacket(outgoingPacket); //send the response
        }
        break;

    case CSPP_ADCS_SUBTYPE: //ADCS

        break;

    case CSPP_EPS_SUBTYPE: //EPS
        if(packet.Command == 0x01){ //Request EPS battery voltage
            float voltage;
            READ_EPS_BATTERY_VOLTAGE(hi2c1, &voltage); //read the voltage from the EPS
            uint8_t bytes[sizeof(float)];
            FloatToUnsigned8bitArray(bytes, voltage); //get the voltage as 4 raw bytes

            CySat_Packet_t outgoingPacket;
            outgoingPacket.Subsystem_Type = CSPP_EPS_SUBTYPE; //EPS module
            outgoingPacket.Command = 0x00; //EPS battery voltage response
            outgoingPacket.Data_Length = sizeof(float);
            outgoingPacket.Data = bytes;
            outgoingPacket.Checksum = generateCySatChecksum(outgoingPacket);

            return sendCySatPacket(outgoingPacket); //send the response
        }
        else{
            return -1;
        }
        break;

    case CSPP_SDR_SUBTYPE: //SDR

        break;

    case CSPP_EOL_SUBTYPE: //End of Life

        break;

    default:
        return -1;
    }

    return 0;
}

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
      GREEN_LED_ON();
      AMBER_LED_ON();
      HAL_Delay(2000);
      GREEN_LED_OFF();
      AMBER_LED_OFF();
      HAL_Delay(2000);
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
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

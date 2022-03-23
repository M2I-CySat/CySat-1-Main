/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PING 0
#define SHUTOFF_BEACON 1
#define ENTER_MAIN_OP_PHASE 2
#define BASIC_HEALTH_CHECK 3
#define BATTERY_VOLTAGE 4
#define SDR_POWER_STATUS 5
#define TIME_SET_REQUEST 6
#define EOL 7
#define UNKNOWN_PACKET 8
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
HAL_StatusTypeDef debugSerialSendHello();
HAL_StatusTypeDef debug_printf(char* format, uint8_t *data);
void debugPrint(int len, uint8_t data[]);
int debugSerialReceive(uint8_t *data);
int parse_packet_command(int len, uint8_t *data);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  int bytes = 0;
  uint8_t data[260] = {0};
  srand(time(NULL));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  for(int i =0; i < 260; i++){
		  data[i] = 0;
	  }
	  bytes = debugSerialReceive(data);
	  if(bytes){
		  // Get command type
		  int packet_type = parse_packet_command(bytes, data);

		  // Don't respond to improper commands
		  if(packet_type == UNKNOWN_PACKET) continue;

		  // Reconstitute packet then reply
  	  	  *(data+2) = *(data+2) - 1; //Response -> reply
  		  uint8_t cs = *(data+bytes-1); //Capture checksum for now
		  switch(packet_type){
		  	  // Transform response
		  	  case PING: ;
		  	  case SHUTOFF_BEACON: ;
		  	  case ENTER_MAIN_OP_PHASE: ;
		  	  case EOL: ;
			  	  HAL_UART_Transmit(&huart1, data, bytes, 10);
			  	  break;

			  // Respond with IEEE-754 floating point for 4.5
		  	  case BATTERY_VOLTAGE: ;
		  		  // Set data length to 1
		  		  *(data+3) = 0x04;
		  		  // Shift data
		  		  *(data+7) = 0x40;
		  		  *(data+6) = 0x90;
		  		  *(data+5) = 0x00;
		  		  *(data+4) = 0x00;
		  		  *(data+8) = cs;
		  		  HAL_UART_Transmit(&huart1, data, bytes+4, 10);
		  		  break;

		  	  case SDR_POWER_STATUS: ;
	  	  	  	  // Simply respond that we got it
	  	  	  	  *(data+3) = 0x03;
	  	  	  	  *(data+4) = 'S';
	  	  	  	  *(data+5) = 'D';
	  	  	  	  *(data+6) = 'R';
	  	  	  	  *(data+7) = cs;
	  	  	  	  HAL_UART_Transmit(&huart1, data, 8, 10);
	  	  	  	  break;

		  	  case TIME_SET_REQUEST: ;
	  	  	  	  // Echo it
		  	  	  HAL_UART_Transmit(&huart1, data, bytes, 10);
		  	  	  break;

		  	  case BASIC_HEALTH_CHECK: ;
	  	  	  	  // Simply respond that we got it
		  	  	  *(data+3) = 0x03;
		  	  	  *(data+4) = 'B';
		  	  	  *(data+5) = 'H';
		  	  	  *(data+6) = 'C';
		  	  	  *(data+7) = cs;
		  	  	  HAL_UART_Transmit(&huart1, data, 8, 10);
		  	  	  break;
		  }
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
HAL_StatusTypeDef debugSerialSendHello(){
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t message[] = "Hello, Ground Station!";
	int len = sizeof(message)/sizeof(uint8_t);
	if(len){
		status = HAL_UART_Transmit(&huart1, message, len, 1000);
		uint8_t crlf[] = "\r\n";
		status = HAL_UART_Transmit(&huart1, crlf, 2, 1000);
	} {
		status = HAL_ERROR;
	}
	return status;
}

int debugSerialReceive(uint8_t *data){
	HAL_StatusTypeDef status;

	uint8_t byte;
	size_t bytes = 0;
	status = HAL_UART_Receive(&huart1, &byte, 1, 10);
	if(byte != 0xff){
		return 0;
	}

	while(status != HAL_TIMEOUT){
	    data[bytes] = byte;
	    bytes++;
	    if(bytes == 260) break;
	    status = HAL_UART_Receive(&huart1, &byte, 1, 10);
    }
	return bytes;
}

int parse_packet_command(int len, uint8_t *data){
	if(len < 4) return UNKNOWN_PACKET;
	if(*(data) != 0xff) return UNKNOWN_PACKET;

	// OBC Commands
	if(*(data+1) == 0x0a){
		if(*(data+2) == 0x01) return PING;
		else if(*(data+2) == 0x03) return SHUTOFF_BEACON;
		else if(*(data+2) == 0x05) return BASIC_HEALTH_CHECK;
		else if(*(data+2) == 0x07) return ENTER_MAIN_OP_PHASE;
	}
	//EPS Commands
	if(*(data+1) == 0x1e){
		if(*(data+2) == 0x01) return BATTERY_VOLTAGE;
	}
	//SDR Commands
	if(*(data+1) == 0x28){
		if(*(data+2) == 0x01) return SDR_POWER_STATUS;
		else if(*(data+2) == 0x11) return TIME_SET_REQUEST;
	}
	//EOL Command
	if(*(data+1) == 0x5a){
		if(*(data+2) == 0x01) return EOL;
	}
	return UNKNOWN_PACKET;
}

void debugPrint(int len, uint8_t *data){
	HAL_UART_Transmit(&huart1, data, len, 10);
	uint8_t crlf[] = "\r\n";
	HAL_UART_Transmit(&huart1, crlf, 2, 10);
}

int isPingRequest(int len, uint8_t *data){
	if(len == 5){
		if(*(data+0) == 0xff &&
				*(data+1) == 0x0a &&
				*(data+2) == 0x01 &&
				*(data+3) == 0x00){
			return 1;
		}
	}
	return 0;
}

HAL_StatusTypeDef debug_printf(char* format, uint8_t *data){

    uint32_t bufferSize = 512;
    uint8_t formatBuffer[bufferSize];
    sprintf(formatBuffer, format, data);
    int len = strlen(formatBuffer);
    HAL_StatusTypeDef status = HAL_ERROR;
    if (len > 0) {
        status = HAL_UART_Transmit(&huart1, formatBuffer, len, 1000);
        uint8_t crlf[] = "\r\n";
        status = HAL_UART_Transmit(&huart1, crlf, 2, 1000); //sends a carriage return and a line feed to UART (meant for putty/windows)
    }
    else{
        status = HAL_ERROR;
    }

    return status;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


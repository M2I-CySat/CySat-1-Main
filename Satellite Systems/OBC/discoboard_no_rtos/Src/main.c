/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "helper_functions.h"
#include "BatteryCapacity.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
HAL_StatusTypeDef IMU_Startup();
HAL_StatusTypeDef I2C_WRITE_VALUE_TO_REG(uint8_t slaveAddress, uint8_t regAddress, uint8_t regValue);
HAL_StatusTypeDef IMU_READ_DATA(uint8_t* data_ptr);
void IMU_Debug(uint8_t* data_ptr);
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
	MX_CRC_Init();
	MX_DMA2D_Init();
	MX_FMC_Init();
	MX_I2C3_Init();
	MX_LTDC_Init();
	MX_SPI5_Init();
	MX_TIM1_Init();
	MX_USART1_UART_Init();
	MX_USB_HOST_Init();
	/* USER CODE BEGIN 2 */
	/* IMU testing
	debug_HAL_Status(IMU_Startup());
	uint8_t* data_pointer = (uint8_t*) malloc(14);
	*/
	/* USER CODE END 2 */



	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	int i = 0;
	while(0)
	//while (1)
	{
	/* USER CODE END WHILE */
	MX_USB_HOST_Process();

	/* USER CODE BEGIN 3 */

		/* IMU testing
		debug_HAL_Status(IMU_READ_DATA(data_pointer));
		IMU_Debug(data_pointer);
		*/

		uint8_t* message;
		HAL_StatusTypeDef status;
		status = serialReceive(&message); //poll for a header message
		if(status == HAL_OK){
		debug_printf("Message received: %s", message); //if message was received correctly, display it on a new line
		}
		else if(status == HAL_ERROR)
		debug_printf("%d: Error %d!", i, status);
		i++;
	}

//	float f = 123.456;
//
//	debug_printf("Float test: %f. Long float test: %lf. Exp test: %e. Dec test: %d. Hex test: 0x%04x", 0.6f, 0.6, 0.6, 60, 60);
//	debug_printf("float variable test: %f\n", f);

	testEnergyDischarge();
  
	//free(data_pointer);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

HAL_StatusTypeDef IMU_Startup(){
	HAL_StatusTypeDef status = HAL_ERROR;

	status = I2C_WRITE_VALUE_TO_REG(IMU_ADDRESS, 0x6b, 0x80);
	if(status != HAL_OK){
		debug_printf("after first command");
		return status;
	}
	// Wait 5ms
	HAL_Delay(5U);
	status = I2C_WRITE_VALUE_TO_REG(IMU_ADDRESS, 0x6b, 0x03);
	if(status != HAL_OK){
		debug_printf("after second command");
		return status;
	}

	status = I2C_WRITE_VALUE_TO_REG(IMU_ADDRESS, 0x1a, 0x03);
	if(status != HAL_OK){
		debug_printf("after third command");
		return status;
	}

	status = I2C_WRITE_VALUE_TO_REG(IMU_ADDRESS, 0x1b, 0x18);
	if(status != HAL_OK){
		debug_printf("after fourth command");
		return status;
	}

	status = I2C_WRITE_VALUE_TO_REG(IMU_ADDRESS, 0x1c, 0x10);
	if(status != HAL_OK){
		debug_printf("after fifth command");
		return status;
	}

	return status;
}

void IMU_Debug(uint8_t* data_ptr){
	int accel_scale = 8;
	int gyro_scale = 2000;

	uint16_t ACCEL_XOUT = (((uint16_t) data_ptr[0])  << 0x08) | ((uint16_t) data_ptr[1]);
	uint16_t ACCEL_YOUT = (((uint16_t) data_ptr[2])  << 0x08) | ((uint16_t) data_ptr[3]);
	uint16_t ACCEL_ZOUT = (((uint16_t) data_ptr[4])  << 0x08) | ((uint16_t) data_ptr[5]);
	uint16_t TEMP_OUT   = (((uint16_t) data_ptr[6])  << 0x08) | ((uint16_t) data_ptr[7]);
	uint16_t GYRO_XOUT   = (((uint16_t) data_ptr[8])  << 0x08) | ((uint16_t) data_ptr[9]);
	uint16_t GYRO_YOUT   = (((uint16_t) data_ptr[10]) << 0x08) | ((uint16_t) data_ptr[11]);
	uint16_t GYRO_ZOUT   = (((uint16_t) data_ptr[12]) << 0x08) | ((uint16_t) data_ptr[13]);

	int32_t final_accel_x = (((int32_t) ACCEL_XOUT - 32767) * accel_scale) / 32767;
	int32_t final_accel_y = (((int32_t) ACCEL_YOUT - 32767) * accel_scale) / 32767;
	int32_t final_accel_z = (((int32_t) ACCEL_ZOUT - 32767) * accel_scale) / 32767;
	int32_t final_temp = ((int32_t) TEMP_OUT / IMU_TEMP_SENSITIVITY) + 21;
	int32_t final_gyro_x = (((int32_t) GYRO_XOUT - 32767) * gyro_scale) / 32767;
	int32_t final_gyro_y = (((int32_t) GYRO_YOUT - 32767) * gyro_scale) / 32767;
	int32_t final_gyro_z = (((int32_t) GYRO_ZOUT - 32767) * gyro_scale) / 32767;

	debug_printf("RAW DATA: Accelerometer (X, Y, Z) = (%hu, %hu, %hu)\r\nTemperature = %hu\r\nGyroscope (X, Y, Z) = (%hu, %hu, %hu)",
					 ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT, TEMP_OUT, GYRO_XOUT, GYRO_YOUT, GYRO_ZOUT);

	debug_printf("\tAccelerometer (X, Y, Z) = (%d, %d, %d)\r\n\tTemperature = %d degrees celsius\r\n\tGyroscope (X, Y, Z) = (%d, %d, %d)",
				 final_accel_x, final_accel_y, final_accel_z, final_temp, final_gyro_x, final_gyro_y, final_gyro_z);
}

HAL_StatusTypeDef IMU_READ_DATA(uint8_t* data_ptr){
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* temp = (uint8_t*) malloc(1);
	*temp = 0x3b; //start reading from register 0x3b
	status = HAL_I2C_Master_Transmit(&hi2c3, (uint16_t) (IMU_ADDRESS << 0x1), temp, 1, HAL_MAX_DELAY);
	if(status != HAL_OK){
		return status;
	}
	free(temp);
	status = HAL_I2C_Master_Receive(&hi2c3, (uint16_t) (IMU_ADDRESS << 0x1), data_ptr, 14, HAL_MAX_DELAY);

	return status;
}

/*Assumes the slave address is not already shifted left*/
HAL_StatusTypeDef I2C_WRITE_VALUE_TO_REG(uint8_t slaveAddress, uint8_t regAddress, uint8_t regValue){
	HAL_StatusTypeDef status = HAL_ERROR;

	uint8_t* data = (uint8_t*) malloc(2); //allocate a pointer to 2 bytes of data
	data[0] = regAddress; //put the register address in the first byte
	data[1] = regValue; //put the register value in the second byte

	status = HAL_I2C_Master_Transmit(&hi2c3, (uint16_t) (slaveAddress << 0x1), data, 2, HAL_MAX_DELAY);
	free(data);
	return status;
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

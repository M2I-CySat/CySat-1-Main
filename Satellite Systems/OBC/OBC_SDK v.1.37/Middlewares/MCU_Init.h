/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file MCU_Init.h
* @brief Header of MCU_Init.c
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @author            Vasil Milev
* @version           1.0.0
* @date              2019.01.10
*
* @copyright         (C) Copyright Endurosat
*
*                    Contents and presentations are protected world-wide.
*                    Any kind of using, copying etc. is prohibited without prior permission.
*                    All rights - incl. industrial property rights - are reserved.
*
* @history
* @revision{         1.0.0  , 2019.01.10, author Vasil Milev, Initial revision }
* @endhistory
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#ifndef MCU_INIT_H
#define MCU_INIT_H

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include "stm32f4xx_hal.h"

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define PAN4_VGY_Pin GPIO_PIN_2
#define PAN4_VGY_GPIO_Port GPIOE
#define M2_IRQ_Pin GPIO_PIN_6
#define M2_IRQ_GPIO_Port GPIOE
#define M1_IRQ_Pin GPIO_PIN_8
#define M1_IRQ_GPIO_Port GPIOI
#define PAN5_CS2_Pin GPIO_PIN_13
#define PAN5_CS2_GPIO_Port GPIOC
#define PAN5_CS1_Pin GPIO_PIN_9
#define PAN5_CS1_GPIO_Port GPIOI
#define PAN6_CS2_Pin GPIO_PIN_10
#define PAN6_CS2_GPIO_Port GPIOI
#define PAN6_CS1_Pin GPIO_PIN_11
#define PAN6_CS1_GPIO_Port GPIOI
#define SBAND_CS_Pin GPIO_PIN_6
#define SBAND_CS_GPIO_Port GPIOF
#define SBAND_SCK_Pin GPIO_PIN_7
#define SBAND_SCK_GPIO_Port GPIOF
#define SBAND_MISO_Pin GPIO_PIN_8
#define SBAND_MISO_GPIO_Port GPIOF
#define SBAND_MOSI_Pin GPIO_PIN_9
#define SBAND_MOSI_GPIO_Port GPIOF
#define SBAND_IRQ_Pin GPIO_PIN_10
#define SBAND_IRQ_GPIO_Port GPIOF
#define PAN1_AN_Pin GPIO_PIN_0
#define PAN1_AN_GPIO_Port GPIOC
#define PAN2_AN_Pin GPIO_PIN_1
#define PAN2_AN_GPIO_Port GPIOC
#define PAN3_AN_Pin GPIO_PIN_2
#define PAN3_AN_GPIO_Port GPIOC
#define PAN4_AN_Pin GPIO_PIN_3
#define PAN4_AN_GPIO_Port GPIOC
#define PAN5_AN_Pin GPIO_PIN_2
#define PAN5_AN_GPIO_Port GPIOA
#define PAN5_DIR1_Pin GPIO_PIN_2
#define PAN5_DIR1_GPIO_Port GPIOH
#define PAN5_DIR2_Pin GPIO_PIN_3
#define PAN5_DIR2_GPIO_Port GPIOH
#define PAN6_DIR1_Pin GPIO_PIN_4
#define PAN6_DIR1_GPIO_Port GPIOH
#define PAN6_DIR2_Pin GPIO_PIN_5
#define PAN6_DIR2_GPIO_Port GPIOH
#define PAN6_AN_Pin GPIO_PIN_3
#define PAN6_AN_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_4
#define LED_G_GPIO_Port GPIOC
#define LED_Y_Pin GPIO_PIN_5
#define LED_Y_GPIO_Port GPIOC
#define PAN5_VGY_Pin GPIO_PIN_1
#define PAN5_VGY_GPIO_Port GPIOB
#define PAN6_VGY_Pin GPIO_PIN_2
#define PAN6_VGY_GPIO_Port GPIOB
#define SEN_SCL_Pin GPIO_PIN_10
#define SEN_SCL_GPIO_Port GPIOB
#define SEN_SDA_Pin GPIO_PIN_11
#define SEN_SDA_GPIO_Port GPIOB
#define ACC1_EN_Pin GPIO_PIN_6
#define ACC1_EN_GPIO_Port GPIOH
#define PAY_SCL_Pin GPIO_PIN_7
#define PAY_SCL_GPIO_Port GPIOH
#define PAY_SDA_Pin GPIO_PIN_8
#define PAY_SDA_GPIO_Port GPIOH
#define ACC2_EN_Pin GPIO_PIN_9
#define ACC2_EN_GPIO_Port GPIOH
#define PAN4_PWM_Pin GPIO_PIN_10
#define PAN4_PWM_GPIO_Port GPIOH
#define PAN5_PWM_Pin GPIO_PIN_11
#define PAN5_PWM_GPIO_Port GPIOH
#define PAN6_PWM_Pin GPIO_PIN_12
#define PAN6_PWM_GPIO_Port GPIOH
#define PAN_SCK_Pin GPIO_PIN_13
#define PAN_SCK_GPIO_Port GPIOB
#define PAN_MISO_Pin GPIO_PIN_14
#define PAN_MISO_GPIO_Port GPIOB
#define PAN_MOSI_Pin GPIO_PIN_15
#define PAN_MOSI_GPIO_Port GPIOB
#define OBC_OUT5_Pin GPIO_PIN_6
#define OBC_OUT5_GPIO_Port GPIOG
#define PAN4_DIR1_Pin GPIO_PIN_7
#define PAN4_DIR1_GPIO_Port GPIOG
#define FLASH_NCS_Pin GPIO_PIN_8
#define FLASH_NCS_GPIO_Port GPIOG
#define PAY_TX_Pin GPIO_PIN_6
#define PAY_TX_GPIO_Port GPIOC
#define PAY_RX_Pin GPIO_PIN_7
#define PAY_RX_GPIO_Port GPIOC
#define MAG1_EN_Pin GPIO_PIN_13
#define MAG1_EN_GPIO_Port GPIOH
#define MAG2_EN_Pin GPIO_PIN_14
#define MAG2_EN_GPIO_Port GPIOH
#define PAN4_DIR2_Pin GPIO_PIN_15
#define PAN4_DIR2_GPIO_Port GPIOH
#define PAN1_CS2_Pin GPIO_PIN_0
#define PAN1_CS2_GPIO_Port GPIOI
#define PAN1_CS1_Pin GPIO_PIN_1
#define PAN1_CS1_GPIO_Port GPIOI
#define PAN2_CS2_Pin GPIO_PIN_2
#define PAN2_CS2_GPIO_Port GPIOI
#define PAN2_CS1_Pin GPIO_PIN_3
#define PAN2_CS1_GPIO_Port GPIOI
#define SD_EN_Pin GPIO_PIN_3
#define SD_EN_GPIO_Port GPIOD
#define OBC_OUT4_Pin GPIO_PIN_6
#define OBC_OUT4_GPIO_Port GPIOD
#define OBC_OUT3_Pin GPIO_PIN_9
#define OBC_OUT3_GPIO_Port GPIOG
#define OBC_OUT2_Pin GPIO_PIN_10
#define OBC_OUT2_GPIO_Port GPIOG
#define OBC_OUT1_Pin GPIO_PIN_11
#define OBC_OUT1_GPIO_Port GPIOG
#define FLASH_MISO_Pin GPIO_PIN_12
#define FLASH_MISO_GPIO_Port GPIOG
#define FLASH_SCK_Pin GPIO_PIN_13
#define FLASH_SCK_GPIO_Port GPIOG
#define FLASH_MOSI_Pin GPIO_PIN_14
#define FLASH_MOSI_GPIO_Port GPIOG
#define SD_DET_Pin GPIO_PIN_15
#define SD_DET_GPIO_Port GPIOG
#define PAN1_VGY_Pin GPIO_PIN_5
#define PAN1_VGY_GPIO_Port GPIOB
#define PAN2_VGY_Pin GPIO_PIN_6
#define PAN2_VGY_GPIO_Port GPIOB
#define PAN3_VGY_Pin GPIO_PIN_7
#define PAN3_VGY_GPIO_Port GPIOB
#define SYS_SCL_Pin GPIO_PIN_8
#define SYS_SCL_GPIO_Port GPIOB
#define SYS_SDA_Pin GPIO_PIN_9
#define SYS_SDA_GPIO_Port GPIOB
#define PAN3_CS2_Pin GPIO_PIN_4
#define PAN3_CS2_GPIO_Port GPIOI
#define PAN3_CS1_Pin GPIO_PIN_5
#define PAN3_CS1_GPIO_Port GPIOI
#define PAN4_CS2_Pin GPIO_PIN_6
#define PAN4_CS2_GPIO_Port GPIOI
#define PAN4_CS1_Pin GPIO_PIN_7
#define PAN4_CS1_GPIO_Port GPIOI


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL TYPES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No External types declarations */

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;

extern SD_HandleTypeDef hsd;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi5;
extern SPI_HandleTypeDef hspi6;
extern TIM_HandleTypeDef htim5;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

extern WWDG_HandleTypeDef hwwdg;

extern SRAM_HandleTypeDef hsram1;

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL ROUTINES DECLARATIONS 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void SystemClock_Config(void);

void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_CAN1_Init(void);
void MX_FMC_Init(void);
void MX_I2C1_Init(void);
void MX_I2C2_Init(void);
void MX_I2C3_Init(void);
void MX_SDIO_SD_Init(void);
void MX_SPI1_Init(void);
void MX_SPI2_Init(void);
void MX_SPI5_Init(void);
void MX_SPI6_Init(void);
void MX_TIM5_Init(void);
void MX_UART4_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART6_UART_Init(void);
void MX_RTC_Init(void);

void MX_WWDG_Init(void);

#endif    /* MCU_INIT_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

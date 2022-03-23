/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file main.h
* @brief Header of main.c
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
#ifndef MAIN_H
#define MAIN_H

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INCLUDES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#include <GroundStation.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

#include <stdint.h>
#include <stdio.h>

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#if !defined(__FILE_defined)
//typedef __FILE FILE;
# define __FILE_defined
#endif

#define COM1    (FILE *)&huart1
#define COM4    (FILE *)&huart4
#define COM6    (FILE *)&huart6
#define GREEN_LED_ON() GPIOC->ODR |= 0x0010;
#define GREEN_LED_OFF() GPIOC->ODR &= ~0x0010;
#define GREEN_LED_XOR() GPIOC->ODR ^= 0x0010;
#define AMBER_LED_ON() GPIOC->ODR |= 0x0020;
#define AMBER_LED_OFF() GPIOC->ODR &= ~0x0020;
#define AMBER_LED_XOR() GPIOC->ODR ^= 0x0020;

//#define HMC5883 1
#define         LIS3MDL         1

#define APPL_ADDRESS            0x08040000
#define BOOT_ADDRESS            0x08000000
#define RTC_INIT_ADDRESS        0x40002850
#define MAILBOX_ADDRESS         0x40002854
#define APP_SIZE_ADDRESS        0x40002858
#define APP_CHKS_ADDRESS        0x4000285C
#define MAILBOX_VAL_BOOT        0xB007B007
#define MAILBOX_VAL_APPL        0xA552A552
#define MAILBOX_VAL_FILE        0xF12EF12E
#define MAILBOX_VAL_HARD        0x0BAD0BAD

#define FLASH_BLANK             0xFF
#define FLASH_SECTOR_SIZE       0x20000
#define FLASH_MIN_SECTOR        6
#define FLASH_MAX_SECTOR        23
#define FLASH_MIN_ADDR          APPL_ADDRESS
#define FLASH_MAX_ADDR          0x08200000
#define FLASH_SIZE      (FLASH_MAX_ADDR - FLASH_MIN_ADDR)

#define UART_BUFFER_SIZE    256

#define CONTROLLING_TIME_HALF_AN_HOUR    300
#define CONTROLLING_TIME_ONE_HOUR        2*CONTROLLING_TIME_HALF_AN_HOUR
#define CONTROLLING_TIME_TWO_HOURS       4*CONTROLLING_TIME_HALF_AN_HOUR
#define CONTROLLING_TIME_THREE_HOURS     6*CONTROLLING_TIME_HALF_AN_HOUR
#define CONTROLLING_TIME_MAX             8*CONTROLLING_TIME_HALF_AN_HOUR

#define CONTROLLING_TIME                 CONTROLLING_TIME_ONE_HOUR

#define BATT_LOW_3_5V                   1489
#define BATT_LOW_3_6V                   1531
#define BATT_LOW_3_7V                   1574
#define BATT_LOW_3_75V                  ((BATT_LOW_3_7V+BATT_LOW_3_8V)/2)
#define BATT_LOW_3_8V                   1617
#define BATT_LOW_3_85V                  ((BATT_LOW_3_8V+BATT_LOW_3_9V)/2)
#define BATT_LOW_3_9V                   1659
#define BATT_LOW_4_0V                   1702
#define BATT_LOW_4_1V                   1744

#define BATT_CURRENT_250mA              83
#define BATT_CURRENT_500mA              167
#define BATT_CURRENT_1000mA             333

#define POWER_ON_GYRO_1  { HAL_GPIO_WritePin(GPIOB, PAN1_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_2  { HAL_GPIO_WritePin(GPIOB, PAN2_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_3  { HAL_GPIO_WritePin(GPIOB, PAN3_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_4  {  HAL_GPIO_WritePin(PAN4_VGY_GPIO_Port, PAN4_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_5  {  HAL_GPIO_WritePin(GPIOB, PAN5_VGY_Pin, GPIO_PIN_RESET);  }
#define POWER_ON_GYRO_6  {  HAL_GPIO_WritePin(GPIOB, PAN6_VGY_Pin, GPIO_PIN_RESET);  }

#define POWER_OFF_GYRO_1  { HAL_GPIO_WritePin(GPIOB, PAN1_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_2  { HAL_GPIO_WritePin(GPIOB, PAN2_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_3  { HAL_GPIO_WritePin(GPIOB, PAN3_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_4  {  HAL_GPIO_WritePin(PAN4_VGY_GPIO_Port, PAN4_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_5  {  HAL_GPIO_WritePin(GPIOB, PAN5_VGY_Pin, GPIO_PIN_SET);  }
#define POWER_OFF_GYRO_6  {  HAL_GPIO_WritePin(GPIOB, PAN6_VGY_Pin, GPIO_PIN_SET);  }

#define POWER_ON_ACC_1  { HAL_GPIO_WritePin(GPIOH, ACC1_EN_Pin, GPIO_PIN_RESET);  }
#define POWER_OFF_ACC_1  { HAL_GPIO_WritePin(GPIOH, ACC1_EN_Pin, GPIO_PIN_SET);  }
#define POWER_ON_ACC_2  { HAL_GPIO_WritePin(GPIOH, ACC2_EN_Pin, GPIO_PIN_RESET);  }
#define POWER_OFF_ACC_2  { HAL_GPIO_WritePin(GPIOH, ACC2_EN_Pin, GPIO_PIN_SET);  }

/*
  // Turn ON UHF
#define POWER_ON_UHF_1  { HAL_GPIO_WritePin(OBC_OUT5_GPIO_Port, OBC_OUT5_Pin, GPIO_PIN_SET); }
#define POWER_ON_UHF_2  { HAL_GPIO_WritePin(OBC_OUT4_GPIO_Port, OBC_OUT4_Pin, GPIO_PIN_RESET); }

  // Turn OFF UHF
#define POWER_OFF_UHF_1  { HAL_GPIO_WritePin(GPIOG, OBC_OUT5_Pin, GPIO_PIN_RESET); }
#define POWER_OFF_UHF_2  { HAL_GPIO_WritePin(GPIOG, OBC_OUT4_Pin, GPIO_PIN_SET); }
*/

#define	__disable_interrupt()	__disable_irq()
#define	__enable_interrupt()	__enable_irq()

#define LINE_BUFFER_SIZE UART_BUFFER_SIZE

#define IDIV_ROUND(n, d) ((((n) < 0) ^ ((d) < 0)) ? (((n) - (d)/2)/(d)) : (((n) + (d)/2)/(d)))


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL TYPES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

typedef void (*pFunction)(void);

typedef struct Sat_Flags{
  unsigned int Detumbling : 1;
  unsigned int Infinity_Detumbling : 1;
  unsigned int Auto_Control : 1;
  unsigned int AD_is_Running : 1;
  unsigned int Antenna_is_Released : 1;
  unsigned int Battery_Low : 1;
  unsigned int BeaconEnabled : 1;
  unsigned int MelodyEnabled : 1;
  unsigned int CameraReady : 1;
  unsigned int MakePicture : 1;
  unsigned int Gyros_ON_OFF : 1;
} Sat_Flags_t;

typedef struct diag_stats {
  uint32_t reset_cnt;
  uint32_t detumbling_num;
  uint32_t autocontrol_num;
} diag_stats_t;

typedef struct {
  uint16_t angle;
  uint16_t rate;
  uint16_t temp;
} gyro_data_t;

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL VARIABLES DECLARATIONS
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
extern FILE *COMM;
extern FILE *SYSCON;
extern FILE *PAYLOAD;

extern uint32_t calendar_format;

extern Sat_Flags_t SatFlags;

extern uint16_t obc[];

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL ROUTINES DECLARATIONS 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

void Error_Handler(void);

void I2C2_Reset (void);

int handleCySatPacket(CySat_Packet_t packet);

int fprintf(FILE *f, const char* format,...);

#endif    /* MAIN_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

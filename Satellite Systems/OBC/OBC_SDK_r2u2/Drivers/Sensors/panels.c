/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file Panels.c
* @brief Manage the panels with all the sensors ( magnetorquer, gyroscope, temeperature sensor, sun sensor )
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
#include "stm32f4xx_hal.h"
#include "main.h"
#include "MCU_Init.h"
#include "cmsis_os.h"
#include "panels.h"
#include <stdio.h>
#include "math.h"

#include <stdint.h>
#include <string.h>
/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
#define	__no_operation				(__NOP)
#define OUTPUT_B_DOT_MAGNIFIER 		3
#define MAGNETORQUE_DIPOLE      	0.058 / OUTPUT_B_DOT_MAGNIFIER

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
volatile uint8_t PanelStat = 0;
uint16_t PanelLight[MAX_PAN] = {0,0,0,0,0,0};
uint16_t PanelTemp[MAX_PAN] = {0,0,0,0,0,0};
volatile uint8_t GyroStat = 0;

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) VARIABLES DEFINITION/DECLARATION 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static const uint32_t AdcChannel[MAX_PAN] = {ADC_CHANNEL_10, ADC_CHANNEL_11, ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_2, ADC_CHANNEL_3};

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
void Panels_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  PanelStat = 0;
  
  // Configure all panel temperature sensors CS as inputs with pull-down 
  GPIO_InitStruct.Pin = PAN5_CS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PAN5_CS2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PAN6_CS2_Pin|PAN1_CS2_Pin|PAN2_CS2_Pin|PAN3_CS2_Pin|PAN4_CS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
  
  osDelay(10);
  
  if (HAL_GPIO_ReadPin(PAN1_CS2_GPIO_Port, PAN1_CS2_Pin)) PanelStat |= (1 << PAN_X_P);
  if (HAL_GPIO_ReadPin(PAN2_CS2_GPIO_Port, PAN2_CS2_Pin)) PanelStat |= (1 << PAN_Y_P);
  if (HAL_GPIO_ReadPin(PAN3_CS2_GPIO_Port, PAN3_CS2_Pin)) PanelStat |= (1 << PAN_Z_P);
  if (HAL_GPIO_ReadPin(PAN4_CS2_GPIO_Port, PAN4_CS2_Pin)) PanelStat |= (1 << PAN_X_M);
  if (HAL_GPIO_ReadPin(PAN5_CS2_GPIO_Port, PAN5_CS2_Pin)) PanelStat |= (1 << PAN_Y_M);
  if (HAL_GPIO_ReadPin(PAN6_CS2_GPIO_Port, PAN6_CS2_Pin)) PanelStat |= (1 << PAN_Z_M);  
  
  // Configure all panel temperature sensors CS as push-pull outputs
  GPIO_InitStruct.Pin = PAN5_CS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PAN6_CS2_Pin|PAN1_CS2_Pin|PAN2_CS2_Pin|PAN3_CS2_Pin|PAN4_CS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
  
  htim5.Init.Period = MAGTORQ_PWM_PERIOD;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  
}

status_t SetMagnetorque(uint8_t Panel, uint8_t perc, uint8_t dir)
{
  uint32_t tmp, chn;
  TIM_OC_InitTypeDef sConfigOC;

  if(perc > 100) tmp = 100;
  else tmp = perc;
  
  if (Panel == PAN_X_M)
  {
    chn = TIM_CHANNEL_1;
    if (dir)
    {
      HAL_GPIO_WritePin(PAN4_DIR1_GPIO_Port, PAN4_DIR1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PAN4_DIR2_GPIO_Port, PAN4_DIR2_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(PAN4_DIR2_GPIO_Port, PAN4_DIR2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PAN4_DIR1_GPIO_Port, PAN4_DIR1_Pin, GPIO_PIN_SET);
    }
  }
  else
  if (Panel == PAN_Y_M)
  {
    chn = TIM_CHANNEL_2;
    if (dir)
    {
      HAL_GPIO_WritePin(PAN5_DIR1_GPIO_Port, PAN5_DIR1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PAN5_DIR2_GPIO_Port, PAN5_DIR2_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(PAN5_DIR2_GPIO_Port, PAN5_DIR2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PAN5_DIR1_GPIO_Port, PAN5_DIR1_Pin, GPIO_PIN_SET);
    }
  }
  else
  if (Panel == PAN_Z_M)
  {
    chn = TIM_CHANNEL_3;
    if (dir)
    {
      HAL_GPIO_WritePin(PAN6_DIR1_GPIO_Port, PAN6_DIR1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PAN6_DIR2_GPIO_Port, PAN6_DIR2_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(PAN6_DIR2_GPIO_Port, PAN6_DIR2_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(PAN6_DIR1_GPIO_Port, PAN6_DIR1_Pin, GPIO_PIN_SET);
    }
  }  
  else 
    return SEN_ERROR;

  tmp = (tmp * (MAGTORQ_PWM_PERIOD+1) + 50) / 100;

  HAL_TIM_PWM_Stop(&htim5, chn);
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = tmp;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, chn);
  HAL_TIM_PWM_Start(&htim5, chn);
  
  return SEN_SUCCESS;
}

void Magnetorquers_Update (Magnetorquer_Axis_t MT_level){

// Connection matrix
// magnetometer X+  <-> magnetorquer Z+
// magnetometer Y+  <-> magnetorquer Z+
// magnetometer Z+  <-> magnetorquer X-
// this is depends how magnetorquers are connected to the OBC !

	/*
	typedef struct tLev {
	  uint8_t x;
	  uint8_t y;
	  uint8_t z;
	}tLev;
	*/

	/*
	tLev Lev[100];
	uint8_t k = 0;
	*/

  static uint8_t LevX = 0, LevY = 0, LevZ = 0; // value in percentage
  static uint8_t DirX = 1, DirY = 1, DirZ = 1;

  if (MT_level.AXIS_X >= 0)  DirX = 1;   // set direction
      else DirX = 0;
  if (MT_level.AXIS_Y >= 0)  DirY = 0;
      else DirY = 1;
  if (MT_level.AXIS_Z >= 0)  DirZ = 0;
      else DirZ = 1;

    LevX = (uint8_t) (sqrt(pow(MT_level.AXIS_X / (MAGNETORQUE_DIPOLE/100),2))); if (LevX > 100) LevX = 100;
    LevY = (uint8_t) (sqrt(pow(MT_level.AXIS_Y / (MAGNETORQUE_DIPOLE/100),2))); if (LevY > 100) LevY = 100;
    LevZ = (uint8_t) (sqrt(pow(MT_level.AXIS_Z / (MAGNETORQUE_DIPOLE/100),2))); if (LevZ > 100) LevZ = 100;
 /*
    Lev[k++].x = LevX; Lev[k++].y= LevY; Lev[k++].z = LevZ;
    if (k == 99) {
      k = 0;
    }
*/
    SetMagnetorque(PAN_X_M, LevZ, DirZ);
    SetMagnetorque(PAN_Y_M, LevY, DirY);
    SetMagnetorque(PAN_Z_M, LevX, DirX);

}

void Boost_Magnetorquers (uint8_t Arrow) {

    SetMagnetorque(PAN_X_M, 100, Arrow);
    SetMagnetorque(PAN_Y_M, 100, Arrow);
    SetMagnetorque(PAN_Z_M, 100, Arrow);

}

void Stop_Magnetorquers (void) {

    SetMagnetorque(PAN_X_M, 0, 0);
    SetMagnetorque(PAN_Y_M, 0, 0);
    SetMagnetorque(PAN_Z_M, 0, 0);

}


status_t Pan_PD_ADC_Measure(uint8_t Panel, uint16_t *val)
{
  ADC_ChannelConfTypeDef sConfig;
  
  //Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  sConfig.Channel = AdcChannel[Panel];
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    return SEN_ERROR;  
  if (HAL_ADC_Start(&hadc1) != HAL_OK)
    return SEN_ERROR;
  if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    return SEN_ERROR;
  *val = (uint16_t)HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
  return SEN_SUCCESS;
}

void Panel_GetPhotodiodesLum(void)
{
  for (uint8_t i = 0; i < MAX_PAN; i++)
  {
    if (Pan_PD_ADC_Measure(i, (uint16_t *)&PanelLight[i]) != SEN_SUCCESS)
      PanelLight[i] = 0xFFFF;
  }
}

void SPI_SlaveSelect(uint8_t Mode)
{
  switch (Mode & (MAX_PAN+1))
  {
    case 0:
      if (Mode & TEMP_CS)
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN1_CS2_GPIO_Port, PAN1_CS2_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN1_CS2_GPIO_Port, PAN1_CS2_Pin, GPIO_PIN_RESET);
      }
      else
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN1_CS1_GPIO_Port, PAN1_CS1_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN1_CS1_GPIO_Port, PAN1_CS1_Pin, GPIO_PIN_RESET);
      }
      break;
      
    case 1:
      if (Mode & TEMP_CS)
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN2_CS2_GPIO_Port, PAN2_CS2_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN2_CS2_GPIO_Port, PAN2_CS2_Pin, GPIO_PIN_RESET);
      }
      else
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN2_CS1_GPIO_Port, PAN2_CS1_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN2_CS1_GPIO_Port, PAN2_CS1_Pin, GPIO_PIN_RESET);
      }
      break;
      
    case 2:
      if (Mode & TEMP_CS)
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN3_CS2_GPIO_Port, PAN3_CS2_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN3_CS2_GPIO_Port, PAN3_CS2_Pin, GPIO_PIN_RESET);
      }
      else
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN3_CS1_GPIO_Port, PAN3_CS1_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN3_CS1_GPIO_Port, PAN3_CS1_Pin, GPIO_PIN_RESET);
      }
      break;
      
    case 3:
      if (Mode & TEMP_CS)
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN4_CS2_GPIO_Port, PAN4_CS2_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN4_CS2_GPIO_Port, PAN4_CS2_Pin, GPIO_PIN_RESET);
      }
      else
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN4_CS1_GPIO_Port, PAN4_CS1_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN4_CS1_GPIO_Port, PAN4_CS1_Pin, GPIO_PIN_RESET);
      }
      break;
      
    case 4:
      if (Mode & TEMP_CS)
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN5_CS2_GPIO_Port, PAN5_CS2_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN5_CS2_GPIO_Port, PAN5_CS2_Pin, GPIO_PIN_RESET);
      }
      else
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN5_CS1_GPIO_Port, PAN5_CS1_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN5_CS1_GPIO_Port, PAN5_CS1_Pin, GPIO_PIN_RESET);
      }
      break;
      
    case 5:
      if (Mode & TEMP_CS)
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN6_CS2_GPIO_Port, PAN6_CS2_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN6_CS2_GPIO_Port, PAN6_CS2_Pin, GPIO_PIN_RESET);
      }
      else
      {
        if (Mode & CS_OFF)
          HAL_GPIO_WritePin(PAN6_CS1_GPIO_Port, PAN6_CS1_Pin, GPIO_PIN_SET);
        else
          HAL_GPIO_WritePin(PAN6_CS1_GPIO_Port, PAN6_CS1_Pin, GPIO_PIN_RESET);
      }
      break;
        
    default:
      break;
  }
}

void StallDelay(void)
{
  for (uint32_t i = 0; i < 400; i++) __no_operation();
}


status_t ADIS16265_WriteReg8(uint8_t Address, uint8_t data, uint8_t Panel)
{
  uint16_t txdata;
  HAL_StatusTypeDef res;
  
  txdata = (0x8000 | ((uint16_t)Address << 8) | (uint16_t)data);
  
  SPI_SlaveSelect(GYRO_CS|CS_ON|Panel);
  res = HAL_SPI_Transmit(&hspi2, (uint8_t *)&txdata, 1, 10);
  SPI_SlaveSelect(GYRO_CS|CS_OFF|Panel);
  
  StallDelay();
  
  if (res != HAL_OK)
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}

status_t ADIS16265_WriteReg16(uint8_t Address, uint16_t data, uint8_t Panel)
{
  uint16_t txdata;
  HAL_StatusTypeDef res;
  
  txdata = (0x8000 | ((uint16_t)(Address+1) << 8) | (data>>8));
  
  SPI_SlaveSelect(GYRO_CS|CS_ON|Panel);
  res = HAL_SPI_Transmit(&hspi2, (uint8_t *)&txdata, 1, 10);
  SPI_SlaveSelect(GYRO_CS|CS_OFF|Panel);
  
  if (res != HAL_OK)
    return SEN_ERROR;
        
  StallDelay();
  
  txdata = (0x8000 | ((uint16_t)Address << 8) | (data & 0xFF));
  
  SPI_SlaveSelect(GYRO_CS|CS_ON|Panel);
  res = HAL_SPI_Transmit(&hspi2, (uint8_t *)&txdata, 1, 10);
  SPI_SlaveSelect(GYRO_CS|CS_OFF|Panel);
  
  if (res != HAL_OK)
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}

status_t ADIS16265_ReadReg16(uint8_t Address, uint16_t *data, uint8_t Panel)
{
  uint16_t txdata;    
  HAL_StatusTypeDef res = HAL_OK;
  
  if (data == NULL)
    return SEN_ERROR;
  
  txdata = (uint16_t)Address << 8;
  
  SPI_SlaveSelect(GYRO_CS|CS_ON|Panel);
  res = HAL_SPI_Transmit(&hspi2, (uint8_t *)&txdata, 1, 10);
  SPI_SlaveSelect(GYRO_CS|CS_OFF|Panel);
  
  if (res != HAL_OK)
    return SEN_ERROR;

  StallDelay();

  SPI_SlaveSelect(GYRO_CS|CS_ON|Panel);
  res = HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&txdata, (uint8_t *)data, 1, 10);
  SPI_SlaveSelect(GYRO_CS|CS_OFF|Panel);

  if (res != HAL_OK)
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}
          
status_t ADIS16265_Init(uint8_t Panel)
{
  uint16_t wTmp;
  
  osDelay(200);
  if (ADIS16265_ReadReg16(ADIS16260_PROD_ID, &wTmp, Panel) != SEN_SUCCESS)
    return SEN_ERROR;
  if (wTmp != ADIS16265_PROD_NUM)
    return SEN_ERROR;
  
  GyroStat |= (1 << Panel);
  
  osDelay(200);

  osDelay(200);
  
  for (uint8_t i = 0; i < 90; i+=2)
  {
    if ((i<0x06||i>0x09)&&(i<0x10||i>0x13)&&(i<0x18||i>0x1f)&&(i<0x2a||i>0x2f)&&(i<0x40||i>0x51))
    {
      if (SEN_SUCCESS != ADIS16265_ReadReg16(i, &wTmp, Panel)) continue;
;
    }
  }
  

  
  if (SEN_SUCCESS != ADIS16265_WriteReg16(ADIS16260_SENS_AVG, 0x0104, Panel))
    return SEN_ERROR;

  return SEN_SUCCESS;
}

status_t ADIS16265_GetAxesRate(AxesRaw_t* buff) 
{
  buff->AXIS_X = buff->AXIS_Y = buff->AXIS_Z = 0x7FFF;
  
  if (GyroStat & (1<< PAN_X_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_GYRO_OUT, (uint16_t *)(&buff->AXIS_X), PAN_X_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      *((uint16_t *)(&buff->AXIS_X)) <<= 2;
      buff->AXIS_X /= 4;
    }    
  }
  
  if (GyroStat & (1<< PAN_Y_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_GYRO_OUT, (uint16_t *)(&buff->AXIS_Y), PAN_Y_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      *((uint16_t *)(&buff->AXIS_Y)) <<= 2;
      buff->AXIS_Y /= 4;
    }
  }
  
  if (GyroStat & (1<< PAN_Z_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_GYRO_OUT, (uint16_t *)(&buff->AXIS_Z), PAN_Z_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      *((uint16_t *)(&buff->AXIS_Z)) <<= 2;
      buff->AXIS_Z /= 4;
    }
  }
  
  return SEN_SUCCESS;  
}

status_t ADIS16265_GetAxesAngle(AxesRaw_t* buff) 
{
  uint32_t uTmp;
  
  if (buff == NULL)
    return SEN_ERROR;
  
  buff->AXIS_X = buff->AXIS_Y = buff->AXIS_Z = 0x7FFF;
  
  if (GyroStat & (1<< PAN_X_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_ANGL_OUT, (uint16_t *)&uTmp, PAN_X_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      uTmp &= 0x3FFF;
      uTmp *= 3600;
      uTmp = (uTmp + ANGLE_UNITS_360/2) / ANGLE_UNITS_360;
      buff->AXIS_X = (int16_t)uTmp;
    }    
  }
  
  if (GyroStat & (1<< PAN_Y_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_ANGL_OUT, (uint16_t *)&uTmp, PAN_Y_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      uTmp &= 0x3FFF;
      uTmp *= 3600;
      uTmp = (uTmp + ANGLE_UNITS_360/2) / ANGLE_UNITS_360;
      buff->AXIS_Y = (int16_t)uTmp;
    }
  }
  
  if (GyroStat & (1<< PAN_Z_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_ANGL_OUT, (uint16_t *)&uTmp, PAN_Z_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      uTmp &= 0x3FFF;
      uTmp *= 3600;
      uTmp = (uTmp + ANGLE_UNITS_360/2) / ANGLE_UNITS_360;
      buff->AXIS_Z = (int16_t)uTmp;
    }
  }
  
  return SEN_SUCCESS;  
}

status_t ADIS16265_GetTemperature(Temperature_t* tmp) 
{
  int32_t iTmp;
  
  if (tmp == NULL)
    return SEN_ERROR;
  
  tmp->Temp_X = tmp->Temp_Y = tmp->Temp_Z = 0x7FFF;
  
  if (PanelStat & (1<< PAN_X_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_TEMP_OUT, (uint16_t *)(&tmp->Temp_X), PAN_X_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      *((uint16_t *)(&tmp->Temp_X)) <<= 4;
      tmp->Temp_X /= 16;      
      iTmp = tmp->Temp_X;
      iTmp *= 1453;
      if (iTmp > 0) iTmp += 500;
      else iTmp -= 500;
      iTmp /= 1000;
      iTmp += 250;
      tmp->Temp_X = iTmp;
    }    
  }
  
  if (PanelStat & (1<< PAN_Y_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_TEMP_OUT, (uint16_t *)(&tmp->Temp_Y), PAN_Y_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      *((uint16_t *)(&tmp->Temp_Y)) <<= 4;
      tmp->Temp_Y /= 16;      
      iTmp = tmp->Temp_Y;
      iTmp *= 1453;
      if (iTmp > 0) iTmp += 500;
      else iTmp -= 500;
      iTmp /= 1000;
      iTmp += 250;
      tmp->Temp_Y = iTmp;
    }    
  }
  
  if (PanelStat & (1<< PAN_Z_M))
  {
    if (ADIS16265_ReadReg16(ADIS16260_TEMP_OUT, (uint16_t *)(&tmp->Temp_Z), PAN_Z_M) != SEN_SUCCESS)
      return SEN_ERROR;
    else
    {
      *((uint16_t *)(&tmp->Temp_Z)) <<= 4;
      tmp->Temp_Z /= 16;      
      iTmp = tmp->Temp_Z;
      iTmp *= 1453;
      if (iTmp > 0) iTmp += 500;
      else iTmp -= 500;
      iTmp /= 1000;
      iTmp += 250;
      tmp->Temp_Z = iTmp;
    }    
  }
  
  return SEN_SUCCESS;  
}

status_t TMP122_GetTemperatureP(Temperature_t* tmp)
{
  uint16_t txdata = 0;
  HAL_StatusTypeDef res = HAL_OK;
  
  if (tmp == NULL)
    return SEN_ERROR;
  
  tmp->Temp_X = tmp->Temp_Y = tmp->Temp_Z = 0xFFFF;

  SPI_SlaveSelect(TEMP_CS|CS_ON|PAN_X_P);
  res = HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&txdata, (uint8_t *)(&tmp->Temp_X), 1, 10);
  SPI_SlaveSelect(TEMP_CS|CS_OFF|PAN_X_P);
  if (res != HAL_OK)
    return SEN_ERROR;
  
  SPI_SlaveSelect(TEMP_CS|CS_ON|PAN_Y_P);
  res = HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&txdata, (uint8_t *)(&tmp->Temp_Y), 1, 10);
  SPI_SlaveSelect(TEMP_CS|CS_OFF|PAN_Y_P);
  if (res != HAL_OK)
    return SEN_ERROR;
  
  SPI_SlaveSelect(TEMP_CS|CS_ON|PAN_Z_P);
  res = HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&txdata, (uint8_t *)(&tmp->Temp_Z), 1, 10);
  SPI_SlaveSelect(TEMP_CS|CS_OFF|PAN_Z_P);
  if (res != HAL_OK)
    return SEN_ERROR;

  return SEN_SUCCESS;
}

status_t TMP122_GetTemperatureM(Temperature_t* tmp)
{
  uint16_t txdata = 0;
  HAL_StatusTypeDef res = HAL_OK;
  
  if (tmp == NULL)
    return SEN_ERROR;
  
  tmp->Temp_X = tmp->Temp_Y = tmp->Temp_Z = 0xFFFF;

  SPI_SlaveSelect(TEMP_CS|CS_ON|PAN_X_M);
  res = HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&txdata, (uint8_t *)(&tmp->Temp_X), 1, 10);
  SPI_SlaveSelect(TEMP_CS|CS_OFF|PAN_X_M);
  if (res != HAL_OK)
    return SEN_ERROR;
  
  SPI_SlaveSelect(TEMP_CS|CS_ON|PAN_Y_M);
  res = HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&txdata, (uint8_t *)(&tmp->Temp_Y), 1, 10);
  SPI_SlaveSelect(TEMP_CS|CS_OFF|PAN_Y_M);
  if (res != HAL_OK)
    return SEN_ERROR;
  
  SPI_SlaveSelect(TEMP_CS|CS_ON|PAN_Z_M);
  res = HAL_SPI_TransmitReceive(&hspi2, (uint8_t *)&txdata, (uint8_t *)(&tmp->Temp_Z), 1, 10);
  SPI_SlaveSelect(TEMP_CS|CS_OFF|PAN_Z_M);
  if (res != HAL_OK)
    return SEN_ERROR;

  return SEN_SUCCESS;
}

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal routines definition */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
* File Name          : ais328dq_driver.c
* Author             : MSH Application Team
* Author             : Abhishek Anand	
* Version            : $Revision:$
* Date               : $Date:$
* Description        : AIS328DQ driver file
*                      
* HISTORY:
* Date               |	Modification                    |	Author
* 16/05/2012         |	Initial Revision                |	Abhishek Anand
* 17/05/2012         |  Modified to support multiple drivers in the same program                |	Abhishek Anand

********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "ais328dq_driver.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "MCU_Init.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

status_t AIS328DQ_Init(uint8_t deviceAddress)
{
  uint8_t val;
  
  //set ODR (turn ON device)
  if (AIS328DQ_SetODR(deviceAddress, AIS328DQ_ODR_100Hz) != SEN_SUCCESS)
    return SEN_ERROR;
  //else fprintf(CON,"\r\n[1]SET_ODR_OK\n\r");
  
  //get sensor ID
  if (AIS328DQ_GetWHO_AM_I(deviceAddress, &val) != SEN_SUCCESS)
    return SEN_ERROR;
  //else fprintf(CON,"[1]GET_WHOAMI_OK=%02X\n\r", val[0]);
  
  //set PowerMode
  if (AIS328DQ_SetMode(deviceAddress, AIS328DQ_NORMAL) != SEN_SUCCESS)
    return SEN_ERROR;
  //else fprintf(CON,"[1]SET_MODE_OK\n\r");  
  
  //set Fullscale
  if (AIS328DQ_SetFullScale(deviceAddress, AIS328DQ_FULLSCALE_2) != SEN_SUCCESS)
    return SEN_ERROR;    
  //else fprintf(CON,"[1]SET_FULLSCALE_OK\n\r");
  
  //set axis Enable
  if (AIS328DQ_SetAxis(deviceAddress, AIS328DQ_X_ENABLE | AIS328DQ_Y_ENABLE |  AIS328DQ_Z_ENABLE) != SEN_SUCCESS)
    return SEN_ERROR;    
  //else fprintf(CON,"[1]SET_AXIS_OK\n\r");
  
  return SEN_SUCCESS;
}

/*******************************************************************************
* Function Name		: AIS328DQ_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions					
* Input			: Register Address
* Output		: Data Read
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
uint8_t AIS328DQ_ReadReg(uint8_t deviceAddr, uint8_t Reg, uint8_t* Data)
{
  if (HAL_OK != HAL_I2C_Mem_Read(&hi2c2, deviceAddr, Reg, 1, Data, 1, 10))
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name		: AIS328DQ_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*			: I2C or SPI writing function
* Input			: Register Address, Data to be written
* Output		: None
* Return		: Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
uint8_t AIS328DQ_WriteReg(uint8_t deviceAddress, uint8_t WriteAddr, uint8_t Data)
{
  if (HAL_OK != HAL_I2C_Mem_Write(&hi2c2, deviceAddress, WriteAddr, 1, &Data, 1, 10))
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : AIS328DQ_GetWHO_AM_I
* Description    : Read identification code from AIS328DQ_WHO_AM_I register
* Input          : char to be filled with the Device identification Value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
status_t AIS328DQ_GetWHO_AM_I(uint8_t deviceAddress, uint8_t* val){
  
  if( !AIS328DQ_ReadReg(deviceAddress, AIS328DQ_WHO_AM_I, val) )
    return SEN_ERROR;
  
  if (*val != AIS328DQ_WHO_AM_I_VAL)
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetODR
* Description    : Sets AIS328DQ Accelerometer Output Data Rate 
* Input          : Output Data Rate
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetODR(uint8_t deviceAddress, AIS328DQ_ODR_t dr){
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(deviceAddress, AIS328DQ_CTRL_REG1, &value) )
    return SEN_ERROR;
  
  value &= 0xE7;
  value |= dr<<AIS328DQ_DR;
  
  if( !AIS328DQ_WriteReg(deviceAddress, AIS328DQ_CTRL_REG1, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetMode
* Description    : Sets AIS328DQ Accelerometer Operating Mode
* Input          : Modality (AIS328DQ_LOW_POWER, AIS328DQ_NORMAL, AIS328DQ_POWER_DOWN...)
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetMode(uint8_t deviceAddress, AIS328DQ_Mode_t pm) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(deviceAddress, AIS328DQ_CTRL_REG1, &value) )
    return SEN_ERROR;
  
  value &= 0x1F;
  value |= (pm<<AIS328DQ_PM);   
  
  if( !AIS328DQ_WriteReg(deviceAddress, AIS328DQ_CTRL_REG1, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}

/*******************************************************************************
* Function Name  : AIS328DQ_SetAxis
* Description    : Enable/Disable AIS328DQ Axis
* Input          : AIS328DQ_X_ENABLE/AIS328DQ_X_DISABLE | AIS328DQ_Y_ENABLE/AIS328DQ_Y_DISABLE
                   | AIS328DQ_Z_ENABLE/AIS328DQ_Z_DISABLE
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetAxis(uint8_t deviceAddress, AIS328DQ_Axis_t axis) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(deviceAddress, AIS328DQ_CTRL_REG1, &value) )
    return SEN_ERROR;
  
  value &= 0xF8;
  value |= (0x07 & axis);
  
  if( !AIS328DQ_WriteReg(deviceAddress, AIS328DQ_CTRL_REG1, value) )
    return SEN_ERROR;   
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetFullScale
* Description    : Sets the AIS328DQ FullScale
* Input          : AIS328DQ_FULLSCALE_2/AIS328DQ_FULLSCALE_4/AIS328DQ_FULLSCALE_8
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetFullScale(uint8_t deviceAddress, AIS328DQ_Fullscale_t fs) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(deviceAddress, AIS328DQ_CTRL_REG4, &value) )
    return SEN_ERROR;
  
  value &= 0xCF;	
  value |= (fs<<AIS328DQ_FS);
  
  if( !AIS328DQ_WriteReg(deviceAddress, AIS328DQ_CTRL_REG4, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetBDU(State_t bdu) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return SEN_ERROR;
  
  value &= 0x7F;
  value |= (bdu<<AIS328DQ_BDU);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : AIS328DQ_BLE_LSB / AIS328DQ_BLE_MSB
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetBLE(AIS328DQ_Endianess_t ble) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return SEN_ERROR;
  
  value &= 0xBF;	
  value |= (ble<<AIS328DQ_BLE);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetFDS
* Description    : Set Filter Data Selection
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetFDS(State_t fds) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return SEN_ERROR;
  
  value &= 0xEF;	
  value |= (fds<<AIS328DQ_FDS);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetBOOT
* Description    : Rebot memory content
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetBOOT(State_t boot) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return SEN_ERROR;
  
  value &= 0x7F;	
  value |= (boot<<AIS328DQ_BOOT);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetSelfTest
* Description    : Set Self Test Modality
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetSelfTest(State_t st) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return SEN_ERROR;
  
  value &= 0xFD;
  value |= (st<<AIS328DQ_ST);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetSelfTestSign
* Description    : Set Self Test Sign (Disable = st_plus, Enable = st_minus)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetSelfTestSign(State_t st_sign) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return SEN_ERROR;
  
  value &= 0xF7;
  value |= (st_sign<<AIS328DQ_ST_SIGN);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetIntHighLow
* Description    : Set Interrupt active state (Disable = active high, Enable = active low)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetIntHighLow(State_t ihl) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return SEN_ERROR;
  
  value &= 0x7F;
  value |= (ihl<<AIS328DQ_IHL);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetIntPPOD
* Description    : Set Interrupt Push-Pull/OpenDrain Pad (Disable = Push-Pull, Enable = OpenDrain)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetIntPPOD(State_t pp_od) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return SEN_ERROR;
  
  value &= 0xBF;
  value |= (pp_od<<AIS328DQ_PP_OD);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1DataSign
* Description    : Set Data signal Interrupt 1 pad
* Input          : Modality by AIS328DQ_INT_Conf_t Typedef 
                  (AIS328DQ_INT_SOURCE, AIS328DQ_INT_1OR2_SOURCE, AIS328DQ_DATA_READY, AIS328DQ_BOOT_RUNNING)
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt1DataSign(AIS328DQ_INT_Conf_t i_cfg) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return SEN_ERROR;
  
  value &= 0xFC;
  value |= (i_cfg<<AIS328DQ_I1_CFG);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2DataSign
* Description    : Set Data signal Interrupt 2 pad
* Input          : Modality by AIS328DQ_INT_Conf_t Typedef 
                  (AIS328DQ_INT_SOURCE, AIS328DQ_INT_1OR2_SOURCE, AIS328DQ_DATA_READY, AIS328DQ_BOOT_RUNNING)
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt2DataSign(AIS328DQ_INT_Conf_t i_cfg) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return SEN_ERROR;
  
  value &= 0xE7;
  value |= (i_cfg<<AIS328DQ_I2_CFG);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetSPI34Wire
* Description    : Set SPI mode 
* Input          : Modality by AIS328DQ_SPIMode_t Typedef (AIS328DQ_SPI_4_WIRE, AIS328DQ_SPI_3_WIRE)
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetSPI34Wire(AIS328DQ_SPIMode_t sim) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, &value) )
    return SEN_ERROR;
  
  value &= 0xFE;
  value |= (sim<<AIS328DQ_SIM);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG4, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_TurnONEnable
* Description    : TurnON Mode selection for sleep to wake function
* Input          : AIS328DQ_SLEEP_TO_WAKE_DIS/AIS328DQ_SLEEP_TO_WAKE_ENA
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_TurnONEnable(AIS328DQ_Sleep_To_Wake_Conf_t stw) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG5, &value) )
    return SEN_ERROR;
  
  value &= 0x00;
  value |= (stw<<AIS328DQ_TURN_ON);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG5, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_HPFilterReset
* Description    : Reading register for reset the content of internal HP filter
* Input          : None
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_HPFilterReset(void) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_HP_FILTER_RESET, &value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetReference
* Description    : Sets Reference register acceleration value as a reference for HP filter
* Input          : Value of reference acceleration value (0-255)
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetReference(int8_t ref) {
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_REFERENCE_REG, ref) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : AIS328DQ_HPM_NORMAL_MODE_RES/AIS328DQ_HPM_REF_SIGNAL/AIS328DQ_HPM_NORMAL_MODE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetHPFMode(AIS328DQ_HPFMode_t hpm) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return SEN_ERROR;
  
  value &= 0x9F;
  value |= (hpm<<AIS328DQ_HPM);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : AIS328DQ_HPFCF [0,3]
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetHPFCutOFF(AIS328DQ_HPFCutOffFreq_t hpf) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return SEN_ERROR;
  
  value &= 0xFC;
  value |= (hpf<<AIS328DQ_HPCF);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
  
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2HPEnable
* Description    : Set Interrupt2 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : AIS328DQ_SetInt2HPEnable(MEMS_ENABLE)
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt2HPEnable(State_t stat) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return SEN_ERROR;
  
  value &= 0xF7;
  value |= stat<<AIS328DQ_HPEN2 ;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}     


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1HPEnable
* Description    : Set Interrupt1 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : AIS328DQ_SetInt1HPEnable(MEMS_ENABLE)
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt1HPEnable(State_t stat) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, &value) )
    return SEN_ERROR;
  
  value &= 0xFB;
  value |= stat<<AIS328DQ_HPEN1 ;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG2, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}  


/*******************************************************************************
* Function Name  : AIS328DQ_Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_Int1LatchEnable(State_t latch) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return SEN_ERROR;
  
  value &= 0xFB;
  value |= latch<<AIS328DQ_LIR1;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_Int2LatchEnable
* Description    : Enable Interrupt 2 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_Int2LatchEnable(State_t latch) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, &value) )
    return SEN_ERROR;
  
  value &= 0xDF;
  value |= latch<<AIS328DQ_LIR2;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_CTRL_REG3, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_ResetInt1Latch(void) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, &value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_ResetInt2Latch
* Description    : Reset Interrupt 2 Latching function
* Input          : None
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_ResetInt2Latch(void) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, &value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Configuration
* Description    : Interrupt 1 Configuration (without 6D_INT)
* Input          : AIS328DQ_INT_AND/OR | AIS328DQ_INT_ZHIE_ENABLE/DISABLE | AIS328DQ_INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use ALL input variable in the argument, as in example above
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt1Configuration(AIS328DQ_IntConf_t ic) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, &value) )
    return SEN_ERROR;
  
  value &= 0x40; 
  value |= ic;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Configuration
* Description    : Interrupt 2 Configuration (without 6D_INT)
* Input          : AIS328DQ_INT_AND/OR | AIS328DQ_INT_ZHIE_ENABLE/DISABLE | AIS328DQ_INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt2Configuration(AIS328DQ_IntConf_t ic) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, &value) )
    return SEN_ERROR;
  
  value &= 0x40; 
  value |= ic;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Mode
* Description    : Interrupt 1 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : AIS328DQ_INT_MODE_OR, AIS328DQ_INT_MODE_6D_MOVEMENT, AIS328DQ_INT_MODE_AND, AIS328DQ_INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt1Mode(AIS328DQ_IntMode_t int_mode) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, &value) )
    return SEN_ERROR;
  
  value &= 0x3F; 
  value |= (int_mode<<AIS328DQ_INT_6D);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_CFG, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Mode
* Description    : Interrupt 2 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : AIS328DQ_INT_MODE_OR, AIS328DQ_INT_MODE_6D_MOVEMENT, AIS328DQ_INT_MODE_AND, AIS328DQ_INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt2Mode(AIS328DQ_IntMode_t int_mode) {
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, &value) )
    return SEN_ERROR;
  
  value &= 0x3F; 
  value |= (int_mode<<AIS328DQ_INT_6D);
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_CFG, value) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_Get6DPositionInt1
* Description    : 6D Interrupt 1 Position Detect
* Input          : Byte to be filled with AIS328DQ_POSITION_6D_t Typedef
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_Get6DPositionInt1(uint8_t* val){
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, &value) )
    return SEN_ERROR;
  
  value &= 0x7F;
  
  switch (value){
  case AIS328DQ_UP_SX:   
    *val = AIS328DQ_UP_SX;    
    break;
  case AIS328DQ_UP_DX:   
    *val = AIS328DQ_UP_DX;    
    break;
  case AIS328DQ_DW_SX:   
    *val = AIS328DQ_DW_SX;    
    break;
  case AIS328DQ_DW_DX:   
    *val = AIS328DQ_DW_DX;    
    break;
  case AIS328DQ_TOP:     
    *val = AIS328DQ_TOP;      
    break;
  case AIS328DQ_BOTTOM:  
    *val = AIS328DQ_BOTTOM;  
    break;
  }
  
  return SEN_SUCCESS;  
}


/*******************************************************************************
* Function Name  : AIS328DQ_Get6DPositionInt2
* Description    : 6D Interrupt 2 Position Detect
* Input          : Byte to be filled with AIS328DQ_POSITION_6D_t Typedef
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_Get6DPositionInt2(uint8_t* val){
  uint8_t value;
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, &value) )
    return SEN_ERROR;
  
  value &= 0x7F;
  
  switch (value){
  case AIS328DQ_UP_SX:   
    *val = AIS328DQ_UP_SX;    
    break;
  case AIS328DQ_UP_DX:   
    *val = AIS328DQ_UP_DX;    
    break;
  case AIS328DQ_DW_SX:   
    *val = AIS328DQ_DW_SX;    
    break;
  case AIS328DQ_DW_DX:   
    *val = AIS328DQ_DW_DX;    
    break;
  case AIS328DQ_TOP:     
    *val = AIS328DQ_TOP;      
    break;
  case AIS328DQ_BOTTOM:  
    *val = AIS328DQ_BOTTOM;   
    break;
  }
  
  return SEN_SUCCESS;  
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,127]
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt1Threshold(uint8_t ths) {
  if (ths > 127)
    return SEN_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_THS, ths) )
    return SEN_ERROR;    
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration = [0,127]
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt1Duration(uint8_t id) {  
  if (id > 127)
    return SEN_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_DURATION, id) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Threshold
* Description    : Sets Interrupt 2 Threshold
* Input          : Threshold = [0,127]
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt2Threshold(uint8_t ths) {
  if (ths > 127)
    return SEN_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_THS, ths) )
    return SEN_ERROR;    
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_SetInt2Duration
* Description    : Sets Interrupt 2 Duration
* Input          : Duration = [0,127]
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_SetInt2Duration(uint8_t id) {  
  if (id > 127)
    return SEN_ERROR;
  
  if( !AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_DURATION, id) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetStatusReg
* Description    : Read the status register
* Input          : char to empty by Status Reg Value
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_GetStatusReg(uint8_t* val) {
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_STATUS_REG, val) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;  
}

/*******************************************************************************
* Function Name  : AIS328DQ_GetStatusBIT
* Description    : Read the status register BIT
* Input          : AIS328DQ_STATUS_REG_ZYXOR, AIS328DQ_STATUS_REG_ZOR, AIS328DQ_STATUS_REG_YOR, AIS328DQ_STATUS_REG_XOR,
                   AIS328DQ_STATUS_REG_ZYXDA, AIS328DQ_STATUS_REG_ZDA, AIS328DQ_STATUS_REG_YDA, AIS328DQ_STATUS_REG_XDA, 
                   AIS328DQ_DATAREADY_BIT
* Output         : status register BIT
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_GetStatusBit(uint8_t statusBIT, uint8_t *val) {
  uint8_t value;  
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_STATUS_REG, &value) )
    return SEN_ERROR;
  
  switch (statusBIT){
  case AIS328DQ_STATUS_REG_ZYXOR:     
    if(value &= AIS328DQ_STATUS_REG_ZYXOR){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }  
  case AIS328DQ_STATUS_REG_ZOR:       
    if(value &= AIS328DQ_STATUS_REG_ZOR){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }  
  case AIS328DQ_STATUS_REG_YOR:       
    if(value &= AIS328DQ_STATUS_REG_YOR){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }                                 
  case AIS328DQ_STATUS_REG_XOR:       
    if(value &= AIS328DQ_STATUS_REG_XOR){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }  
  case AIS328DQ_STATUS_REG_ZYXDA:     
    if(value &= AIS328DQ_STATUS_REG_ZYXDA){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  case AIS328DQ_STATUS_REG_ZDA:       
    if(value &= AIS328DQ_STATUS_REG_ZDA){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  case AIS328DQ_STATUS_REG_YDA:       
    if(value &= AIS328DQ_STATUS_REG_YDA){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  case AIS328DQ_STATUS_REG_XDA:       
    if(value &= AIS328DQ_STATUS_REG_XDA){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }                                      
  }
  
  return SEN_ERROR;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetAccAxesRaw
* Description    : Read the Acceleration Values Output Registers
* Input          : buffer to empity by AccAxesRaw_t Typedef
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_GetAccAxesRaw(uint8_t deviceAddr, AxesRaw_t* buff) {
  int16_t value;
  uint8_t *valueL = (uint8_t *)(&value);
  uint8_t *valueH = ((uint8_t *)(&value)+1);
  
  if( !AIS328DQ_ReadReg(deviceAddr, AIS328DQ_OUT_X_L, valueL) )
    return SEN_ERROR;
  
  if( !AIS328DQ_ReadReg(deviceAddr, AIS328DQ_OUT_X_H, valueH) )
    return SEN_ERROR;
  
  buff->AXIS_X = value/16;
  
  if( !AIS328DQ_ReadReg(deviceAddr, AIS328DQ_OUT_Y_L, valueL) )
    return SEN_ERROR;
  
  if( !AIS328DQ_ReadReg(deviceAddr, AIS328DQ_OUT_Y_H, valueH) )
    return SEN_ERROR;
  
  buff->AXIS_Y = value/16;
  
  if( !AIS328DQ_ReadReg(deviceAddr, AIS328DQ_OUT_Z_L, valueL) )
    return SEN_ERROR;
  
  if( !AIS328DQ_ReadReg(deviceAddr, AIS328DQ_OUT_Z_H, valueH) )
    return SEN_ERROR;
  
  buff->AXIS_Z = value/16;
  
  return SEN_SUCCESS;  
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : buffer to empty by Int1 Source Value
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_GetInt1Src(uint8_t* val) {  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, val) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetInt2Src
* Description    : Reset Interrupt 2 Latching function
* Input          : buffer to empty by Int2 Source Value
* Output         : None
* Return         : Status [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_GetInt2Src(uint8_t* val) {  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, val) )
    return SEN_ERROR;
  
  return SEN_SUCCESS;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetInt1SrcBit
* Description    : Reset Interrupt 1 Latching function
* Input          : AIS328DQ_INT1_SRC_IA, AIS328DQ_INT1_SRC_ZH, AIS328DQ_INT1_SRC_ZL .....
* Output         : None
* Return         : Status of BIT [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_GetInt1SrcBit(uint8_t statusBIT, uint8_t *val) {
  uint8_t value;  
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT1_SRC, &value) )
    return SEN_ERROR;
  
  if(statusBIT == AIS328DQ_INT_SRC_IA){
    if(value &= AIS328DQ_INT_SRC_IA){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }    
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZH){
    if(value &= AIS328DQ_INT_SRC_ZH){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZL){
    if(value &= AIS328DQ_INT_SRC_ZL){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YH){
    if(value &= AIS328DQ_INT_SRC_YH){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YL){
    if(value &= AIS328DQ_INT_SRC_YL){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XH){
    if(value &= AIS328DQ_INT_SRC_XH){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XL){
    if(value &= AIS328DQ_INT_SRC_XL){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }    
  } 
  return SEN_ERROR;
}


/*******************************************************************************
* Function Name  : AIS328DQ_GetInt2SrcBit
* Description    : Reset Interrupt 2 Latching function
* Input          : AIS328DQ_INT_SRC_IA, AIS328DQ_INT_SRC_ZH, AIS328DQ_INT_SRC_ZL .....
* Output         : None
* Return         : Status of BIT [SEN_ERROR, SEN_SUCCESS]
*******************************************************************************/
status_t AIS328DQ_GetInt2SrcBit(uint8_t statusBIT, uint8_t *val) {
  uint8_t value;  
  
  if( !AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, AIS328DQ_INT2_SRC, &value) )
    return SEN_ERROR;
  
  if(statusBIT == AIS328DQ_INT_SRC_IA){
    if(value &= AIS328DQ_INT_SRC_IA){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZH){
    if(value &= AIS328DQ_INT_SRC_ZH){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_ZL){
    if(value &= AIS328DQ_INT_SRC_ZL){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YH){
    if(value &= AIS328DQ_INT_SRC_YH){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }    
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_YL){
    if(value &= AIS328DQ_INT_SRC_YL){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }   
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XH){
    if(value &= AIS328DQ_INT_SRC_XH){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }  
  }
  
  if(statusBIT == AIS328DQ_INT_SRC_XL){
    if(value &= AIS328DQ_INT_SRC_XL){     
      *val = MEMS_SET;
      return SEN_SUCCESS;
    }
    else{  
      *val = MEMS_RESET;
      return SEN_SUCCESS;
    }    
  } 
  return SEN_ERROR;
}
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/

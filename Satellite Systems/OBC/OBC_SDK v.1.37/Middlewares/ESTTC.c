/*!
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* @file ESTTC.c
* @brief EnduroSat telemetry and telecommand communication protocol
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
#include "ESTTC.h"
#include "MCU_Init.h"
#include "fatfs.h"
#include "panels.h"
#include "version.h"
#include "DAT_Inputs.h"


/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL DEFINES
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* No Internal defines */

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
volatile char RxBuffer[ESTTC_INTERFACE_NUMBER][UART_BUFFER_SIZE];
volatile uint32_t RxBuffHead[ESTTC_INTERFACE_NUMBER], RxBuffTail[ESTTC_INTERFACE_NUMBER], RxBuffLen[ESTTC_INTERFACE_NUMBER];

FIL df[ESTTC_INTERFACE_NUMBER];
DIR dd[ESTTC_INTERFACE_NUMBER];

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) VARIABLES DEFINITION/DECLARATION 
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static FATFS SD_Fs;
static char txline[ESTTC_INTERFACE_NUMBER][LINE_BUFFER_SIZE];
static char rxline[ESTTC_INTERFACE_NUMBER][LINE_BUFFER_SIZE];
static const FILE * Esttc_usart_interfaces[ESTTC_INTERFACE_NUMBER] = {COM1, COM4, COM6};
static uint32_t fpos[ESTTC_INTERFACE_NUMBER], bsize[ESTTC_INTERFACE_NUMBER];
static FILINFO fno[ESTTC_INTERFACE_NUMBER];
static uint16_t pack_data_position[ESTTC_INTERFACE_NUMBER] = {0,0,0};

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DECLARATION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static uint8_t FLASH_Write(uint8_t *src, uint32_t addr, uint32_t len);
static uint8_t FLASH_PatternCheck(uint32_t addr, uint8_t pattern, uint32_t len);
static uint32_t GetPhrase(char *dst, uint32_t len, char term, ESTTC_InterfacesEnum Interface);
static int getbyte(uint32_t tmt_ms, ESTTC_InterfacesEnum Interface);
static uint8_t HexToBin(uint8_t hb, uint8_t lb);
static uint8_t ESTTC_ProcessData(ESTTC_InterfacesEnum Interface);
static void APP_InitCdCardFiles(void);

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* EXTERNAL (NONE STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
void ESTTC_InitTask(void)
{
    uint8_t u8_index;
    uint32_t br;

    for( u8_index = 0; u8_index < ESTTC_INTERFACE_NUMBER ; u8_index ++ )
    {
        df[u8_index].fs = 0;

        RxBuffHead[u8_index] = 0;
        RxBuffTail[u8_index] = 0;
        RxBuffLen[u8_index] = 0;

        fpos[u8_index] = 0;
        bsize[u8_index] = 0;
    }


    osThreadDef(myESTTC_UART_COMM, ESTTC_UART_COMM, osPriorityLow, 1, 5*128);
    osThreadCreate(osThread(myESTTC_UART_COMM), NULL);

    /* Start reception using interrupts through USART */
    HAL_UART_Receive_IT((UART_HandleTypeDef*)COMM, (uint8_t *)&br, 1);
    HAL_UART_Receive_IT((UART_HandleTypeDef*)SYSCON, (uint8_t *)&br, 1);
    HAL_UART_Receive_IT((UART_HandleTypeDef*)PAYLOAD, (uint8_t *)&br, 1);
}

static  uint32_t rx_data_dummy;
void ESTTC_UART_COMM(void const * argument)
{
  uint8_t ProcessedPacket = 0;

  APP_InitCdCardFiles();


  for(;;)
  {
      ProcessedPacket = ProcessedPacket | ESTTC_ProcessData(ESTTC_COMM_INTEFACE);

      ProcessedPacket = ProcessedPacket | ESTTC_ProcessData(ESTTC_PAYLOAD_INTEFACE );

      ProcessedPacket = ProcessedPacket | ESTTC_ProcessData(ESTTC_SYSCOMM_INTEFACE );

	  if ( ProcessedPacket == 0 )  /* if there were no packets to process */
	  {
	      /* give time for the other processes */
	      ProcessedPacket = 0;
	      osDelay(10);
	  }
  }
}

static uint8_t ESTTC_ProcessData(ESTTC_InterfacesEnum Interface)
{
    char *begin;
    uint32_t len;
    uint32_t i, j;
    uint16_t utmp;
    uint32_t br;
    Compass_Axis_t B_raw;
    AxesRaw_t data;
    Temperature_t tmperature_sens;
    int16_t tp;
    char s8_tmp;
    FRESULT fr;

    FLASH_EraseInitTypeDef fes;
    FILE * ComInterface;
    uint32_t se;

    uint8_t ProcessedPacket = 0;

    if( Interface >= ESTTC_INTERFACE_NUMBER )
    {
        Error_Handler();
    }

    if(GetPhrase(rxline[Interface], LINE_BUFFER_SIZE-1, '\r', Interface))
    {
      ComInterface = (FILE *)Esttc_usart_interfaces[Interface];

      begin = strstr(rxline[Interface], "ES+");
      if(begin != NULL)
      {
        ProcessedPacket = 1;

        len = strlen(begin);
        if( len <= LINE_BUFFER_SIZE)
        {
          strcpy(txline[Interface], begin);
          for (i = 4, j = 4; i < len; i+=2, j++)
          {
              txline[Interface][j] = HexToBin(txline[Interface][i], txline[Interface][i+1]);
          }

          if (txline[Interface][3] == 'R')
          {
            if (txline[Interface][4] == OBC_I2C_ADDRESS)
            {
              switch(txline[Interface][5])
              {
                case ESTTC_CMD_ACCSEL_1_DATA: //0x00:
                  if (AIS328DQ_GetAccAxesRaw(AIS328DQ_1_MEMS_I2C_ADDRESS, &data) == SEN_SUCCESS) { 
                    fprintf(ComInterface, "OK+1%d/%d/%d\r", data.AXIS_X, data.AXIS_Y, data.AXIS_Z);
                    fprintf(ComInterface, "Accelerometer 1 X=%d Y=%d Z=%d\r", data.AXIS_X, data.AXIS_Y, data.AXIS_Z);
                  }
                  else {
                    I2C2_Reset();                    
                    fprintf(ComInterface, "ERR - Accelerometer 1 fail - I2C bus restart is performed, please try command again\r  ");
                  }
                  break;
                case ESTTC_CMD_ACCSEL_1_ACESS:	//0x01
                  if(SEN_SUCCESS == AIS328DQ_ReadReg(AIS328DQ_1_MEMS_I2C_ADDRESS, txline[Interface][6], (uint8_t *)&txline[Interface][7])) {
                    fprintf(ComInterface, "OK+1%02X/%02X\r", txline[Interface][6], txline[Interface][7]);
                    fprintf(ComInterface, "Accelerometer 1 register %X has value %X\r", txline[Interface][6], txline[Interface][7]);
                  }
                  else
                    fprintf(ComInterface, "ERR - Not valid parameters!\r");
                  break;
                  
                case ESTTC_CMD_ACCSEL_2_DATA: //0x02:
                  if (AIS328DQ_GetAccAxesRaw(AIS328DQ_2_MEMS_I2C_ADDRESS, &data) == SEN_SUCCESS) { 
                    fprintf(ComInterface, "OK+2%d/%d/%d\r", data.AXIS_X, data.AXIS_Y, data.AXIS_Z);
                    fprintf(ComInterface, "Accelerometer 2 X=%d Y=%d Z=%d\r", data.AXIS_X, data.AXIS_Y, data.AXIS_Z);
                  }
                  else {
                    I2C2_Reset();  
                    fprintf(ComInterface, "Accelerometer 2 fail - I2C bus restart is performed, please try command again\r  ");
                  } 
                  break;                  

                case ESTTC_CMD_ACCSEL_2_ACESS: //0x03:
                  if(SEN_SUCCESS == AIS328DQ_ReadReg(AIS328DQ_2_MEMS_I2C_ADDRESS, txline[Interface][6], (uint8_t *)&txline[Interface][7])) {
                    fprintf(ComInterface, "OK+2%02X/%02X\r", txline[Interface][6], txline[Interface][7]);
                    fprintf(ComInterface, "Accelerometer 2 register %X, has value %X\r", txline[Interface][6], txline[Interface][7]);
                  }
                  else
                    fprintf(ComInterface, "ERR - Not valid parameters!\r");
                  break;
                  
                case ESTTC_CMD_MAG1_DATA: //0x04:
                #ifdef HMC5883                  
                        if (Magnitometers_Read_Data (&B_raw, E_810uT) != E_OK) {
                          I2C2_Reset();                          
                          fprintf(ComInterface, "ERR exe\r");
                        }
                        else {  
                    fprintf(ComInterface, "OK+1%1.0f/%01.0f/%1.0f\r", B_raw.AXIS_X, B_raw.AXIS_Y, B_raw.AXIS_Z);
                        fprintf(ComInterface, "Magnetometer 1 - Magnetic field in specific rande X=%1.0f Y=%1.0f Z=%1.0f\r", B_raw.AXIS_X, B_raw.AXIS_Y, B_raw.AXIS_Z);
                        }
                #else   
                      if (Magnitometers_LIS3MDL_Read_Data(&B_raw, LIS3MDL_MAG_I2C_ADDRESS_LOW) != E_OK) {
                        I2C2_Reset();
                        fprintf(ComInterface, "ERR exe\r");
                      }
                      else    {  
                        fprintf(ComInterface, "OK+1%1.0f/%01.0f/%1.0f\r", B_raw.AXIS_X, B_raw.AXIS_Y, B_raw.AXIS_Z);
                        fprintf(ComInterface, "Magnetometer 1 - Magnetic field in specific rande X=%1.0f Y=%1.0f Z=%1.0f\r", B_raw.AXIS_X, B_raw.AXIS_Y, B_raw.AXIS_Z);
                      }  
                #endif        
                  break;                  

                case ESTTC_CMD_MGA1_ACESS: //0x05:
                #ifdef HMC5883                     
                  if (SEN_SUCCESS == HMC5883L_ReadReg(txline[Interface][7], (uint8_t *)&txline[Interface][9], txline[Interface][8]))
                  {
                    fprintf(ComInterface, "OK+");
                    for (i = 0; i < txline[Interface][8]; i++) fprintf(ComInterface, "%02X", txline[Interface][9+i]);
                    fprintf(ComInterface, "\r");
                  }
                #else                   
                  if (SEN_SUCCESS == LIS3MDL_MAG_ReadReg(LIS3MDL_MAG_I2C_ADDRESS_LOW, txline[Interface][6], (uint8_t *) &txline[Interface][7])) {
                    fprintf(ComInterface, "OK+1%02X/%02X\r", txline[Interface][6], txline[Interface][7]);
                    fprintf(ComInterface, "Magnetometer 1 reg No %d has value %d\r", txline[Interface][6], txline[Interface][7]);
                  }
                #endif  
                  else
                    fprintf(ComInterface, "ERR exe\r");
                  break;
                  
                case ESTTC_CMD_MAG2_DATA: //0x06:
                #ifdef HMC5883   
                        Magnitometers_Init (E_SINGLE,
                                          E_NORMAL,
                                          E_COMBINED,
                                          E_810uT,
                                          E_30HZ,
                                          E_MEAS_AVG_8);                  
                      if (Magnitometers_Read_Data (&B_raw, E_810uT) != E_OK) fprintf(ComInterface, "ERR exe\r");
                      else fprintf(ComInterface, "Magnetometer 2 -  Magnetic field in specific rande X=%1.0f Y=%1.0f Z=%1.0f\r", B_raw.AXIS_X, B_raw.AXIS_Y, B_raw.AXIS_Z);
                #else   
                      Magnitometers_LIS3MDL_Init(LIS3MDL_MAG_I2C_ADDRESS_HIGH);
                      if (Magnitometers_LIS3MDL_Read_Data(&B_raw, LIS3MDL_MAG_I2C_ADDRESS_HIGH) != E_OK) fprintf(ComInterface, "ERR exe\r");
                      else {
                        fprintf(ComInterface, "OK+2%1.0f/%01.0f/%1.0f\r", B_raw.AXIS_X, B_raw.AXIS_Y, B_raw.AXIS_Z);
                        fprintf(ComInterface, "Magnetometer 2 -  Magnetic field in specific rande X=%1.0f Y=%1.0f Z=%1.0f\r", B_raw.AXIS_X, B_raw.AXIS_Y, B_raw.AXIS_Z);
                      }                         
                #endif        
                  break;      
                  
                case ESTTC_CMD_MGA2_ACESS: //0x07:
                #ifdef HMC5883                     
                  if (SEN_SUCCESS == HMC5883L_ReadReg(txline[Interface][7], (uint8_t *)&txline[Interface][9], txline[Interface][8]))
                  {
                    fprintf(ComInterface, "OK+");
                    for (i = 0; i < txline[Interface][8]; i++) fprintf(ComInterface, "%02X", txline[Interface][9+i]);
                    fprintf(ComInterface, "\r");
                  }
                #else                 
                  if (SEN_SUCCESS == LIS3MDL_MAG_ReadReg(LIS3MDL_MAG_I2C_ADDRESS_HIGH, txline[Interface][6], (uint8_t *) &txline[Interface][7])) {
                    fprintf(ComInterface, "OK+2%02X/%02X\r", txline[Interface][6], txline[Interface][7]);
                    fprintf(ComInterface, "Magnetometer 2 reg No %d has value %d\r", txline[Interface][6], txline[Interface][7]);
                  }
                #endif  
                  else
                    fprintf(ComInterface, "ERR exe\r");
                  break;                 

                case ESTTC_CMD_GYR1_X_RADIO_DATA: //0x08:
                  if ((ADIS16265_GetAxesRate(&data) == SEN_SUCCESS) && (data.AXIS_X != 0x7FFF))   {  
                     fprintf(ComInterface, "OK+1%d\r", data.AXIS_X);
                     fprintf(ComInterface, "Gyroscope 1 Data X=%5d\r", data.AXIS_X);
                  }
                  else fprintf(ComInterface, "ERR exe\r");
                break;

                case ESTTC_CMD_GYR1_X_ANGLE_DATA: //0x09:
                  if ((ADIS16265_GetAxesAngle(&data) == SEN_SUCCESS) && (data.AXIS_X != 0x7FFF))   { 
                     fprintf(ComInterface, "OK+1%d\r", data.AXIS_X);
                     fprintf(ComInterface, "Gyroscope 1 Angle X=%5d\r", data.AXIS_X);
                  }
                  else fprintf(ComInterface, "ERR exe\r");
                break;    
                
                case ESTTC_CMD_GYR1_X_AB_DATA: //0x0A:
                  if (ADIS16265_ReadReg16(txline[Interface][6], (uint16_t *)&utmp, PAN_X_M) == SEN_SUCCESS)   {
                      fprintf(ComInterface, "OK+1%02X/%02X\r", txline[Interface][6], utmp);
                      fprintf(ComInterface, "Gyroscope 1 reg No %d has value %d\r", txline[Interface][6], utmp);
                  } 
                  else fprintf(ComInterface, "ERR exe\r");
                break;                               
                
                case ESTTC_CMD_GYR2_Y_RADIO_DATA: //0x0B:
                  if ((ADIS16265_GetAxesRate(&data) == SEN_SUCCESS) && (data.AXIS_Y != 0x7FFF))   {   
                     fprintf(ComInterface, "OK+2%d\r", data.AXIS_Y);
                     fprintf(ComInterface, "Gyroscope 2 Data Y=%5d\r", data.AXIS_Y);
                  }
                  else fprintf(ComInterface, "ERR exe\r");
                break;

                case ESTTC_CMD_GYR2_Y_ANGLE_DATA: //0x0C:
                  if ((ADIS16265_GetAxesAngle(&data) == SEN_SUCCESS) && (data.AXIS_Y != 0x7FFF))   {              
                     fprintf(ComInterface, "OK+2%d\r", data.AXIS_Y);
                     fprintf(ComInterface, "Gyroscope 2 Angle Y=%5d\r", data.AXIS_Y);
                  }
                  else fprintf(ComInterface, "ERR exe\r");
                break; 
                
                case ESTTC_CMD_GYR2_Y_AB_DATA: //0x0D:
                  if (ADIS16265_ReadReg16(txline[Interface][6], (uint16_t *)&utmp, PAN_Y_M) == SEN_SUCCESS)   {
                      fprintf(ComInterface, "OK+2%02X/%02X\r", txline[Interface][6], utmp);
                      fprintf(ComInterface, "Gyroscope 2 reg No %d has value %d\r", txline[Interface][6], utmp);
                  } 
                  else fprintf(ComInterface, "ERR exe\r");
                break;
                
                case ESTTC_CMD_GYR3_Z_RADIO_DATA: //0x0E:
                  if ((ADIS16265_GetAxesRate(&data) == SEN_SUCCESS) && (data.AXIS_Z != 0x7FFF))   {    
                     fprintf(ComInterface, "OK+3%d\r", data.AXIS_Z);
                     fprintf(ComInterface, "Gyroscope 3 Data Z=%5d\r", data.AXIS_Z);
                  }
                  else fprintf(ComInterface, "ERR exe\r");
                break;

                case ESTTC_CMD_GYR3_Z_ANGLE_DATA: //0x0F:
                  if ((ADIS16265_GetAxesAngle(&data) == SEN_SUCCESS) && (data.AXIS_Z != 0x7FFF))  {               
                     fprintf(ComInterface, "OK+3%d\r", data.AXIS_Z);
                     fprintf(ComInterface, "Gyroscope 3 Angle Z=%5d\r", data.AXIS_Z);
                  }
                  else fprintf(ComInterface, "ERR exe\r");
                break;   
                
                case ESTTC_CMD_GYR4_Z_AB_DATA: //0x10:
                  if (ADIS16265_ReadReg16(txline[Interface][6], (uint16_t *)&utmp, PAN_Z_M) == SEN_SUCCESS)  {
                      fprintf(ComInterface, "OK+3%02X/%02X\r", txline[Interface][6], utmp);
                      fprintf(ComInterface, "Gyroscope 3 reg No %d has value %d\r", txline[Interface][6], utmp);
                  } 
                  else fprintf(ComInterface, "ERR exe\r");
                break;                    
                  
                case ESTTC_CMD_TEMP_PANEL_X_P: //0x14:
                  Panels_Init();
                  if ((TMP122_GetTemperatureP(&tmperature_sens) == SEN_SUCCESS) && (PanelStat & (1<< PAN_X_P))) {
                      tp = ((tmperature_sens.Temp_X/8)*10)/16;
                      fprintf(ComInterface, "OK+1%04X\r",(uint16_t)tmperature_sens.Temp_X);
                      fprintf(ComInterface, "Temperature Panel 1 (PAN1) =%3d.%d\r", tp/10, tp%10);
                   } else
                      fprintf(ComInterface, "ERR - Panel 1 is not attached!\r");
                   break;
                  
                case ESTTC_CMD_TEMP_PANEL_Y_P: //0x15:
                  Panels_Init();
                  if ((TMP122_GetTemperatureP(&tmperature_sens) == SEN_SUCCESS) && (PanelStat & (1<< PAN_Y_P))) {
                      tp = ((tmperature_sens.Temp_Y/8)*10)/16;
                      fprintf(ComInterface, "OK+2%04X\r",(uint16_t)tmperature_sens.Temp_Y);
                      fprintf(ComInterface, "Temperature Panel 2 (PAN2) =%3d.%d\r", tp/10, tp%10);
                   } else
                      fprintf(ComInterface, "ERR - Panel 2 is not attached!\r");
                   break;
                   
                case ESTTC_CMD_TEMP_PANEL_Z_P: //0x16:
                  Panels_Init();
                  if ((TMP122_GetTemperatureP(&tmperature_sens) == SEN_SUCCESS) && (PanelStat & (1<< PAN_Z_P))) {
                      tp = ((tmperature_sens.Temp_Z/8)*10)/16;
                      fprintf(ComInterface, "OK+3%04X\r",(uint16_t)tmperature_sens.Temp_Z);
                      fprintf(ComInterface, "Temperature Panel 3 (PAN3) =%3d.%d\r", tp/10, tp%10);
                   } else
                      fprintf(ComInterface, "ERR - Panel 3 is not attached!\r");
                   break;
                   
                case ESTTC_CMD_TEMP_PANEL_X_M: //0x17:
                  Panels_Init();
                  if ((TMP122_GetTemperatureM(&tmperature_sens) == SEN_SUCCESS) && (PanelStat & (1<< PAN_X_M))) {
                      tp = ((tmperature_sens.Temp_X/8)*10)/16;
                      fprintf(ComInterface, "OK+4%04X\r",(uint16_t)tmperature_sens.Temp_X);
                      fprintf(ComInterface, "Temperature Panel 4 (PAN4) =%3d.%d\r", tp/10, tp%10);
                   } else
                      fprintf(ComInterface, "ERR - Panel 4 is not attached!\r");
                   break;
                  
                case ESTTC_CMD_TEMP_PANEL_Y_M: //0x18:
                  Panels_Init();
                  if ((TMP122_GetTemperatureM(&tmperature_sens) == SEN_SUCCESS) && (PanelStat & (1<< PAN_Y_M))) {
                      tp = ((tmperature_sens.Temp_Y/8)*10)/16;
                      fprintf(ComInterface, "OK+5%04X\r",(uint16_t)tmperature_sens.Temp_Y);
                      fprintf(ComInterface, "Temperature Panel 5 (PAN5) =%3d.%d\r", tp/10, tp%10);
                   } else
                      fprintf(ComInterface, "ERR - Panel 5 is not attached!\r");
                   break;
                   
                case ESTTC_CMD_TEMP_PANEL_Z_M: //0x19:
                  Panels_Init();
                  if ((TMP122_GetTemperatureM(&tmperature_sens) == SEN_SUCCESS) && (PanelStat & (1<< PAN_Z_M))) {
                      tp = ((tmperature_sens.Temp_Z/8)*10)/16;
                      fprintf(ComInterface, "OK+6%04X\r",(uint16_t)tmperature_sens.Temp_Z);
                      fprintf(ComInterface, "Temperature Panel 6 (PAN6) =%3d.%d\r", tp/10, tp%10);
                   } else
                      fprintf(ComInterface, "ERR - Panel 6 is not attached!\r");
                   break;                                     

                case ESTTC_CMD_PHOTO_PANEL_1: //0x1A:
                   Panel_GetPhotodiodesLum();
                   fprintf(ComInterface, "OK+1%02X\r", PanelLight[0]);
                   fprintf(ComInterface, "Panel Light 1 (PAN1) =%4u\r ", PanelLight[0]);
                   break;
                   
                case ESTTC_CMD_PHOTO_PANEL_2: //0x1B:
                   Panel_GetPhotodiodesLum();
                   fprintf(ComInterface, "OK+2%02X\r", PanelLight[1]);
                   fprintf(ComInterface, "Panel Light 2 (PAN2) =%4u\r ", PanelLight[1]);
                   break;
                   
                case ESTTC_CMD_PHOTO_PANEL_3: //0x1C:
                   Panel_GetPhotodiodesLum();
                   fprintf(ComInterface, "OK+3%02X\r", PanelLight[2]);
                   fprintf(ComInterface, "Panel Light 3 (PAN3) =%4u\r ", PanelLight[2]);
                   break;
                                     
                case ESTTC_CMD_PHOTO_PANEL_4: //0x1D:
                   Panel_GetPhotodiodesLum();
                   fprintf(ComInterface, "OK+4%02X\r", PanelLight[3]);
                   fprintf(ComInterface, "Panel Light 4 (PAN4) =%4u\r ", PanelLight[3]);
                   break;
                                              
                case ESTTC_CMD_PHOTO_PANEL_5: //0x1E:
                   Panel_GetPhotodiodesLum();
                   fprintf(ComInterface, "OK+5%02X\r", PanelLight[4]);
                   fprintf(ComInterface, "Panel Light 5 (PAN5) =%4u\r ", PanelLight[4]);
                   break;
                                              
                case ESTTC_CMD_PHOTO_PANEL_6: //0x1F:
                   Panel_GetPhotodiodesLum();
                   fprintf(ComInterface, "OK+6%02X\r", PanelLight[5]);
                   fprintf(ComInterface, "Panel Light 6 (PAN6) =%4u\r ", PanelLight[5]);
                   break;
                   
                case ESTTC_CMD_GET_TIME: //0x31:   // get time
                  if (HAL_OK == HAL_RTC_GetTime(&hrtc, &sTime, calendar_format))
                  {
                    fprintf(ComInterface, "OK TIME %02d:%02d:%02d  \r", sTime.Hours, sTime.Minutes, sTime.Seconds);
                  }else{
                    fprintf(ComInterface, "ERR exe\r");
                  }
                    HAL_RTC_GetDate(&hrtc, &sDate, calendar_format); // synchronization RTC date/time
                  break;

                case ESTTC_CMD_GET_DATA: //0x33:   // get date
                  if (HAL_OK == HAL_RTC_GetDate(&hrtc, &sDate, calendar_format))
                    fprintf(ComInterface, "OK DATE YY/MM/DD %d / %d / %d  \r", sDate.Year, sDate.Month, sDate.Date);
                  else
                    fprintf(ComInterface, "ERR exe\r");
                  break;


                case ESTTC_CMD_PAR_NUM:   // get s/w version
                      if (*((__IO uint32_t*)MAILBOX_ADDRESS) == MAILBOX_VAL_APPL)
                      {
                    	  fprintf(ComInterface, "OK: SDK v.%d.%0d / APPL v.%d.%0d / <%s %s>\r",
                    				verSDK_MAJOR_REV_NB, verSDK_MINOR_REV_NB,
                    	          verFW_MAJOR_REV_NB, verFW_MINOR_REV_NB,
                    				__DATE__, __TIME__);
                      }
                      else
                      {
                    	  fprintf(ComInterface, "OK: BOOT v.%d.%0d / <%s %s>\r",
                    				verFW_MAJOR_REV_NB, verFW_MINOR_REV_NB,
                    			  __DATE__, __TIME__);
                      }
                  break;

                default:
                  if (txline[Interface][5] < ESTTC_CMD_PAR_NUM)
                  {
                      txline[Interface][5] = ESTTC_CMD_PAR_NUM-1;
                      fprintf(ComInterface, "ERR invalid number\r");
                  }
                  else
                    fprintf(ComInterface, "ERR parm\r");
                  break;
              }
            }
            else
            if (txline[Interface][4] == UHF_I2C_ADDRESS)
            {
              switch(txline[Interface][5])
              {
                default:
                  if (txline[Interface][5] < UHF_MAX_PAR_NUM)
                  {
                    if (HAL_OK == HAL_I2C_Mem_Read (&hi2c1, UHF_I2C_ADDRESS<<1, txline[Interface][5], 1, (uint8_t *)&txline[Interface][7], txline[Interface][6], 10))
                    {
                      fprintf(ComInterface, "OK+%02X%02X%02X%02X\r", txline[Interface][7], txline[Interface][8], txline[Interface][9], txline[Interface][10]);
                    }
                  }
                  else
                    fprintf(ComInterface, "ERR parm\r");
                  break;
              }
            }
            else
              fprintf(ComInterface, "ERR addr\r");
          }
          else
          if (txline[Interface][3] == 'W')
          {
            if (txline[Interface][4] == EPS_I2C_ADDRESS)
            {
              if (txline[Interface][5] == EPS_OUT_PAR_NUM)
              {
                if (txline[Interface][6] == ((len-10)/2))
                {
                  if (HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, EPS_I2C_ADDRESS<<1, (uint8_t *)&txline[Interface][7], txline[Interface][6], 10))
                    fprintf(ComInterface, "OK\r");
                  else
                    fprintf(ComInterface, "ERR exe\r");
                }
                else
                  fprintf(ComInterface, "ERR len\r");
              }
              else
                fprintf(ComInterface, "ERR parm\r");
            }
            else
            if (txline[Interface][4] == ANT_I2C_ADDRESS)
            {
              if (txline[Interface][6] == ((len-10)/2))
              {
                if (HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, ANT_I2C_ADDRESS<<1, (uint8_t *)&txline[Interface][7], txline[Interface][6], 10))
                  fprintf(ComInterface, "OK\r");
                else
                  fprintf(ComInterface, "ERR exe\r");
              }
              else
                fprintf(ComInterface, "ERR len\r");
            }
            else
            if (txline[Interface][4] == OBC_I2C_ADDRESS)
            {
              if (txline[Interface][5] <= ESTTC_CMD_PAR_NUM)
              {
                if (txline[Interface][6] == ((len-10)/2))
                {
                  switch(txline[Interface][5])
                  {
                    case ESTTC_CMD_ACCSEL_1_ACESS: // 0x01
                      if(SEN_SUCCESS == AIS328DQ_WriteReg(AIS328DQ_1_MEMS_I2C_ADDRESS,txline[Interface][7], txline[Interface][8])) {
                        fprintf(ComInterface, "OK+1%02X/%02X\r", txline[Interface][7], txline[Interface][8]);
                        fprintf(ComInterface, "Accelerometer 1 register %X is set with value %X\r", txline[Interface][7], txline[Interface][8]);
                      }
                      else
                        fprintf(ComInterface, "ERR - Not valid parameters!\r");
                      break;

                    case ESTTC_CMD_ACCSEL_2_ACESS: //0x03:
                      if(SEN_SUCCESS == AIS328DQ_WriteReg(AIS328DQ_2_MEMS_I2C_ADDRESS,txline[Interface][7], txline[Interface][8])) {
                        fprintf(ComInterface, "OK+2%02X/%02X\r", txline[Interface][7], txline[Interface][8]);
                        fprintf(ComInterface, "Accelerometer 2 register %X is set with value %X\r", txline[Interface][7], txline[Interface][8]);
                      }
                      else
                        fprintf(ComInterface, "ERR - Not valid parameters!\r");
                      break;

                    case ESTTC_CMD_MGA1_ACESS: //0x05:
#ifdef HMC5883                     
                      if (SEN_SUCCESS == HMC5883L_WriteReg(txline[Interface][7], txline[Interface][8]))
                        fprintf(ComInterface, "Magnetometer 1 set reg No %d with value %d\r", txline[Interface][7], txline[Interface][8]);
                      else
                        fprintf(ComInterface, "ERR exe\r");
#else                         
                      if (SEN_SUCCESS == LIS3MDL_MAG_WriteReg(LIS3MDL_MAG_I2C_ADDRESS_LOW, txline[Interface][7], txline[Interface][8])) {
                        fprintf(ComInterface, "OK+1%02X/%02X\r", txline[Interface][7], txline[Interface][8]);
                        fprintf(ComInterface, "Magnetometer 1 set reg No %d with value %d\r", txline[Interface][7], txline[Interface][8]);
                      }
                      else
                        fprintf(ComInterface, "ERR exe\r");
#endif                       
                      break;

                    case ESTTC_CMD_MGA2_ACESS: //0x07:
 #ifdef HMC5883                     
                      if (SEN_SUCCESS == HMC5883L_WriteReg(txline[Interface][7], txline[Interface][8]))
                        fprintf(ComInterface, "Magnetometer 2 set reg No %d with value %d\r", txline[Interface][7], txline[Interface][8]);
                      else
                        fprintf(ComInterface, "ERR exe\r");
#else                         
                      if (SEN_SUCCESS == LIS3MDL_MAG_WriteReg(LIS3MDL_MAG_I2C_ADDRESS_HIGH, txline[Interface][7], txline[Interface][8])) {
                        fprintf(ComInterface, "OK+2%02X/%02X\r", txline[Interface][7], txline[Interface][8]);
                        fprintf(ComInterface, "Magnetometer 2 set reg No %d with value %d\r", txline[Interface][7], txline[Interface][8]);
                      }
                      else
                        fprintf(ComInterface, "ERR exe\r");
#endif                       
                      break;                     
                      
                    case ESTTC_CMD_GYR1_X_AB_DATA: //0x0A:
                      utmp = ((uint16_t)txline[Interface][8] << 8) + (uint16_t)txline[Interface][9];
                      if (SEN_SUCCESS == ADIS16265_WriteReg16(txline[Interface][7], utmp, PAN_X_M)) {
                        fprintf(ComInterface, "OK+1%02X/%02X\r", txline[Interface][7], utmp);
                        fprintf(ComInterface, "Gyroscope 1 set reg No %d with value %d\r", txline[Interface][7], utmp);
                      }
                      else
                        fprintf(ComInterface, "ERR exe\r");
                      break;   
                      
                    case ESTTC_CMD_GYR2_Y_AB_DATA: //0x0D:
                      utmp = ((uint16_t)txline[Interface][8] << 8) + (uint16_t)txline[Interface][9];
                      if (SEN_SUCCESS == ADIS16265_WriteReg16(txline[Interface][7], utmp, PAN_Y_M)) {
                        fprintf(ComInterface, "OK+2%02X/%02X\r", txline[Interface][7], utmp);
                        fprintf(ComInterface, "Gyroscope 2 set reg No %d with value %d\r", txline[Interface][7], utmp);
                      }
                      else
                        fprintf(ComInterface, "ERR exe\r");
                      break;                                          

                    case ESTTC_CMD_GYR4_Z_AB_DATA: //0x10:
                      utmp = ((uint16_t)txline[Interface][8] << 8) + (uint16_t)txline[Interface][9];
                      if (SEN_SUCCESS == ADIS16265_WriteReg16(txline[Interface][7], utmp, PAN_Z_M)) {
                        fprintf(ComInterface, "OK+3%02X/%02X\r", txline[Interface][7], utmp);
                        fprintf(ComInterface, "Gyroscope 3 set reg No %d with value %d\r", txline[Interface][7], utmp);
                      }
                      else
                        fprintf(ComInterface, "ERR exe\r");
                      break;                        

                    case ESTTC_CMD_MAGTRK1_POWER: //0x11:
                      if (txline[Interface][7] > 100) txline[Interface][8] = 100; // 100% is maximum
                      if (txline[Interface][8] > 1)  txline[Interface][9] = 1; // 0 - negative, 1 is positive direction
                      if (SetMagnetorque(PAN_X_M, txline[Interface][7], txline[Interface][8]) == SEN_SUCCESS) {
                          fprintf(ComInterface, "OK+1%02X/X", txline[Interface][7]);
                          if(txline[Interface][8] == 0)
						  {
						    fprintf(ComInterface, "-\r");
						  }
						  else
						  {
						    fprintf(ComInterface, "+\r");
						  }

                          fprintf(ComInterface, "Set Magnetorquer 1 (TRQ1) on PAN 4 with power %d and direction X", txline[Interface][7]);
                          if(txline[Interface][8] == 0)
                          {
                        	  fprintf(ComInterface, "-\r");
                          }
                          else
                          {
                        	  fprintf(ComInterface, "+\r");
                          }
                      }
                      else fprintf(ComInterface, "ERR - Not valid parameters!\r");
                      break;
                  
                    case ESTTC_CMD_MAGTRK2_POWER: //0x12:
                      if (txline[Interface][7] > 100) txline[Interface][8] = 100; // 100% is maximum
                      if (txline[Interface][8] > 1)  txline[Interface][9] = 1; // 0 - negative, 1 is positive direction
                      if (SetMagnetorque(PAN_Y_M, txline[Interface][7], txline[Interface][8]) == SEN_SUCCESS) {
                          fprintf(ComInterface, "OK+2%02X/Y", txline[Interface][7]);
                          if(txline[Interface][8] == 0)
						  {
						    fprintf(ComInterface, "-\r");
						  }
						  else
						  {
						    fprintf(ComInterface, "+\r");
						  }
                          fprintf(ComInterface, "Set Magnetorquer 2 (TRQ2) on PAN 5 with power %d and direction Y", txline[Interface][7]);
                          if(txline[Interface][8] == 0)
                          {
                        	  fprintf(ComInterface, "-\r");
                          }
                          else
                          {
                        	  fprintf(ComInterface, "+\r");
                          }
                      }
                      else fprintf(ComInterface, "ERR - Not valid parameters!\r");
                      break;
                      
                    case ESTTC_CMD_MAGTRK3_POWER: //0x13:
                      if (txline[Interface][7] > 100) txline[Interface][8] = 100; // 100% is maximum
                      if (txline[Interface][8] > 1)  txline[Interface][9] = 1; // 0 - negative, 1 is positive direction
                      if (SetMagnetorque(PAN_Z_M, txline[Interface][7], txline[Interface][8]) == SEN_SUCCESS) {
                          fprintf(ComInterface, "OK+3%02X/Z", txline[Interface][7]);
                          if(txline[Interface][8] == 0)
						  {
						    fprintf(ComInterface, "-\r");
						  }
						  else
						  {
						    fprintf(ComInterface, "+\r");
						  }

                          fprintf(ComInterface, "Set Magnetorquer 3 (TRQ3) on PAN 6 with power %d and direction Z", txline[Interface][7]);
                          if(txline[Interface][8] == 0)
                          {
                        	  fprintf(ComInterface, "-\r");
                          }
                          else
                          {
                        	  fprintf(ComInterface, "+\r");
                          }
                      }
                      else fprintf(ComInterface, "ERR - Not valid parameters!\r");
                      break;

                    case ESTTC_CMD_SET_TIME: //0x32:   // set time
                        sTime.Hours = (uint16_t)((txline[Interface][7]-0x30)*10 + (txline[Interface][8]-0x30));
                        sTime.Minutes = (uint16_t)((txline[Interface][9]-0x30)*10 + (txline[Interface][10]-0x30));
                        sTime.Seconds = (uint16_t)((txline[Interface][11]-0x30)*10 + (txline[Interface][12]-0x30));
                        if (HAL_OK == HAL_RTC_SetTime(&hrtc, &sTime, calendar_format))
                          fprintf(ComInterface, "OK TIME %02d:%02d:%02d  \r", sTime.Hours, sTime.Minutes, sTime.Seconds);
                        else
                          fprintf(ComInterface, "ERR exe\r");
                        break;

                    case ESTTC_CMD_SET_DATA: //0x34:   // set date
                        sDate.Year = (uint16_t)((txline[Interface][7]-0x30)*10 + (txline[Interface][8]-0x30));
                        sDate.Month = (uint16_t)((txline[Interface][9]-0x30)*10 + (txline[Interface][10]-0x30));
                        sDate.Date = (uint16_t)((txline[Interface][11]-0x30)*10 + (txline[Interface][12]-0x30));
                        if (HAL_OK == HAL_RTC_SetDate(&hrtc, &sDate, calendar_format))
                          fprintf(ComInterface, "OK DATE YY/MM/DD %d / %d / %d  \r", sDate.Year, sDate.Month, sDate.Date);
                        else
                          fprintf(ComInterface, "ERR exe\r");
                    break;



                    case ESTTC_CMD_PAR_NUM: // Conditional RESET
                      if ((txline[Interface][6] == 1) && (txline[Interface][7] == 0xA))
                      {
                        fprintf(ComInterface, "\nOK+APPL\r");
                        *((__IO uint32_t *)MAILBOX_ADDRESS) = MAILBOX_VAL_APPL;
                      }
                      else
                      if ((txline[Interface][6] == 1) && (txline[Interface][7] == 0xB))
                      {
                        fprintf(ComInterface, "\nOK+BOOT\r");
                        *((__IO uint32_t *)MAILBOX_ADDRESS) = MAILBOX_VAL_BOOT;
                      }
                      else
                      if ((txline[Interface][6] == 1) && (txline[Interface][7] == 0xF))
                      {
                        fprintf(ComInterface, "\nOK+FILE\r");
                        *((__IO uint32_t *)MAILBOX_ADDRESS) = MAILBOX_VAL_FILE;
                      }
                      else
                        fprintf(ComInterface, "\nERR\r");
                      osDelay(50);
                      MX_WWDG_Init();
                      while(1);
                      break;

                    default:
                      fprintf(ComInterface, "ERR parm\r");
                      break;
                  } // switch case
                } // txline[Interface][6]
                else
                  fprintf(ComInterface, "ERR len\r");
              } // txline[Interface][5]
              else
                fprintf(ComInterface, "ERR parm\r");
            } // txline[Interface] [4]
            else
            if (txline[Interface][4] == UHF_I2C_ADDRESS)
            {
              if (txline[Interface][6] == ((len-10)/2))
              {
                if (HAL_OK == HAL_I2C_Mem_Write(&hi2c1, UHF_I2C_ADDRESS<<1, txline[Interface][5], 1, (uint8_t *)&txline[Interface][7], txline[Interface][6], 10))
                    fprintf(ComInterface, "OK\r");
                else
                  fprintf(ComInterface, "ERR exe\r");
              }
              else
                fprintf(ComInterface, "ERR len\r");
            }
            else
              fprintf(ComInterface, "ERR addr\r");
          }
          else
          if (txline[Interface][3] == 'D')
          {
            if (txline[Interface][4] == OBC_I2C_ADDRESS)
            {
              if (begin[6] == 'F')
              {
                switch(begin[7])
                {
                  case 'A':
                    if (*((__IO uint32_t*)MAILBOX_ADDRESS) == MAILBOX_VAL_APPL)
                    {
                      fprintf(ComInterface, "ERR+INAPP\r");
                      break;
                    }
                    if (df[Interface].fs) f_close(&df[Interface]);
                    sprintf(txline[Interface], "0:/%s", &begin[8]);
                    if (FR_OK != (fr = f_open(&df[Interface], txline[Interface], FA_READ | FA_OPEN_EXISTING)))
                    {
                      fprintf(ComInterface, "ERR+FNF(%u)=%s\r", fr, txline[Interface]);
                      break;
                    }
                    else{
                        if (FR_OK == f_stat(txline[Interface], &fno[Interface]) )
                        {
                            if( fno[Interface].fsize < 0xFFFF )
                            {
                              fprintf(ComInterface, "OK+%08X B\r", (uint16_t)(f_size(&df[Interface])));
                            }else if( fno[Interface].fsize < 0x3FFFFF )
                            {
                              fprintf(ComInterface, "OK+%08X kB\r", (uint16_t)(f_size(&df[Interface])>>10));
                            }else
                            {
                                fprintf(ComInterface, "OK+%08X MB\r", (uint16_t)(f_size(&df[Interface])>>20));
                            }
                        }
                    }

                    fprintf(ComInterface, "Erasing FLASH...\n\r");
                    if (HAL_OK != HAL_FLASH_Unlock())
                    {
                      f_close(&df[Interface]);
                      fprintf(ComInterface, "ERR+FU\r");
                      break;
                    }
                    //FLASH_SetErrorCode();
                    // Calculate number of sectors
                    bsize[Interface] = (f_size(&df[Interface]) + FLASH_SECTOR_SIZE) / FLASH_SECTOR_SIZE;
                    for (i = FLASH_MIN_SECTOR; i < FLASH_MIN_SECTOR + bsize[Interface]; i++)
                    {
                      fes.TypeErase = FLASH_TYPEERASE_SECTORS;
                      fes.NbSectors = 1;
                      fes.Sector = i;
                      fes.VoltageRange = FLASH_VOLTAGE_RANGE_3;
                      if (HAL_OK != HAL_FLASHEx_Erase(&fes, &se))
                      {
                        f_close(&df[Interface]);
                        fprintf(ComInterface, "ERR+FBSE%u@%u\r", (uint16_t)se, (uint16_t)i);
                        break;
                      }
                      fprintf(ComInterface, "Sector%u erased.\n\r", (uint16_t)i);
                    }
                    fprintf(ComInterface, "BLANK CHECK...\n\r");
                    if (HAL_OK != (se = FLASH_PatternCheck(FLASH_MIN_ADDR, FLASH_BLANK, f_size(&df[Interface]))))
                    {
                      f_close(&df[Interface]);

                      fprintf(COMM, "ERR+FB%08X addr=%08X size=%08X\r", (uint16_t)se, (uint16_t)FLASH_MIN_ADDR, (uint16_t)df[Interface].fsize);

                      break;
                    }
                    fprintf(ComInterface, "BLANK\n\r");
                    bsize[Interface] = sizeof(txline[Interface]);
                    for (fpos[Interface] = 0; fpos[Interface] < f_size(&df[Interface]); fpos[Interface] += bsize[Interface])
                    {
                      if (f_size(&df[Interface]) - fpos[Interface] < bsize[Interface]) bsize[Interface] = f_size(&df[Interface]) - fpos[Interface]; // Last packet alignment
                      if (FR_OK != (fr = f_read(&df[Interface], txline[Interface], bsize[Interface], (UINT*)&br)))
                      {
                        fprintf(ComInterface, "ERR+FIR(%u)=%u\r", (uint16_t)fr, (uint16_t)br);
                        break;
                      }
                      if (bsize[Interface] != br)
                      {
                        fprintf(ComInterface, "ERR+FRS(%u)=%u\r", (uint16_t)bsize[Interface], (uint16_t)br);
                        break;
                      }
                      if (HAL_OK == (se = FLASH_Write((uint8_t *)txline[Interface], FLASH_MIN_ADDR+fpos[Interface], bsize[Interface])))
                      {
                        if ((fpos[Interface]%0x4000)==0) fprintf(ComInterface, "+");
                      }
                      else
                      {
                        fprintf(ComInterface, "ERR+FW%u\r", (uint16_t)se);
                        break;
                      }
                    }
                    j = f_size(&df[Interface]);
                    f_close(&df[Interface]);
                    if (fpos[Interface] == j)
                    {
                      __disable_interrupt();
                      for (j = 0, i = 0; i < fpos[Interface]; i++) j += *((uint8_t*)(i+FLASH_MIN_ADDR));
                      *((__IO uint32_t*)APP_SIZE_ADDRESS) = fpos[Interface];
                      *((__IO uint32_t*)APP_CHKS_ADDRESS) = j;
                      __enable_interrupt();
                      fprintf(ComInterface, "OK+%08X %08X\r", (uint16_t)j, (uint16_t)fpos[Interface]);
                    }
                    break;

                  case 'C':
                    if (df[Interface].fs) f_close(&df[Interface]);
                    fprintf(ComInterface, "OK\r");
                    break;

                  case 'S': // Calculate file checksum
                    if (df[Interface].fs) f_close(&df[Interface]);
                    sprintf(txline[Interface], "0:/%s", &begin[8]);
                    if (FR_OK != (fr = f_open(&df[Interface], txline[Interface], FA_READ | FA_OPEN_EXISTING)))
                    {
                      fprintf(ComInterface, "ERR+FNF(%u)=%s\r", fr, txline[Interface]);
                      break;
                    }
                    for (j = 0, i = 0; i < f_size(&df[Interface]); i++)
                    {
                      if (FR_OK != (fr = f_read(&df[Interface], &s8_tmp, 1, (UINT*)&br)))
                      {
                        f_close(&df[Interface]);
                        fprintf(ComInterface, "ERR+FIR(%u)=%u\r", (uint16_t)fr, (uint16_t)br);
                        break;
                      }
                      if (1 != br)
                      {
                        f_close(&df[Interface]);
                        fprintf(ComInterface, "ERR+FRS=%u\r", (uint16_t)br);
                        break;
                      }
                      j += s8_tmp;
                    }

                    if (FR_OK == f_stat(txline[Interface], &fno[Interface]) )
                    {
                        if( fno[Interface].fsize < 0xFFFF )
                        {
                            fprintf(ComInterface, "OK+%08X %08X B\r", (uint16_t)j, (uint16_t)f_size(&df[Interface]));
                        }else if( fno[Interface].fsize < 0x3FFFFF )
                        {
                            fprintf(ComInterface, "OK+%08X %08X kB\r", (uint16_t)j, (uint16_t)(f_size(&df[Interface])>>10));
                        }else
                        {
                            fprintf(ComInterface, "OK+%08X %08X MB\r", (uint16_t)j, (uint16_t)(f_size(&df[Interface])>>20));
                        }
                    }else{
                        fprintf(ComInterface, "ERR+WrongPath");
                    }

                    f_close(&df[Interface]);
                    break;

                  case 'I': // Find file for reading
                    if (df[Interface].fs) f_close(&df[Interface]);
                    sprintf(txline[Interface], "0:/%s", &begin[8]);
                    if (FR_OK == (fr = f_open(&df[Interface], txline[Interface], FA_READ | FA_OPEN_EXISTING)))
                    {
                        if (FR_OK == f_stat(txline[Interface], &fno[Interface]) )
                        {
                            if( fno[Interface].fsize < 0xFFFF )
                            {
                                fprintf(ComInterface, "OK+%08X B\r", (uint16_t)fno[Interface].fsize);
                            }else if( fno[Interface].fsize < 0x3FFFFF )
                            {
                                fprintf(ComInterface, "OK+%08X kB\r", (uint16_t)(fno[Interface].fsize>>10));
                            }else
                            {
                                fprintf(ComInterface, "OK+%08X MB\r", (uint16_t)(fno[Interface].fsize>>20));
                            }
                        }
                    }
                    else {
                        fprintf(ComInterface, "ERR+FNF(%u)=%s\r", fr, txline[Interface]);
                    }
                    break;

                  case 'R': // Read from file @ position
                    if (df[Interface].fs == 0)
                    {
                      fprintf(ComInterface, "ERR+FIH\r");
                      break;
                    }
                    sscanf(&begin[8], "%08X", (unsigned int *)&fpos[Interface]);
                    sscanf(&begin[8+8], "%04X", (unsigned int *)&bsize[Interface]);
                    if ((fpos[Interface] > f_size(&df[Interface]))||(bsize[Interface] > sizeof(txline[Interface])))
                    {
                      if( fno[Interface].fsize < 0xFFFF )
                      {
                          fprintf(ComInterface, "ERR+FIP=%u-%u B,%u-%u\r", (uint16_t)fpos[Interface], (uint16_t)fno[Interface].fsize, (uint16_t)bsize[Interface], sizeof(txline[Interface]));
                      }else if( fno[Interface].fsize < 0x3FFFFF )
                      {
                          fprintf(ComInterface, "ERR+FIP=%u-%u kB,%u-%u\r", (uint16_t)fpos[Interface], (uint16_t)(fno[Interface].fsize>>10), (uint16_t)bsize[Interface], sizeof(txline[Interface]));
                      }else
                      {
                          fprintf(ComInterface, "ERR+FIP=%u-%u MB,%u-%u\r", (uint16_t)fpos[Interface], (uint16_t)(fno[Interface].fsize>>20), (uint16_t)bsize[Interface], sizeof(txline[Interface]));
                      }

                      break;
                    }
                    if (FR_OK != (fr = f_lseek(&df[Interface], fpos[Interface])))
                    {
                      fprintf(ComInterface, "ERR+FIS(%u)=%u\r", (uint16_t)fr, (uint16_t)fpos[Interface]);
                      break;
                    }
                    if (FR_OK != (fr = f_read(&df[Interface], txline[Interface], bsize[Interface], (UINT*)&br)))
                    {
                      fprintf(ComInterface, "ERR+FIR(%u)=%u\r", fr, (uint16_t)br);
                      break;
                    }
                    if (bsize[Interface] != br)
                    {
                      fprintf(ComInterface, "ERR+FRS(%u)=%u\r", (uint16_t)bsize[Interface], (uint16_t)br);
                      break;
                    }
                    br = 0;
                    for (i = 0; i < bsize[Interface]; i++)
                    {
                      br += txline[Interface][i];
                      fprintf(ComInterface, "%c", txline[Interface][i]);
                    }
                    fprintf(ComInterface, "%c", (BYTE)br);
                    break;

                  case 'O': // Create file for writing
                    if (df[Interface].fs) f_close(&df[Interface]);
                    sprintf(txline[Interface], "0:/%s", &begin[8]);
                    if (FR_OK == (fr = f_open(&df[Interface], txline[Interface], FA_WRITE | FA_CREATE_ALWAYS)))
                    {
                        fprintf(ComInterface, "OK\r");
                    }
                    else fprintf(ComInterface, "ERR+FNC(%u)=%s\r", fr, txline[Interface]);
                    break;

                  case 'W': // Write to file @ position
                    if (df[Interface].fs == NULL)
                    {
                      fprintf(ComInterface, "ERR+FIH\r");
                      break;
                    }
                    sscanf(&begin[8], "%08X", (unsigned int *)&fpos[Interface]);
                    sscanf(&begin[8+8], "%04X", (unsigned int *)&bsize[Interface]);
                    if ((fpos[Interface] > f_size(&df[Interface]))||(bsize[Interface] > sizeof(txline[Interface])))
                    {
                      if( fno[Interface].fsize < 0xFFFF )
                      {
                          fprintf(ComInterface, "ERR+FIP=%X-%X B,%s\r", (uint16_t)fpos[Interface], (uint16_t)fno[Interface].fsize, &begin[8]);
                      }else if( fno[Interface].fsize < 0x3FFFFF )
                      {
                          fprintf(ComInterface, "ERR+FIP=%X-%X kB,%s\r", (uint16_t)fpos[Interface], (uint16_t)(fno[Interface].fsize>>10), &begin[8]);
                      }else
                      {
                          fprintf(ComInterface, "ERR+FIP=%X-%X MB,%s\r", (uint16_t)fpos[Interface], (uint16_t)(fno[Interface].fsize>>20), &begin[8]);
                      }

                      break;
                    }
                    if (FR_OK != (fr = f_lseek(&df[Interface], fpos[Interface])))
                    {
                      fprintf(ComInterface, "ERR+FIS(%u)=%u\r", (uint16_t)fr, (uint16_t)fpos[Interface]);
                      break;
                    }
                    int ch = -1;
                    br = 0;
                    for (i = 0; i < bsize[Interface]; i++)
                    {
                      if ((ch = getbyte(20, Interface)) == -1) break;
                      txline[Interface][i] = (BYTE)ch;
                      br += (BYTE)ch;
                    }
                    if (ch == -1)
                    {
                      fprintf(ComInterface, "ERR+FTM\r");
                      break;
                    }
                    if ((ch = getbyte(20, Interface)) == -1)
                    {
                      fprintf(ComInterface, "ERR+FTM\r");
                      break;
                    }
                    if (((BYTE)br) != ((BYTE)ch))
                    {
                      fprintf(ComInterface, "ERR+FEC=%02X(%02X)\r", (BYTE)br, (BYTE)ch);
                      break;
                    }
                    if (FR_OK != (fr = f_write(&df[Interface], txline[Interface], bsize[Interface], (UINT*)&br)))
                    {
                      fprintf(ComInterface, "ERR+FWE=%u\r", fr);
                      break;
                    }
                    if (bsize[Interface] != br)
                    {
                      fprintf(ComInterface, "ERR+FWC=%u(%u)\r", (uint16_t)br, (uint16_t)bsize[Interface]);
                      break;
                    }
                    fprintf(ComInterface, "OK\r");
                    break;

                  case 'D': // Delete file
                    if (df[Interface].fs) f_close(&df[Interface]);
                    sprintf(txline[Interface], "0:/%s", &begin[8]);
                    if (FR_OK != f_unlink(txline[Interface]))
                      fprintf(ComInterface, "ERR+FDL%s\r", txline[Interface]);
                    else
                      fprintf(ComInterface, "OK\r");
                    break;

                  case 'L': // Write to file "DirList" a list of existing files
                    if (df[Interface].fs) f_close(&df[Interface]);
                    if (FR_OK != (fr = f_open(&df[Interface], "0:/DirList.txt", FA_WRITE | FA_CREATE_ALWAYS)))
                    {
                        fprintf(ComInterface, "ERR+LCF(%u)\r", fr);
                      break;
                    }
                    len = strlen(&begin[8]);
                    if ((len == 0) || (len > 12))
                    {
                      begin[8] = '*';
                      begin[9] = 0;
                    }
                    fprintf((FILE *)&df[Interface], "----- FATFS RevID.%05u -----\r\n", _FATFS);
                    fprintf((FILE *)&df[Interface], "--- Name ---    --- size ---\r\n" );
                    strcpy(txline[Interface], &begin[8]);
                    j = 0;
                    if (FR_OK != (fr = f_findfirst(&dd[Interface], &fno[Interface], "", txline[Interface])))
                    {
                      fprintf(ComInterface, "ERR+LFF(%u)\r", fr);
                      break;
                    }
                    do {
                      if (strlen(fno[Interface].fname) == 0) break;
                      j++;

                      if( fno[Interface].fsize < 0xFFFF )
                      {
                          sprintf(rxline[Interface], "%13s   %u B\n", fno[Interface].fname, (uint16_t)fno[Interface].fsize);
                      }else if( fno[Interface].fsize < 0x3FFFFF )
                      {
                          sprintf(rxline[Interface], "%13s   ~%u kB\n", fno[Interface].fname, (uint16_t)(fno[Interface].fsize>>10));
                      }else
                      {
                          sprintf(rxline[Interface], "%13s   ~%u MB\n", fno[Interface].fname, (uint16_t)(fno[Interface].fsize>>20));
                      }

                      f_puts(rxline[Interface], &df[Interface]);
                    } while (FR_OK == f_findnext(&dd[Interface], &fno[Interface]));
                    fprintf((FILE *)&df[Interface], "-----------------------------\n         TOTAL FILES %u\n", (uint16_t)j);
                    f_close(&df[Interface]);
                    fprintf(ComInterface, "OK\r");
                    break;

                  default: // Unknown command
                    fprintf(ComInterface, "ERR+UNC:%s\r", &begin[7]);
                    break;
                }
              }
            }
          }
          else
            fprintf(ComInterface, "ERR cmd\r");
        }else{
            fprintf(ComInterface, "ERR - Wrong Length!\r");
        }
      }
    }

    return ProcessedPacket;
}

int ESTTC_getchar(ESTTC_InterfacesEnum Interface)
{
	int ch = -1;

	__disable_interrupt();

	if(RxBuffTail[Interface] != RxBuffHead[Interface])
	{
		ch = (BYTE)RxBuffer[Interface][RxBuffTail[Interface]];
		RxBuffTail[Interface] = (RxBuffTail[Interface] + 1) % UART_BUFFER_SIZE;
		RxBuffLen[Interface]--;
	}
	__enable_interrupt();

	return ch;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if( huart == &huart1 )
    {
        MX_USART1_UART_Init();
        HAL_UART_Receive_IT((UART_HandleTypeDef*)COMM, (uint8_t *)&rx_data_dummy, 1);
    }else if( huart == &huart4 )
    {
        MX_UART4_Init();
        HAL_UART_Receive_IT((UART_HandleTypeDef*)PAYLOAD, (uint8_t *)&rx_data_dummy, 1);
    }else if( huart == &huart6 )
    {
        MX_USART6_UART_Init();
        HAL_UART_Receive_IT((UART_HandleTypeDef*)SYSCON, (uint8_t *)&rx_data_dummy, 1);
    }
}

/*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* INTERNAL (STATIC) ROUTINES DEFINITION
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static int getbyte(uint32_t tmt_ms, ESTTC_InterfacesEnum Interface)
{
  int ch;

  if ((ch = ESTTC_getchar(Interface)) != -1) return ch;

  for (uint32_t i = 0; i < tmt_ms; i++)
  {
    osDelay(1);
    if ((ch = ESTTC_getchar(Interface)) != -1) break;
  }
  return ch;
}

static uint32_t GetPhrase(char *dst, uint32_t len, char term, ESTTC_InterfacesEnum Interface)
{
  int ch;

  if ((dst == NULL) || (len == 0)) return 0;

  while((ch = ESTTC_getchar(Interface)) != -1 )
  {
      dst[pack_data_position[Interface]] = (BYTE)ch;

      pack_data_position[Interface] = (pack_data_position[Interface] + 1) % LINE_BUFFER_SIZE;

      if(term == (BYTE)ch)
      {
          dst[pack_data_position[Interface]-1] = 0;
          pack_data_position[Interface] = 0;
          return 1;
      }
  }

  return 0;
}

static uint8_t HexToBin(uint8_t hb, uint8_t lb)
{
  uint8_t thb = hb, tlb = lb;

  if (thb > '9') thb += 9;
  if (tlb > '9') tlb += 9;

  return (thb << 4) + (tlb & 0x0f);
}

static uint8_t FLASH_PatternCheck(uint32_t addr, uint8_t pattern, uint32_t len)
{
  uint32_t i;

  if (addr < FLASH_MIN_ADDR) return HAL_ERROR;
  if ((addr + len) > (FLASH_MIN_ADDR+FLASH_SIZE)) return HAL_ERROR;

  for (i = addr; i < addr+len; i++)
  {
    if (pattern != *((uint8_t*)i))
    {
      return i;
    }
  }

  return HAL_OK;
}

static uint8_t FLASH_Write(uint8_t *src, uint32_t addr, uint32_t len)
{
  if ((src == NULL)||(addr < FLASH_MIN_ADDR)||((addr + len) > (FLASH_MIN_ADDR+FLASH_SIZE)))
    return HAL_ERROR;

  HAL_StatusTypeDef res = HAL_OK;
  uint32_t prog_size = FLASH_TYPEPROGRAM_DOUBLEWORD;
  uint32_t addr_inc = sizeof(uint64_t);

  {
    prog_size = FLASH_TYPEPROGRAM_BYTE;
    addr_inc = sizeof(uint8_t);
  }

  for (uint32_t i = 0; i < len; i += addr_inc)
  {
    res = HAL_FLASH_Program(prog_size, addr+i, src[i]);
    if (res != HAL_OK) break;
  }

  return res;
}

static void APP_InitCdCardFiles(void)
{
    uint32_t i, j;
    FRESULT appl_fd_result = FR_INVALID_PARAMETER;
    FIL df;

    if (FR_OK == (appl_fd_result = f_mount(&SD_Fs, "0", 1)))
      fprintf(SYSCON, "SD card mounted OK.\r");
    else
      fprintf(SYSCON, "SD card fail err=%u.\r", appl_fd_result);

    HAL_RTC_GetTime(&hrtc, &sTime, calendar_format);
    HAL_RTC_GetDate(&hrtc, &sDate, calendar_format);

    appl_fd_result = f_open(&df, "0:/sys.log", FA_WRITE | FA_READ | FA_OPEN_ALWAYS /*| FA_OPEN_APPEND*/);   //VMI /*| FA_OPEN_APPEND*/


    if( FR_OK == appl_fd_result )
    {
        appl_fd_result = f_lseek(&df, df.fsize);

        if( FR_OK == appl_fd_result )
        {
            if( FR_OK == appl_fd_result )
            {
                i = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0);
                j = *((__IO uint32_t*)MAILBOX_ADDRESS);
                fprintf((FILE *)&df, ">%4d/%02d/%02d %2d:%02d:%02d > RTC_REG=%04X%04X MAILBOX_REG=%04X%04X ",
                        sDate.Year, sDate.Month, sDate.Date,
                        sTime.Hours, sTime.Minutes, sTime.Seconds,
                        (uint16_t)(i >> 16),(uint16_t)(i),
                        (uint16_t)(j >> 16),(uint16_t)(j));
                if (*((__IO uint32_t*)MAILBOX_ADDRESS) == MAILBOX_VAL_APPL)
                {
                    fprintf((FILE *)&df, "SW version: Appl %d.%02d <%s %s>\r",
                          verFW_MAJOR_REV_NB, verFW_MINOR_REV_NB, __DATE__, __TIME__);
                }
                else
                {
                  fprintf((FILE *)&df, "SW version: Boot %d.%02d <%s %s>\r",
                          verFW_MAJOR_REV_NB, verFW_MINOR_REV_NB, __DATE__, __TIME__);
                }
            }
            f_close(&df);
        }else{
          fprintf(SYSCON, "ERR+SYS.LOG(%u)\r", (uint16_t)appl_fd_result);
        }
    }
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

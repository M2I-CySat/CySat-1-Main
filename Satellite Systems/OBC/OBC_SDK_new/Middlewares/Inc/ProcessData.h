// header file ProcessData.h  -> to use copy -> #include "ProcessData.h"
#ifndef __PROCESS_header
#define __PROCESS_header

#include "main.h"

extern char update_flags;
extern UART_HandleTypeDef huart4;

#define UPDATE_FLASH_FLAG     0x01
#define UPDATE_T5_FLAG        0x02
#define UPDATE_T6_FLAG        0x04
#define UPDATE_T7_FLAG        0x08
#define UPDATE_T8_FLAG        0x10
#define UPDATE_DEFAULTS_FLAG  0x20
#define UPDATE_SOFTWARE_FLAG  0x80

extern void Send_16bit_to_UART(unsigned short data);
extern void Digits_to_UART(int data, char DigitNum);
extern void Measure_ADC_Channels(void);
extern void Send_Data_Thru_UART(void);
short I2C_Data_to_Send(char Data_Num);

#endif /* __PROCESS_header */
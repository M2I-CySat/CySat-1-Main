#ifndef ADCS_COM_H_

#define ADCS_COM_H_

#include "adcs_functions.h"
#include "stm32f4xx_hal.h"
#include "MCU_Init.h"
#include "main.h"

void Determine_Initial_Angular_Rates(void);
void Initial_Detumbling(void);
void Continued_Detumbling_To_Y_Thomson(void);
#endif

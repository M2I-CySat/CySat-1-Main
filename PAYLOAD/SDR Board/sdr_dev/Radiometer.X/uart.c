/*UART Functions
 * Written by Matthew E. Nelson
 *
 * This file contains the functions for using the UART module on the PIC32
 * These functions have been written mainly for the UBW32 Bitwhacker board
 * however, it should work with any PIC32_5/6/7_MX chips
 * These functions have been setup for use of the UART2 port
 */

/*

 * uart.c Function
 * Created 3/22/2012
 * Version 0.1
 * Last Revision
 */

/*

 * Changelog
 * 0.1 - Created file
 */

//Defines

//Input the Clock frequency of the PIC32
#define SYS_FREQ (80000000L)

//Includes - plib.h for UART functions
#include <plib.h>


//Functions
// *****************************************************************************
// void UARTTxBuffer(char *buffer, UINT32 size)
// *****************************************************************************
void SendDataBuffer(const char *buffer, UINT32 size)
{
    while(size)
    {
        while(!UARTTransmitterIsReady(UART2))
            ;

        UARTSendDataByte(UART2, *buffer);

        buffer++;
        size--;
    }

    while(!UARTTransmissionHasCompleted(UART2))
        ;
}
// *****************************************************************************
// UINT32 GetDataBuffer(char *buffer, UINT32 max_size)
// *****************************************************************************
UINT32 GetDataBuffer(char *buffer, UINT32 max_size)
{
    UINT32 num_char;

    num_char = 0;

    while(num_char < max_size)
    {
        UINT8 character;

        while(!UARTReceivedDataIsAvailable(UART2))
            ;

        character = UARTGetDataByte(UART2);

        if(character == '\r')
            break;

        *buffer = character;

        buffer++;
        num_char++;
    }

    return num_char;
}
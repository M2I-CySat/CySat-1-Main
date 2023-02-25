/*
 * File:   main.c
 * Author: Matthew Nelson
 *
 * Created on February 26th, 2012
 */

#include <p32xxxx.h>
#include <string.h>
#include <plib.h>

// External Osc with PLL
#pragma config POSCMOD = XT, FNOSC = PRIPLL
// SYSCLK 8MHz /2 *20 /1 = 80MHz
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_1
#define SYSCLK 80000000
// PBCLK 80MHz / 8 = 10MHz
#pragma config FPBDIV = DIV_1
#define PBCLK 80000000
// Disable secondary osc, clock out, clock switching, watchdog
#pragma config FSOSCEN = OFF, OSCIOFNC = OFF, FCKSM = CSDCMD, FWDTEN = OFF, CP = OFF, BWP = OFF

//These are the LEDs on the bitwhacker
#define YLED LATEbits.LATE0
#define RLED LATEbits.LATE1
#define WLED LATEbits.LATE2
#define GLED LATEbits.LATE3

//CS pin for ADL7466
#define ad7466_cs LATEbits.LATE4

// Constants
const int BAUDRATE = 9600;

//Write/Read from SPI Function
char writeSPI1( char i )
{
	SPI1BUF = i;			// write to buffer for TX
	while( !SPI1STATbits.SPIRBF );	// wait for TX complete
	return SPI1BUF;			// read the received values
} // END writeSPI2()

void setupSerial(void) {
    UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, PBCLK, BAUDRATE);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    SendDataBuffer("\n\r***Radiometer Serial port configured***\n\r", sizeof("\n\r***Radiometer Serial port configured***\n\r"));
    return;
}

char Req_7466_data(void) {
    INT data = 0;
    char upper = 0;
    char lower = 0;
    //Open the SPI port
    //Once the CS pin is driven low, the ADL7466 will send 16-bits of data
    //4 bits is the padded zeros, 12-bits of data.
    //OpenSPI1(SPI_MODE16_ON|SPI_SMP_ON|MASTER_ENABLE_ON|SEC_PRESCAL_1_1|PRI_PRESCAL_1_1, SPI_ENABLE);
    YLED = 0;
    ad7466_cs = 0;
    upper = writeSPI1(0);
    lower = writeSPI1(0);
    ad7466_cs = 1;
    //int isDataAvlbl;
    //isDataAvlbl = DataRdySPI1();
    //putcSPI1(0xFFFF);
    //putcSPI1(0xFF);
    //int data1=getcSPI1();
    //int data1 = SpiChnGetC(1);
    //CloseSPI1();
    YLED = 1;
    return lower;
}

int setup(void) {
    // Optimize system timings
    SYSTEMConfig(SYSCLK, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    // Setup interrupts
    //INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    //INTEnableInterrupts();
    // Setup LEDs, these pins need to be outputs
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    // Set all analog pins to be digital I/O
    AD1PCFG = 0xFFFF;
    //Configure SPI
    SpiChnOpen(1, SPI_CON_MSTEN | SPICON_CKE | SPI_CON_MODE8 | SPI_CON_ON, 200);
    PORTSetPinsDigitalIn(IOPORT_E, BIT_4);
    //PORTSetPinsDigitalIn(IOPORT_D, BIT_10);
    //PORTSetPinsDigitalIn(IOPORT_D, BIT_0);
    //PORTSetPinsDigitalIn(IOPORT_C, BIT_4);
    int rData = SPI1BUF;    //Clears receive buffer
    IFS0CLR = 0x03800000;   //Clears any existing event (rx / tx/ fault interrupt)
    SPI1STATCLR = 0x40;      //Clears overflow
    ad7466_cs = 1;          //Make sure CS pin is high

    YLED = 1;  //1 is off, 0 is on
    RLED = 1;
    WLED = 1;
    GLED = 1;
    setupSerial();
    SendDataBuffer("Radiometer Configuration Complete.\n\r", sizeof("Radiometer Configuration Complete.\n\r"));
    return 0;
}

int main(void) {
    UINT8   buf[1024];
    setup();
    SendDataBuffer("Beginning Main Loop...\n\r", sizeof("Beginning Main Loop...\n\r"));
    RLED = 0;
    unsigned int ticks;
    INT16 square_law = 0;
    char s[32];
    while(TRUE) {
        int data = 2000;
        SendDataBuffer("Reading Data...\n\r", sizeof("Reading Data...\n\r"));
        sprintf(buf, "test: %ld\r\n\r\n", data);
        SendDataBuffer(buf, strlen(buf));
        GLED = 0;  //Turn on Green LED
        square_law = Req_7466_data();
        sprintf(buf, "ADL7466: %ld\r\n\r\n", square_law);
        SendDataBuffer(buf, strlen(buf));
        
        //GLED = 1;
    }
    return 0;
}

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

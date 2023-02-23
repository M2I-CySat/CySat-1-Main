/*
 * File:   main.c
 * Author: Matthew Nelson
 *
 * Created on February 26th, 2012
 * Modified - March 19th, 2012
 * Version 0.31
 */

/*Pinout
 * V-pole - AN4 (B4)
 * H-pole - AN5 (B5)
 */

/*Changelog
 * 0.10 - Created program, initial function of reading A/D for Square law
 * 0.20 - Added RTC routine, need to add crystal for UBW32 board
 * 0.30 - Added Mass Storage Device
 * 0.31 - Modularized many of the functions to seperate files
 
 */

#include <p32xxxx.h>
#include <string.h>
#include <plib.h>

//The following configure the processor such as clock speed and other configuration bits
// External Osc with PLL
#pragma config POSCMOD = XT, FNOSC = PRIPLL
// SYSCLK 8MHz /2 *20 /1 = 80MHz
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20, FPLLODIV = DIV_1
#define SYSCLK 80000000
#define SYS_FREQ (80000000L)
// PBCLK 80MHz / 1 = 80MHz
#pragma config FPBDIV = DIV_1
#define PBCLK 80000000
// Disable secondary osc, clock out, clock switching, watchdog
#pragma config FSOSCEN = OFF, OSCIOFNC = OFF, FCKSM = CSDCMD, FWDTEN = OFF, CP = OFF, BWP = OFF

//These are the LEDs on the bitwhacker
#define YLED LATEbits.LATE0
#define RLED LATEbits.LATE1
#define WLED LATEbits.LATE2
#define GLED LATEbits.LATE3

//Defines for allowing us to clear the screen and returning to home position
#define clrscr() putsUART2("\x1b[2J")
#define home() putsUART2("\x1b[H")

// Constants
const int BAUDRATE = 19200;  //Baud rate for the serial port
//const int SAMPLES = 10000;   //How many samples to take and then average

//This function setups up the serial port
void setupSerial(void) {
    UARTConfigure(UART2, UART_ENABLE_HIGH_SPEED | UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART2, PBCLK, BAUDRATE);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    SendDataBuffer("\n\r***Radiometer Serial port configured***\n\r", sizeof("\n\r***Radiometer Serial port configured***\n\r"));
    return;
}

void setupADC(void) {
    // configure and enable the ADC
    CloseADC10();   // ensure the ADC is off before setting the configuration
    // define setup parameters for OpenADC10
    // Turn module on | ouput in integer | trigger mode auto | enable autosample
    #define PARAM1  ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON
    int PARAM1_2 = ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON;
    // define setup parameters for OpenADC10
    // ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode
    #define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON | ADC_ALT_INPUT_ON
    // define setup parameters for OpenADC10
    //  use ADC internal clock | set sample time
    #define PARAM3  ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15
    // define setup parameters for OpenADC10
    // set AN4 and AN5 as analog inputs
    #define PARAM4 ENABLE_AN4_ANA | ENABLE_AN5_ANA
    // define setup parameters for OpenADC10
    // do not assign channels to scan
    #define PARAM5 SKIP_SCAN_ALL
    // use ground as neg ref for A | use AN4 for input A | use ground as neg ref for A | use AN5 for input B
    // configure to sample AN4 & AN5
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN4 |  ADC_CH0_NEG_SAMPLEB_NVREF | ADC_CH0_POS_SAMPLEB_AN5);
    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 ); // configure ADC using the parameters defined above
    EnableADC10(); // Enable the ADC
}

//Setup routine
int setup(void) {
    rtccTime	tm, tm1;			// time structure
    rtccDate	dt, dt1;			// date structure
    //Set the date and time
    //RtccSetTimeDate(0x10073000, 0x07011602);
    //RtccInit();			// init the RTCC
    //while(RtccGetClkStat()!=RTCC_CLK_ON);	// wait for the SOSC to be actually running and RTCC to have its clock source
						// could wait here at most 32ms
    // Optimize system timings
    SYSTEMConfig(SYSCLK, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    
    // Setup LEDs, these pins need to be outputs
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    //Turn off all LEDs
    YLED = 1;  //1 is off, 0 is on
    RLED = 1;
    WLED = 1;
    GLED = 1;
    setupSerial();
    setupADC();
    SendDataBuffer("Radiometer Configuration Complete.\n\r", sizeof("Radiometer Configuration Complete.\n\r"));
    return 0;
}

// delay in microseconds function
void delay_us( int delay )
{
	// note that 1 core tick = 2 SYS cycles (this is fixed)
	int us_ticks=(SYS_FREQ/1000000)/2;
	WriteCoreTimer( 0 );
	while( ReadCoreTimer() < delay*us_ticks );
} // END delay_us()

float convert_adc(int value) {
    /*Square law detector is 53 mV per dB*/
    float a = 0;
    a = ((.322 * value)/ 5.3)-60;
    return a;
}

int main(void) {
    UINT8   buf[1024];
    setup();
    SendDataBuffer("Beginning Main Loop...\n\r", sizeof("Beginning Main Loop...\n\r"));
    delay_us(10000);
    RLED = 0;
    unsigned int ticks;
    INT16 square_law_v = 0;
    INT16 square_law_h = 0;
    int adc = 0;
    float pwr_v = 0;
    float pwr_h = 0;
    char s[32];
    clrscr();
    home();
    SendDataBuffer("Reading Data...\r\n", sizeof("Reading Data...\r\n"));
    while(TRUE) {
        //clrscr();
        //home();
        WLED = 1;  //Turn on White LED
        square_law_v = read_raw_v();
        square_law_h = read_raw_h();
        pwr_v = convert_adc(square_law_v);
        pwr_h = convert_adc(square_law_h);
        sprintf(buf, "$%ld,", square_law_v);
        SendDataBuffer(buf, strlen(buf));
        sprintf(buf, "%ld,", square_law_h);
        SendDataBuffer(buf, strlen(buf));
        sprintf(buf, "%3.4f,", pwr_v);
        SendDataBuffer(buf, strlen(buf));
        sprintf(buf, "%2.4f,#\n\r", pwr_h);
        SendDataBuffer(buf, strlen(buf));
        delay_us(100000);
        WLED = 0;
    }
    return 0;
}


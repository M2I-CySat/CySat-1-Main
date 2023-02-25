/*Square Law Detector Functions
 * Written by Matthew E. Nelson
 *
 * This file contains the functions for using getting data from the AD5902
 * Square Law detector.  For the most part, the AD5902 outputs an analog
 * voltage based on the input RF power to the detector.  In addition, routines
 * have been added to collect several samples and average them out.
 * These functions have been written mainly for the UBW32 Bitwhacker board
 * however, it should work with any PIC32_5/6/7_MX chips
 *
 */

/*

 * square_law.c Function
 * Created 3/22/2012
 * Version 0.1
 * Last Revision
 */

/*

 * Changelog
 * 0.1 - Created file
 */

//Defines

const int SAMPLES = 10000;   //How many samples to take and then average

//Input the Clock frequency of the PIC32
#define SYS_FREQ (80000000L)

//Includes - None should be needed
#include <plib.h>
//Functions

int read_raw_v(void)  {
    unsigned int channel4 = 0;
    //unsigned int channel5 = 0;
    unsigned int offset = 0;
    while ( ! mAD1GetIntFlag() ) {
        // wait for the first conversion to complete so there will be vaild data in ADC result registers
        }
    // the results of the conversions are available in channel4 and channel5
    offset = 8 * ((~ReadActiveBufferADC10() & 0x01));  // determine which buffer is idle and create an offset

    channel4 = ReadADC10(offset);  	// read the result of channel 4 conversion from the idle buffer
    //channel5 = ReadADC10(offset + 1);  	// read the result of channel 5 conversion from the idle buffer
    mAD1ClearIntFlag();
    return channel4;
}

int read_raw_h(void)  {
    //unsigned int channel4 = 0;
    unsigned int channel5 = 0;
    unsigned int offset = 0;
    while ( ! mAD1GetIntFlag() ) {
        // wait for the first conversion to complete so there will be vaild data in ADC result registers
        }
    // the results of the conversions are available in channel4 and channel5
    offset = 8 * ((~ReadActiveBufferADC10() & 0x01));  // determine which buffer is idle and create an offset

    //channel4 = ReadADC10(offset);  	// read the result of channel 4 conversion from the idle buffer
    channel5 = ReadADC10(offset + 1);  	// read the result of channel 5 conversion from the idle buffer
    mAD1ClearIntFlag();
    return channel5;
}

int avg_power_v(void)  {
    int a = 0;
    int j = 0;
    int b = 0;

    for ( j=0; j < SAMPLES; j++)  {
        a = a+read_raw_v();
        b = a/SAMPLES;
    }
    return b;
}

int avg_power_h(void)  {
    int a = 0;
    int j = 0;
    int b = 0;

    for ( j=0; j < SAMPLES; j++)  {
        a = a+read_raw_h();
        b = a/SAMPLES;
    }
    return b;
}

/*
float convert_adc(float value) {
    /*Square law detector is 53 mV per dB*/
/*
    float a = 0;
    a = ((.322 * value)/ 5.3)-60;
    return a;
}
*/
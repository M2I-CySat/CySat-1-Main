/*RTC Functions
 * Written by Matthew E. Nelson
 *
 * This file contains the functions for using the RTCC module on the PIC32
 * These functions have been written mainly for the UBW32 Bitwhacker board
 * however, it should work with any PIC32_5/6/7_MX chips
 * Please note that to use the RTCC module a 32.768 KHz crystal is required
 * please see the Microchip documentation on additional information
 */

/*

 * RTC.c Function
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

//Includes - None should be needed
#include <plib.h>
//Functions
/*********************************************************************
 * Function:        int CheckRtccRunning(int secWait)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          1(true) if test succeeded, 0(FALSE) otherwise
 *
 * Side Effects:    None
 *
 * Overview:        The function checks that the RTCC has the clock enabled and counts the time.
 *
 * Note:            None
 ********************************************************************/
int CheckRtccRunning(int secWait)
{
	#define	WAIT_FOR_SEC_TMO	1100			// how many ms to wait for the RTCC seconds count to change

	rtccTime	t0, t1;
	int		fail;
	int		secCnt;
	unsigned int	tStart;



	for(secCnt=0, fail=0; secCnt<secWait; secCnt++)
	{
		tStart=ReadCoreTimer();
		t0.l=RtccGetTime();
		do
		{
			t1.l=RtccGetTime();
		}while((t1.sec == t0.sec) && (ReadCoreTimer()-tStart) < (SYS_FREQ/2000)*WAIT_FOR_SEC_TMO);	// wait seconds change

		if(t1.sec==t0.sec)
		{
			fail=1;
			break;	// failed
		}
	}

	return !fail;
}

void SetTimeDate(int hr,int min, int sec, int month, int day, int year) {

}
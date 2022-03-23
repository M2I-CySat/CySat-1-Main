/*
 * adcs_commissioning_steps.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Xiangzhu Yan
 */

#include "adcs_commissioning_steps.h"

void Determine_Initial_Angular_Rates(void){
    //Send Telecommand
    TC_10(1);
    TC_11(1,1,0,0,0,0,0,0,0);
    TC_14(2);

    //Send telemetry request
//    TLM_140();
//    TLM_147();
//    TLM_155();
//    TLM_151();

}

void Initial_Detumbling(void){
    //Send Telecommand
    TC_10(1);
    TC_11(1,1,0,0,0,0,0,0,0);
    TC_14(2);

    //1 min delay
    HAL_Delay(60000);
    //Initial activation 1:
    TC_13(1,1,600);
    //Initial activation 2:
    TC_13(2,1,600);
    //Final activation:
    TC_13(2,1,600);

    //Send telemetry request
//    TLM_140();
//    TLM_147();
//    TLM_155();
//    TLM_151();
//    TLM_157();


}

void Continued_Detumbling_To_Y_Thomson(void){
    //Send Telecommand
    TC_10(1);
    TC_11(1,1,0,0,0,0,0,0,0);
    TC_14(2);
    TC_13(0,1,0);

    //First attempt
    //Second attempt
    //Third attempt
    TC_7(2);
    TC_7(5);
    TC_7(10);
    TC_33(0,0,0);//TODO
    TC_63();
//    TLM_140();
//    TLM_147();
//    TLM_155();
//    TLM_170();
//    TLM_172();

}

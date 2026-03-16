/*
 * initcpu.c
 *
 *  Created on: 16-Mar-2026
 *      Author: Arindam Roy
 */
#include"F28x_Project.h"

void init_cpu1(void)

{


    EALLOW;
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1; // Enables PIE
    EDIS;


}





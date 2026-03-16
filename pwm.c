/*
 * pwm.c
 *
 *  Created on: 14-Mar-2026
 *      Author: Arindam Roy
 */
#include "F28x_Project.h"

void pwm(void)
{
    EALLOW;

    // Connect EPWM1 to CPU1
    DevCfgRegs.CPUSEL0.bit.EPWM1 = 0;

    // Disable TB clock during configuration
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    // Enable clock to EPWM1 module
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;

    // EPWM clock = SYSCLK = 120 MHz
    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 0;

    // Time base clock configuration
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;

    // Up count mode
    EPwm1Regs.TBCTL.bit.CTRMODE = 0;

    // 20kHz PWM
    // TBPRD = (120MHz / 20kHz) - 1 = 5999
    EPwm1Regs.TBPRD = 5999;

    // 50% duty cycle
    EPwm1Regs.CMPA.bit.CMPA = 3000;

    // Action qualifier
    EPwm1Regs.AQCTLA.bit.ZRO = 2;   // Set high at counter = 0
    EPwm1Regs.AQCTLA.bit.CAU = 1;   // Clear at CMPA

    // Shadow register configuration
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = 0;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = 0;

    // Enable ADC trigger SOCA
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETSEL.bit.SOCASEL = 4;

    // Enable ADC trigger SOCB
    EPwm1Regs.ETSEL.bit.SOCBEN = 1;
    EPwm1Regs.ETSEL.bit.SOCBSEL = 4;

    // Generate trigger every event
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;
    EPwm1Regs.ETPS.bit.SOCBPRD = 1;

    // Enable time base clock
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;

    EDIS;
}

void addDeadband(void)
{
    EALLOW;

    // Enable deadband
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;

    // Active high complementary
    EPwm1Regs.DBCTL.bit.POLSEL = 2;

    // Use EPWM1A as source
    EPwm1Regs.DBCTL.bit.IN_MODE = 0;

    // Deadtime calculation
    // SYSCLK = 120 MHz
    // 1us = 120 clock cycles

    EPwm1Regs.DBRED.bit.DBRED = 120;
    EPwm1Regs.DBFED.bit.DBFED = 120;

    EDIS;
}






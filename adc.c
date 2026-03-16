/*
 * adc.c
 *
 *  Created on: 14-Mar-2026
 *      Author: Arindam Roy
 */
#include "F28x_Project.h"

volatile Uint16 protection_index = 0;

volatile float voltage_output[8];
volatile float voltage_input[8];
volatile float voltage_dcbus[8];
volatile float current[8];

volatile float voltage_output_avg = 0;
volatile float voltage_input_avg = 0;
volatile float voltage_dcbus_avg = 0;
volatile float current_avg = 0;

volatile Uint16 protection_state_overvoltage = 0;
volatile Uint16 protection_counter_overvoltage = 0;

float voltage_limit = 3000;
Uint16 counter_limit = 5;

Uint16 fault = 0;
Uint16 reset = 0;

__interrupt void adcISR(void);
void protection(void);
void adc(void)
{
    EALLOW;

    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;

    AdcaRegs.ADCCTL2.bit.RESOLUTION = 0;
    AdcaRegs.ADCCTL2.bit.SIGNALMODE = 0;
    AdcaRegs.ADCCTL2.bit.PRESCALE = 7;

    AdcbRegs.ADCCTL2.bit.RESOLUTION = 0;
    AdcbRegs.ADCCTL2.bit.SIGNALMODE = 0;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 7;

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 2;  // pin 29 to 3.3v
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 59;

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 4; //pin 69 to 3.3v
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5;
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 59;

    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 2;  // pin 28 to 3.3v
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = 59;

    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 5; //pin 65 to 3.3v
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 5;
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = 59;

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    IER |= M_INT1;

    PieVectTable.ADCA1_INT = &adcISR;

    EDIS;
}
__interrupt void adcISR(void)
{


    voltage_dcbus[protection_index] =
        AdcaResultRegs.ADCRESULT0;

    voltage_output[protection_index] =
        AdcaResultRegs.ADCRESULT1;

    current[protection_index] =
        AdcbResultRegs.ADCRESULT0;

    voltage_input[protection_index] =
        AdcbResultRegs.ADCRESULT1;

    if(protection_index == 7)
    {
        protection_index = 0;

        int i;

        voltage_output_avg = 0;
        voltage_input_avg = 0;
        voltage_dcbus_avg = 0;
        current_avg = 0;

        for(i=0;i<8;i++)
        {
            voltage_output_avg += voltage_output[i];
            voltage_input_avg += voltage_input[i];
            voltage_dcbus_avg += voltage_dcbus[i];
            current_avg += current[i];
        }

        voltage_output_avg /= 8;
        voltage_input_avg /= 8;
        voltage_dcbus_avg /= 8;
        current_avg /= 8;

        protection();
    }
    else
    {
        protection_index++;
    }
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}
void protection(void)
{
    switch(protection_state_overvoltage)
    {
        case 0:

            protection_counter_overvoltage = 0;

            if(voltage_dcbus_avg > voltage_limit)
            {
                protection_state_overvoltage = 1;
            }

        break;

        case 1:

            if(voltage_dcbus_avg > voltage_limit)
            {
                protection_counter_overvoltage++;

                if(protection_counter_overvoltage
                   >= counter_limit)
                {
                    protection_state_overvoltage = 2;
                    fault = 1;
                }
            }
            else
            {
                protection_state_overvoltage = 0;
                protection_counter_overvoltage = 0;
            }

        break;

        case 2:

            if(reset == 1)
            {
                fault = 0;
                protection_state_overvoltage = 0;
            }

        break;
    }
}




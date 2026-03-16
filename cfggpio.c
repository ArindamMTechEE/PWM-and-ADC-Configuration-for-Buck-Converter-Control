#include"F28x_Project.h"

void cfg_gpio(void)
{
    EALLOW;


    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;    // EPWM1A pin 40
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;


    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;    // EPWM2A pin 41
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;

    EDIS;
}






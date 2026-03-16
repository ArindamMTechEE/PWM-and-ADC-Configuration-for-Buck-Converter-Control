#include"F28x_Project.h"

void dis_wdg(void)

{
    EALLOW;
    WdRegs.SCSR.bit.WDENINT = 1; // watchdog reset output signal is disabled
    WdRegs.WDCR.bit.WDDIS = 1; // disables the watchdog timer
    WdRegs.WDCR.bit.WDPS = 5; // pre-scalar of 16
    EDIS;
}

void cfg_wdg(void)
{
    EALLOW;
    WdRegs.WDKEY.bit.WDKEY = 0x55;
    WdRegs.WDKEY.bit.WDKEY = 0xAA; // watchdog timer periodic reset

    WdRegs.WDCR.bit.WDDIS = 0; // enabling watchdog timer

    EDIS;
}

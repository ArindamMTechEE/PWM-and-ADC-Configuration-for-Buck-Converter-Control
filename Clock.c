#include"F28x_Project.h"

void clk(void)

{
    EALLOW;

    ClkCfgRegs.CLKSRCCTL1.bit.OSCCLKSRCSEL = 01; // selecting the external clock
    ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 0; // bypass the PLL, clock feed from OSCCLK
    ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV =0; // divide by 1
    ClkCfgRegs.SYSPLLMULT.bit.IMULT = 12; // multiply by 12
    int i;
    for(i=0;i<=5;i++)
        {
        ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1; //running the loop for PLL locking
        }

    ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = 01;
    ClkCfgRegs.SYSPLLCTL1.bit.PLLCLKEN = 1;
    ClkCfgRegs.SYSCLKDIVSEL.bit.PLLSYSCLKDIV = 00;
    EDIS;
}

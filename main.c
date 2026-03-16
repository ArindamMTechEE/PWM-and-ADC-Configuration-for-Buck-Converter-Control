#include "F28x_Project.h"

extern void clk(void);
extern void dis_wdg(void);
extern void cfg_wdg(void);
extern void cfg_gpio(void);
extern void pwm(void);
extern void addDeadband(void);
__interrupt void adcISR(void);
extern void adc(void);
extern void protection(void);
extern void init_cpu1(void);

void main(void)
{
    DINT;
    clk();
    dis_wdg();
    cfg_wdg();
    cfg_gpio();
    pwm();
    addDeadband();
    __interrupt void adcISR(void);
    adc();
    protection();
    init_cpu1();

    EINT;

    while(1);
}









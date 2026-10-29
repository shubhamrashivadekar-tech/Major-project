#include<lpc21xx.h>
#include"header.h"


//pin 21 for the servo motor change  it based

void servo_init(void)
{
        PINSEL1 |=(1<<10);

        PWMPR=14;
        PWMMR0=20000;
        PWMMR5=1500;

        PWMMCR=1<<1;
        PWMPCR=1<<13;   //PWM5 output enable
        PWMLER=(1<<0)|(1<<5);
        PWMTCR=(1<<0)|(1<<3);
}

void servo_0(void)
{
        PWMMR5=1000;
        PWMLER=(1<<4);
}

void servo_90(void)
{
        PWMMR5=1500;
        PWMLER=(1<<4);
}

void servo_180(void)
{
        PWMMR5=2000;
        PWMLER=(1<<4);
}


void gate_operation(void)
{
        servo_180();
        servo_180();
        delay_ms(100);
        uart0_tx_string("\r\nGATE OPEN\r\n");
        delay_sec(3); //how much time the gate should be open
        uart0_tx_string("\r\nGATE CLOCSE\r\n");
        servo_0();
        servo_0();
        delay_ms(100);
}
#include"header.h"

extern int ext0;
extern int ext1;
extern int ext2;

void EINT0_handler(void)__irq
{
        ext0=ext0^1;
        EXTINT=1;
        VICVectAddr=0;
}

void EINT1_handler(void)__irq
{
        ext1=ext1^1;
        EXTINT|=1<<3;
        VICVectAddr=0;
}


void config_eint0(void)
{
        PINSEL1|=1;
        EXTMODE=1;
        EXTPOLAR=0;
        VICIntSelect=0;
        VICVectCntl2=14|(1<<5);
        VICVectAddr2=(unsigned int)EINT0_handler;
        VICIntEnable=(1<<14);
}

void config_eint1(void)
{
        PINSEL1|=0x0300;
        EXTMODE |=1<<3;
        EXTPOLAR =0;
        VICIntSelect=0;
        VICVectCntl3=17|(1<<5);
        VICVectAddr3=(unsigned int)EINT1_handler;
        VICIntEnable=(1<<17);
}
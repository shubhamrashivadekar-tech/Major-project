#include"header.h"

void uart1_init(unsigned int baud)
{
          int result,pclk;
        if(VPBDIV==0)
                pclk=15000000;
        else if(VPBDIV==1)
                pclk=60000000;
        else if(VPBDIV==2)
                pclk=30000000;
        PINSEL0 |=0x050000;

        result=pclk/(16*baud);
        U1LCR=0x83;
        U1DLL=result&0xff;
        U1DLM=(result>>8)&0xff;
        U1LCR=0x03;

}

#define RDR (U1LSR&1)
unsigned char uart1_rx(void)
{
        while(RDR==0);
        return U1RBR;
}

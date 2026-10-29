#include<lpc21xx.h>
#include"header.h"

void uart0_init(unsigned int baud)
{
        int result,pclk;
        if(VPBDIV==0)
                pclk=15000000;
        else if(VPBDIV==1)
                pclk=60000000;
        else if(VPBDIV==2)
                pclk=30000000;
        PINSEL0 |=0x05;

        result=pclk/(16*baud);
        U0LCR=0x83;
        U0DLL=result&0xff;
        U0DLM=(result>>8)&0xff;
        U0LCR=0x03;

}

#define THRE ((U0LSR>>5)&1)
void uart0_tx(unsigned char data)
{
        U0THR=data;
        while(THRE==0);
}

#define RDR (U0LSR&1)
unsigned char uart0_rx(void)
{
        while(RDR==0);
        return U0RBR;
}

//uart0 string
void uart0_tx_string(unsigned char * ptr)
{
        while(*ptr)
        {
                uart0_tx(*ptr);
                ptr++;
        }
}

void uart0_integer(int num)
{
        int i=0;
        char a[10];
        if(num==0)
        {
                uart0_tx('0');
                return;
        }
        if(num<0)
        {
                num=-num;
                uart0_tx('-');
        }

        while(num)
        {
                a[i]=num%10 +48;
                i++;
                num=num/10;
        }

        for(i=i-1;i>=0;i--)
                uart0_tx(a[i]);
}

extern int ext0;
extern int ext1;
extern int ext2;
void uart0_rx_string(unsigned char *ptr ,int len)
{
        int i;
        for(i=0;i<len;i++)
        {
        // && ext0!=1 && ext1!=1 && ext2!=1
                while(RDR==0 && ext0!=1 && ext1!=1 && ext2!=1);
                ptr[i]=U0RBR;
                if(ptr[i]=='\r')
                        break;
                if(ext0==1 || ext1==1 || ext2==1)
                        break;
        }
        ptr[i]='\0';
}

void bcd_uart0(unsigned char value)
{
        uart0_tx((value>>4)+'0');
        uart0_tx((value&0x0f)+'0');
}

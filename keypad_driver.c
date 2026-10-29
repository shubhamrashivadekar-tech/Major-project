#include<lpc21xx.h>
#include"header.h"

#define R1 (1<<8)
#define R2 (1<<9)
#define R3 (1<<10)
#define R4 (1<<11)

#define C1 (1<<12)
#define C2 (1<<13)
#define C3 (1<<14)
#define C4 (1<<15)


void keypad_init(void)
{
        IODIR0 |=(R1|R2|R3|R4);         //OUT DIR
        IODIR0 &=~(C1|C2|C3|C4);   //INPUT DIR
}

char keypad_read(void)
{
                        while(1)
                        {
                                IOSET0 =R2|R3|R4;
                                IOCLR0 =R1;

                                if((IOPIN0&C1)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C1)==0);
                                        return '1';
                                }
                                if((IOPIN0&C2)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C2)==0);
                                        return '2';
                                }
                                if((IOPIN0&C3)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C3)==0);
                                        return '3';
                                }
                                if((IOPIN0&C4)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C4)==0);
                                        return 'A';
                                }

                                IOSET0 =R1|R3|R4;
                                IOCLR0 =R2;

                                if((IOPIN0&C1)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C1)==0);
                                        return '4';
                                }
                                if((IOPIN0&C2)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C2)==0);
                                        return '5';
                                }
                                if((IOPIN0&C3)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C3)==0);
                                        return '6';
                                }
                                if((IOPIN0&C4)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C4)==0);
                                        return 'B';
                                }

                                IOSET0 =R1|R2|R4;
                                IOCLR0 =R3;

                                if((IOPIN0&C1)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C1)==0);
                                        return '7';
                                }
                                if((IOPIN0&C2)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C2)==0);
                                        return '8';
                                }
                                if((IOPIN0&C3)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C3)==0);
                                        return '9';
                                }
                                if((IOPIN0&C4)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C4)==0);
                                        return 'C';
                                }

                                IOSET0 =R1|R2|R3;
                                IOCLR0 =R4;

                                if((IOPIN0&C1)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C1)==0);
                                        return '*';
                                }
                                if((IOPIN0&C2)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C2)==0);
                                        return '0';
                                }
                                if((IOPIN0&C3)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C3)==0);
                                        return '#';
                                }
                                if((IOPIN0&C4)==0)
                                {
                                        delay_ms(20);
                                        while((IOPIN0&C4)==0);
                                        return 'D';
                                }
                        }
}


int keypad_verify(void)
{
                char real_pass[5];
                //read saved pass word through eeprom
                char a[5];
                int i;
                for(i=0;i<4;i++)
                {
                        real_pass[i]=i2c_read(0xa1,i);
                }
                for(i=0;i<4;i++)
                {
                        //a[i]=keypad_read();
                        //lcd_data('*');
                        a[i]=uart0_rx();
                        if(a[i]=='\r')
                                break;
                        uart0_tx(a[i]);
                }
                a[i]='\0';
                //uart0_rx_string(a,5);
                if(strcmp(a,real_pass)==0)
                        return 1;
                else
                        return 0;
}
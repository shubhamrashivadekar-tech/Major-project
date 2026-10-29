#include<lpc21xx.h>
#include"header.h"

void lcd_data(unsigned char data)
{
        unsigned int result;
        //higher nibble
        IOCLR1=0xfe<<16;
        result=(data&0xf0)<<16;
        IOSET1=result;
        IOSET1=1<<17;
        IOCLR1=1<<18;
        IOSET1=1<<19;
        delay_ms(2);
        IOCLR1=1<<19;

        //lower nibble
        IOCLR1=0xfe<<16;
        result=(data&0x0f)<<20;
        IOSET1=result;
        IOSET1=1<<17;
        IOCLR1=1<<18;
        IOSET1=1<<19;
        delay_ms(2);
        IOCLR1=1<<19;
}

void lcd_cmd(unsigned char cmd)
{
        unsigned int result;
        //higher nibble
        IOCLR1=0xfe<<16;
        result=(cmd&0xf0)<<16;
        IOSET1=result;
        IOCLR1=1<<17;
        IOCLR1=1<<18;
        IOSET1=1<<19;
        delay_ms(2);
        IOCLR1=1<<9;

        //lower nibble
        IOCLR1=0xfe<<16;
        result=(cmd&0x0f)<<20;
        IOSET1=result;
        IOCLR1=1<<17;
        IOCLR1=1<<18;
        IOSET1=1<<19;
        delay_ms(2);
        IOCLR1=1<<19;
}

void lcd_init(void)
{
        IODIR1=0xfe<<16;
        PINSEL2 |=0;
        lcd_cmd(0x02);
        lcd_cmd(0x28);
        lcd_cmd(0x0e);
        lcd_cmd(0x01);

}

void lcd_string(char * ptr)
{
        while(*ptr)
        {
                lcd_data(*ptr);
                ptr++;
        }
}

void lcd_integer(int num)
{
        int i=0;
        char a[10];
        if(num==0)
        {
                lcd_data('0');
                return ;
        }
        if(num<0)
        {
                num=-num;
                lcd_data('-');
        }

        while(num)
        {
                a[i]=num%10 +48;
                i++;
                num=num/10;
        }

        for(i=i-1;i>=0;i--)
        {
                lcd_data(a[i]);
        }
}

void bcd_lcd(unsigned char value)
{
        lcd_data((value>>4)+'0');
        lcd_data((value&0x0f)+'0');
}
~
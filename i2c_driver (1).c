#include<lpc21xx.h>
#include"header.h"

typedef unsigned char u8;

void i2c_init(void)
{
        PINSEL0 |=0x50;
        I2CONSET=(1<<6);
        I2SCLH=75;
        I2SCLL=75;
}

#define SI ((I2CONSET>>3)&1)

void i2c_write(u8 sa,u8 mr ,u8 data)
{
        I2CONSET=(1<<5);
        I2CONCLR=(1<<3);
        while(SI==0);
        I2CONCLR=(1<<5);

        //SA + W + ACk
        I2DAT=sa;
        I2CONCLR=(1<<3);
        while(SI==0);
        if(I2STAT==0x20)
        {
                uart0_tx_string("ERR : SA + W\r\n");
                goto exit;
        }

        //MR + ACk
        I2DAT=mr;
        I2CONCLR=(1<<3);
        while(SI==0);
        if(I2STAT==0x30)
        {
                uart0_tx_string("ERR :Memory addr \r\n");
                goto exit;
        }

        //data +ack
        I2DAT=data;
        I2CONCLR=(1<<3);
        while(SI==0);
        if(I2STAT==0x30)
        {
                uart0_tx_string("ERR : ");
                goto exit;
        }

        //stop condition
        exit:
                I2CONSET=(1<<4);
                I2CONCLR=(1<<3);

}


unsigned char i2c_read(u8 sa,u8 mr)
{
        u8 temp;
        //dummy write
        //start condition
        I2CONSET=(1<<5);
        I2CONCLR=(1<<3);
        while(SI==0);
        I2CONCLR=(1<<5);

        //SA + W + ACk
        I2DAT=sa^1;
        I2CONCLR=(1<<3);
        while(SI==0);
        if(I2STAT==0x20)
        {
                uart0_tx_string("ERR : SA + W\r\n");
                goto exit;
        }

        //MR + ACk
        I2DAT=mr;
        I2CONCLR=(1<<3);
        while(SI==0);
        if(I2STAT==0x30)
        {
                uart0_tx_string("ERR :Memory addr \r\n");
                goto exit;
        }

        //restart
        //start condition
        I2CONSET=(1<<5);
        I2CONCLR=(1<<3);
        while(SI==0);
        I2CONCLR=(1<<5);

        //SA + R + ACk
        I2DAT=sa;
        I2CONCLR=(1<<3);
        while(SI==0);
        if(I2STAT==0x48)
        {
                uart0_tx_string("ERR : SA + W\r\n");
                goto exit;
        }

        I2CONCLR=(1<<3);
        while(SI==0);
        temp=I2DAT;

        exit:
                I2CONSET=(1<<4);
                I2CONCLR=(1<<3);
        return temp;
}


void set_eeprom(void)
{
        int i,ma;
        char s[13]="4900C9C08FCF";
        char pass1[5]="1234",pass2[5]="1122";

        /* RFID saved at page 0 (SA->0xA0,MA->0,data)
                total 12 bytes
        */
        for(i=0;i<12;i++)
        {
                i2c_write(0xA0,i,s[i]);
                delay_ms(20);
        }

        /* key pad pass saved at page 0 (SA->0xA0,MA->126,data)(126 to 130)
                total 4 bytes
        */
        ma=126;
        for(i=0;i<4;i++)
        {
                   i2c_write(0xA0,ma,pass1[i]);
                   delay_ms(20);
                   ma++;
        }
        /* key pad pass saved at page 0 (SA->0xA0,MA->130,data)(130 to 134)
                total 4 bytes
        */
        for(i=0;i<4;i++)
        {
                   i2c_write(0xA0,ma,pass2[i]);
                   delay_ms(20);
                   ma++;
        }

}


//for setting time to the rtc
void set_rtc(void)
{
        i2c_write(0xd0,0x00,0x22); //sec
        i2c_write(0xd0,0x01,0x12);      //min
        i2c_write(0xd0,0x02,0x04);  //hr
        i2c_write(0xd0,0x04,0x13);  //date
        i2c_write(0xd0,0x05,0x10);  //month
        i2c_write(0xd0,0x06,0x26);  //year
}


//reading time from the rtc module
RTC t1;
void read_rtc(void)
{
           t1.sec=i2c_read(0xD1,0x00);
           //t1.sec=t1.sec&0x7f;
           t1.min=i2c_read(0xD1,0x01);
           t1.hr=i2c_read(0xD1,0x02);
           //t1.hr=t1.hr&0x3f;
           t1.day=i2c_read(0xD1,0x03);
           t1.date=i2c_read(0xD1,0x04);
           t1.month=i2c_read(0xD1,0x05);
           t1.year=i2c_read(0xD1,0x06);
}

//printing the rtc data
void print_RTC(char *ptr)
{
        unsigned char sec;
        unsigned char min;
        unsigned char hr;

        unsigned char date;
        unsigned char month;
        unsigned char year;

        sec=ptr[0] & 0x7f; ///?
        min=ptr[1] & 0x7f;
        hr=ptr[2] & 0x3f;

        date=ptr[4];
        month=ptr[5];
        year=ptr[6];

        uart0_tx_string("\r\nDate : ");
        bcd_uart0(date);
        uart0_tx('/');
        bcd_uart0(month);
        uart0_tx('/');
        bcd_uart0(year);

        uart0_tx_string("\r\nTime : ");
        bcd_uart0(hr);
        uart0_tx(':');
        bcd_uart0(min);
        uart0_tx(':');
        bcd_uart0(sec);

        lcd_cmd(0x01);
        lcd_cmd(0x80);
        lcd_string("D:");
        bcd_lcd(date);
        lcd_data(':');
        bcd_lcd(month);
        lcd_data(':');
        bcd_lcd(year);

        lcd_cmd(0xC0);
        lcd_string("T:");
        bcd_lcd(hr);
        lcd_data(':');
        bcd_lcd(min);
        lcd_data(':');
        bcd_lcd(sec);
         return ;
}

void print_rtc(void)
{
        char rt[8];
        read_rtc();
        rt[0]=t1.sec;
        rt[1]=t1.min;
        rt[2]=t1.hr;
        rt[3]=t1.day;
        rt[4]=t1.date;
        rt[5]=t1.month;
        rt[6]=t1.year;
        print_RTC(rt);
}

//save authentication data
int sda=256;
void success_data(void)
{
        unsigned char data[8];
        int i;
        data[0]=t1.sec;
        data[1]=t1.min;
        data[2]=t1.hr;
        data[3]=t1.day;
        data[4]=t1.date;
        data[5]=t1.month;
        data[6]=t1.year;

        for(i=0;i<7;i++)
        {
                i2c_write(0xA2,sda,data[i]);
                delay_ms(20);
                sda++;

        }

}

//failure dara store
int fda=512;
void failure_data(void)
{
   unsigned char data[8];
        int i;
        data[0]=t1.sec;
        data[1]=t1.min;
        data[2]=t1.hr;
        data[3]=t1.day;
        data[4]=t1.date;
        data[5]=t1.month;
        data[6]=t1.year;

        for(i=0;i<7;i++)
        {
                i2c_write(0xA4,fda,data[i]);
                delay_ms(20);
                fda++;

        }
}

void printSuccessData(void)
{
        unsigned char c[8];
        unsigned int read_addr=256;
        int i;
        uart0_tx_string("\r\n******SUCCESS AUTHENTICATION LOG*******\r\n");
        while(read_addr<sda)
        {
                for(i=0;i<7;i++)
                {
                        c[i]=i2c_read(0xA3,read_addr);
                        read_addr++;
                }
                print_RTC(c);
                delay_ms(100);
        }
}

void printFailureData(void)
{
  unsigned char c[8];
        unsigned int read_addr=512;
        int i;
        uart0_tx_string("\r\n******FAILURE AUTHENTICATION LOG*******\r\n");
        while(read_addr<fda)
        {
                for(i=0;i<7;i++)
                {
                        c[i]=i2c_read(0xA3,read_addr);
                        read_addr++;
                }
                print_RTC(c);
                delay_ms(100);
        }
}

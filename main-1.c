#include<lpc21xx.h>
#include"header.h"

#define PIR (IOPIN0>>4)
#define GREEN_LED (1<<5)
#define RED_LED (1<<6)

#define BUZZ (1<<7)

int ext0=0;     //for external interrupt
int ext1=0;     //for external interrupt
int ext2=0;     //for external interrupt

int main()
{

        int c=0,i;
        char ch;
        int f=0;
        lcd_init();
        uart0_init(9600);
        //uart1_init(9600);
        i2c_init();
        //keypad_init();
        servo_init();
        config_eint0();
        config_eint1();
        IODIR0 |=GREEN_LED|RED_LED|BUZZ;
        IOSET0 =GREEN_LED|RED_LED|BUZZ;

        uart0_tx_string("\r\n-------system started--------\r\n");
        lcd_string("SMART BUILDING");

        uart0_tx_string("STORING USER PREDEFINED DATA\r\n");
        set_eeprom();
        uart0_tx_string("STORING compleated\r\n");

        uart0_tx_string("Data in the eeprom\r\n");

        for(i=0;i<12;i++)
        {
                ch=i2c_read(0xa1,i);
                uart0_tx(ch);
        }
        uart0_tx_string("\r\n");
        set_rtc();
        uart0_tx_string("RTC data and time set\r\n");

        read_rtc();
        print_rtc();
        uart0_tx_string("Monitoring satrted\r\n");
        while(1)
        {
                //c=0;
                lcd_cmd(0x01);
                lcd_string("Monitoring...");
                if(((PIR&1)==1) && ext0==0 && ext1==0 && ext2==0)               //when motion detected
                {
                        lcd_cmd(0x01);
                        lcd_string("Motion Detected");
                        uart0_tx_string("Movement detected\r\n");
                        delay_ms(10);
                        while(c<2)
                        {
                                if(ext0!=0 || ext1!=0 || ext2!=0 )
                                        break;
                                lcd_cmd(0x01);
                                lcd_string("RFID Ready");
                                lcd_cmd(0xc0);
                                lcd_string("Scan Card");
                                uart0_tx_string("RFID is ready\r\n");
                                uart0_tx_string("SCAN YOUR CARD...\r\n");
                                if(rfid_read())
                                {
                                                if(ext0!=0 || ext1!=0 || ext2!=0 )
                                                        break;
                                                uart0_tx_string("\r\nRFID Authentication Successful...\r\n");
                                                lcd_cmd(0x01);
                                                lcd_string("Access Granted");
                                                IOCLR0=GREEN_LED;
                                                delay_ms(700);
                                                IOSET0=GREEN_LED;
                                                f=1;
                                                goto  send;
                                }
                                else
                                {
                                        if(ext0!=0 || ext1!=0 || ext2!=0 )
                                                break;
                                        uart0_tx_string("\r\nRFID Authentication not Successful...\r\n");
                                        lcd_cmd(0x01);
                                        lcd_string("Access Denied");
                                        IOCLR0=RED_LED|BUZZ;
                                        delay_ms(700);
                                        IOSET0=RED_LED|BUZZ;
                                        c++;
                                }
                        }
                        c=0;
                        lcd_cmd(0x01);
                        lcd_string("KEYPAD Ready");
                        lcd_cmd(0xc0);
                        lcd_string("ENTER PASSWORD");
                        uart0_tx_string("KEYPAD is ready...\r\n");
                        while(c<2)
                        {
                                if(ext0!=0 || ext1!=0 || ext2!=0 )
                                        break;
                                uart0_tx_string("ENTER YOUR PASSWORD...\r\n");
                                if(keypad_verify())
                                {
                                                if(ext0!=0 || ext1!=0 || ext2!=0 )
                                                        break;
                                                uart0_tx_string("PASSWORD Authentication Successful...\r\n");
                                                lcd_cmd(0x01);
                                                lcd_string("Access Granted");
                                                IOCLR0=GREEN_LED;
                                                delay_ms(700);
                                                IOSET0=GREEN_LED;
                                                f=1;
                                                goto  send;
                                }
                                else
                                {
                                                if(ext0!=0 || ext1!=0 || ext2!=0 )
                                                        break;
                                                uart0_tx_string("PASSWORD Authentication NOT Successful...\r\n");
                                                lcd_cmd(0x01);
                                                lcd_string("Access Denied");
                                                IOCLR0=RED_LED|BUZZ;
                                                delay_ms(700);
                                                IOSET0=RED_LED|BUZZ;
                                                c++;
                                }
                        }
                        send:
                        if(f==1 && ext0==0 && ext1==0 && ext2==0)
                        {
                                c=0;
                                f=0;
                                success_data();
                                uart0_tx_string("Data saved successfully\r\n");
                                IOCLR0=GREEN_LED;
                                delay_ms(700);
                                IOSET0=GREEN_LED;
                                gate_operation();
                        }
                        else if(f==0 && ext0==0 && ext1==0 && ext2==0)
                        {
                                c=0;
                                f=0;
                                failure_data();
                                uart0_tx_string("Too many attempts security ALERT\r\n");
                                IOCLR0=RED_LED|BUZZ;
                                delay_ms(700);
                                IOSET0=RED_LED|BUZZ;
                                IOCLR0=BUZZ;
                        }
                }
                else if(ext0==1)
                {
                        uart0_tx_string("MANUAL DOOR MODE\r\n");
                        lcd_cmd(0x01);
                        lcd_string("MANUAL OPEN");
                        //open door
                        servo_180();
                        uart0_tx_string("MANUAL DOOR OPEN\r\n");
                        delay_ms(500);
                        uart0_tx_string("MANUAL DOOR CLOSE\r\n");
                        ext0=0;
                }
                else if(ext1==1)
                {
                        uart0_tx_string("Security Alarm Reset\r\n");
                        //buzzer reset
                        IOSET0=BUZZ;
                        ext1=0;
                }
                else if(ext2==1)
                {
                        uart0_tx_string("Emergency Mode Activated\r\n");
                        //open door
                }
        }
}

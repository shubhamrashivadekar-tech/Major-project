#include<lpc21xx.h>
#include<string.h>
typedef unsigned char u8;

//timer functions declerations

extern void delay_sec(unsigned int sec);
extern void delay_ms(unsigned int ms);

//lcd functions declerations
extern void lcd_data(unsigned char data);
extern void lcd_cmd(unsigned char cmd);
extern void lcd_init(void);
extern void lcd_integer(int num);
extern void lcd_string(char * ptr);
extern void bcd_lcd(unsigned char value);

//uart0 functions declerations
extern void uart0_init(unsigned int baud);
extern void uart0_tx(unsigned char data);
extern unsigned char uart0_rx(void);
extern void uart0_tx_string(unsigned char * ptr);
extern void uart0_integer(int num);
extern void uart0_rx_string(unsigned char *ptr ,int len);
extern void bcd_uart0(unsigned char value);

//uart1 functions declerations
extern void uart1_init(unsigned int baud);
extern unsigned char uart1_rx(void);

//RFID declerations
extern int rfid_read(void);
extern int rfid_verify(char * s);

//KEY PAD declerations
extern unsigned char key_read(void);
extern void keypad_init(void);
extern int keypad_verify(void);

typedef struct RTC
{
         unsigned char sec;
         unsigned char min;
         unsigned char hr;
         unsigned char day;
         unsigned char date;
         unsigned char month;
         unsigned char year;
}RTC;


//i2c header files
extern void i2c_init(void);
extern void i2c_write(u8 sa,u8 mr ,u8 data);
extern unsigned char i2c_read(u8 sa,u8 mr);
extern void set_eeprom(void);
extern void set_rtc(void);
extern void read_rtc(void);
extern void print_RTC(char *ptr);
extern void print_rtc(void);
extern void success_data(void);
extern void failure_data(void);
extern void printSuccessData(void);
void printFailureData(void);


//servo declerations
extern void servo_init(void);
extern void servo_0(void);
extern void servo_90(void);
extern void servo_180(void);
extern void gate_operation(void);




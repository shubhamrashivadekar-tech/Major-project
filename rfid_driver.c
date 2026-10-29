 #include<lpc21xx.h>
   #include"header.h"
   #include<string.h>
  
   int rfid_read(void)
   {
           unsigned char rec_id[13];
           //uart0_tx_string("\r\nrfid initialised..");
           uart0_rx_string(rec_id,12);
         uart0_tx_string("\r\n");
          uart0_tx_string(rec_id);
          if(rfid_verify(rec_id))
                  return 1;
          else
                  return 0;
  }
 
 int rfid_verify(char * s)
  {
        unsigned char stor_id[13]="4900C9C08FCF";
          //read from the eeprom and compare it with above received
          if(strcmp(s,stor_id)==0)
                 return 1;
          else
                  return 0;
  }
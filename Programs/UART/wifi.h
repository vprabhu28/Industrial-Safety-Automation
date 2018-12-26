#include <LPC21xx.H> 
#include "lcd.h"
#include "Serial.h"
#include "type.h"
#include "irq.h"
#include "uart.h"

#define WIFIPutSSerial(string) uart1_puts( string )
#define WIFIGetCSerial( )  uart1_getkey()
#define WIFIPutCSerial(chr) uart1_putc(chr)

void intWIFI( void );
unsigned char WIFI_D[20];
void putSLcd(unsigned char *st) {
	for( ;*st ;lcd_data_write(*st++) );
}

void recvResponse123(unsigned char *response)
{
    unsigned char ch;
    
  	while(WIFIGetCSerial() != '\n' );
    while( (ch = WIFIGetCSerial()) != '\r' )*response++ = ch;
	*response = '\0' ;       
}
void recvResponse(unsigned char *response)
{
    unsigned char ch;
    
    while(WIFIGetCSerial() != '\r' );
	while(WIFIGetCSerial() != '\n' );
    while( (ch = WIFIGetCSerial()) != '\r' )*response++ = ch;
	*response = '\0' ;       
}
void WIFI_CONNECT_Response(unsigned char *response)
{
    unsigned char ch;
    
//  while(WIFIGetCSerial() != '\r' );
	while(WIFIGetCSerial() != '\n' );
    while( (ch = WIFIGetCSerial()) != '\r' )*response++ = ch; 
	*response = '\0' ;       
}
void WIFI_RECEIVE_IP(unsigned char *response)
{
    unsigned char ch;
    
//  while(WIFIGetCSerial() != '\r' );
	while(WIFIGetCSerial() != '"' );
    while( (ch = WIFIGetCSerial()) != '"' )*response++ = ch; 
	*response = '\0' ;       
}
void recvResponse_DATA(unsigned char *response)
{
    unsigned char ch;
    
    while(WIFIGetCSerial() != '$' );
    while( (ch = WIFIGetCSerial()) != '*' ) *response++ = ch;
    *response = '\0' ;       
}
void Receive_Data(unsigned char *response)
{
    unsigned char ch,i=0;
    
  	while(WIFIGetCSerial() != ':' );
    while( (ch = WIFIGetCSerial()) != '$' )WIFI_D[i++] = ch;
	WIFI_D[i] = '\0' ; 
	      
}
void intWIFI( void )
{
	    unsigned char buff[60],cmd[20],temp[20];
		char ch;  
		int len;
	
	    
		WIFIPutCSerial('A');
		delay(1000);
		WIFIPutCSerial('T');
		delay(1000);
		WIFIPutCSerial('E');
		delay(1000);
		WIFIPutCSerial('0');
		delay(1000);
		WIFIPutSSerial("\r\n");
		recvResponse(buff);
		delay(1000);
		lcd_command_write(1);
	//	putSLcd(buff);
		
		lcd_command_write(0xc0);
		putSLcd("Connecting.");
		delay(1000);
	    WIFIPutSSerial("AT\r\n"); 	  
	    recvResponse(buff);
	
		lcd_command_write(0xc0);
		putSLcd("Connecting..");
	   	lcd_command_write(0x80);
	
	//	putSLcd(buff);
		lcd_command_write(0xc0);
		putSLcd("Connecting...");
		delay(5000);
	   
	    WIFIPutSSerial("AT+CWMODE=2\r\n"); // MULTIPLE MODE SELECTION    
	    recvResponse(buff);
		lcd_command_write(0x80);
	//	putSLcd(buff);
		lcd_command_write(0xc0);
		putSLcd("Connecting....");
		delay(5000);
	
	//WIFIPutSSerial("AT+CWJAP=\"rekha\",\"12345678\"\r\n"); // MULTIPLE MODE SELECTION 
	////    WIFIPutSSerial("AT+CWJAP=\"E-RACHIT\",\"ER38151817i20\"\r\n"); // MULTIPLE MODE SELECTION 
	////	 WIFIPutSSerial("AT+CWJAP=\"ASHOK\",\"ashok273996\"\r\n");    
	//WIFI_CONNECT_Response(buff);	  
	//	lcd_command_write(0x80);
	//putSLcd(buff);
	//	lcd_command_write(0xc0);
	//putSLcd("Connecting4...");
	//
	//WIFI_CONNECT_Response(buff);
	//	lcd_command_write(0x80);
	//putSLcd(buff); 
	//	delay(10000);
	
	 	WIFIPutSSerial("AT+CIPMUX=1\r\n"); // MULTIPLE MODE SELECTION    
	    recvResponse(buff);
		lcd_command_write(0x80);
	//	putSLcd(buff);
		lcd_command_write(0xc0);
		putSLcd("Connecting.....");
		delay(5000); 
		delay(5000); 
	
	//	WIFIPutSSerial("AT+CIPSTART=\"TCP\",\"192.168.4.2\",\"80\"\r");
		
		WIFIPutSSerial("AT+CIPSERVER=1,80\r\n"); // MULTIPLE MODE SELECTION    
	    recvResponse(buff);
		delay(8000);
		lcd_command_write(0x80);
	//	putSLcd(buff);
		
		lcd_command_write(0xc0);
		putSLcd("Connecting......");
	
		WIFIPutSSerial("AT+CIFSR\r\n"); // MULTIPLE MODE SELECTION    
		WIFI_RECEIVE_IP(buff);
		lcd_command_write(1);
		lcd_command_write(0x80);
		putSLcd(buff);
		delay(10000); 
	//	lcd_command_write(0x80);
//		while(1)
//		{
//			delay(500);
//			GAS_CHECK();
//			SOLAR_VOLTAGE();
//			BATTERY_VOLTAGE();
//			current_sensor();
//	
//		}	
}

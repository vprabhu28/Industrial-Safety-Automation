#include "serial.h"
#include "gsm.h"
#include "lcd.h"

#define GsmPutSSerial(string) uart0_puts( string )
#define GsmGetCSerial( )  uart0_getkey()
#define GsmPutCSerial(chr) uart0_putc(chr)


#define  getCSerial() uart0_getkey() 

void recvResponse_1(unsigned char *response)
{
		unsigned char ch;
		while(GsmGetCSerial() != '\n' );
		while( (ch = GsmGetCSerial()) != '\r' ) *response++ = ch;
		*response = '\0' ; 
		GsmGetCSerial();
}
void intGsm( void )
{
     unsigned char buff[10];  
    
	    lcd_putstring(0,0,"GSM INT..................");
		GsmPutCSerial('A'); 
	    delay(100);
	    GsmPutCSerial('T'); 
	    delay(100);
	    GsmPutCSerial('E');
	    delay(100);
	    GsmPutCSerial('0');
	    delay(100);
	    GsmPutCSerial('\r'); 
	    recvResponse_1(buff);
	    GsmPutSSerial("AT+CMGF=1\r"); 
	    recvResponse_1(buff);   
	    
	    GsmPutSSerial("AT+CNMI=0,0,0,0\r"); 
	    recvResponse_1(buff);
	    
	    GsmPutSSerial("AT+CSAS\r"); //Save the SMS Settings 
	    recvResponse_1(buff); 
}
unsigned char sendSMS( unsigned char *num, unsigned char * str )
{
  
	unsigned char ch;
	unsigned char res[10];
	unsigned char *response = res;
		GsmPutSSerial("AT+CMGS=\"");
		GsmPutSSerial(num);
		GsmPutSSerial("\"\r");
	
	while( (ch = GsmGetCSerial()) != '>' && ch !='+' ) ;
	if( ch == '+' )
    {
       
		*response++ = ch;
		while( (ch = GsmGetCSerial()) != '\r' ) *response++ = ch;
		*response = '\0' ; 
		GsmGetCSerial();
        return 0;
    }
		GsmGetCSerial(); 
		GsmPutSSerial( str );		
		GsmPutCSerial(26);	   //Send Control Z.    
		recvResponse_1( response );
		if( response[3] != 'S' )
    {              
		recvResponse_1( response );
		return 1;
    }
		return 0 ;      
}
unsigned char getSMS(sms_data *sms_data_ptr)
{
	unsigned char ch, i;
		
		GsmPutSSerial("AT+CMGL=\"ALL\"\r");
		
	while( (ch = GsmGetCSerial()) != 'K' && ch != '+' ) ;
	if( ch == 'K' )
    {
	while( GsmGetCSerial() != '\r' );
	while( GsmGetCSerial() != '\n' );  
	return 0; 
    }
    
    while( (ch = GsmGetCSerial()) != 'S' && ch != 'G' ) ;
    
    if( ch == 'S' )
    {
        while( GsmGetCSerial() != '\r' );
        while( GsmGetCSerial() != '\n' );  
        return 0; 
    }
    
    while( GsmGetCSerial() != ' ' );
    
    i = 0 ;
    while( (ch = GsmGetCSerial()) != ',' ) sms_data_ptr->index[i++] = ch;
    sms_data_ptr->index[i++] = '\0';
   
    
    while( GsmGetCSerial() != ',' );
    while( GsmGetCSerial() != '\"' );
    
    
    i = 0 ;
    while( (ch = GsmGetCSerial()) != '\"' ) sms_data_ptr->phone_num[i++] = ch;
    sms_data_ptr->phone_num[i++] = '\0';
    
    
    while( GsmGetCSerial() != '\n' );
    
    
    i = 0 ;
    while( (ch = GsmGetCSerial()) != '\r' ) sms_data_ptr->dat[i++] = ch;
    sms_data_ptr->dat[i++] = '\0';
  
    
     while( GsmGetCSerial() != '\n' );
    
    while(1)
    {
        if( GsmGetCSerial() == '+' )
        {
            while( GsmGetCSerial() != '\n' );
            while( GsmGetCSerial() != '\r' );
            while( GsmGetCSerial() != '\n' ); 
        }
        else
        {
            while( GsmGetCSerial() != '\n' );
            while( GsmGetCSerial() != '\n' ); 
            return 1;  
        }
    }  
         
}
unsigned char deleteSms(unsigned char *index)
{

		unsigned char response[10];
		
		GsmPutSSerial("AT+CMGD="); 
		
		GsmPutSSerial(index);
		
		GsmPutSSerial("\r");

    	recvResponse_1( response );  
    
    return 1;  
}

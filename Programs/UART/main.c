#include "LPC214x.H"                        
#include "all.h"

extern DWORD UART0Count;
extern BYTE UART0Buffer[BUFSIZE];
extern DWORD UART1Count;
extern BYTE UART1Buffer[BUFSIZE];
#define UART0_HOST_BAUD  9600
#define UART1_HOST_BAUD  9600

sms_data sms_dat;

#define GAS1  (1 << 12)
#define FIRE1 (1 << 13)
#define BUZZER (1 << 22)


#define GAS  	(1 << 16)
#define FIRE    (1 << 17)
#define TEMP    (1 << 18)
#define VOLT    (1 << 21)


unsigned char buf1[10],buf2[10],buf[10], IOT_BUFF[80];

void GAS_CHECK(void);
void TEMP_MONITOR(void);
void process_adc2(void);
void FIRE_CHECK(void);
void VOLTAGE_CHECK(void);
unsigned int i, cnt;

int main (void)
{
	IO0DIR &= ~(GAS1|FIRE1);
	IO1DIR |= (GAS|FIRE|TEMP|VOLT|BUZZER);
	IO1CLR |= (GAS|FIRE|TEMP|VOLT|BUZZER);
	init_lcd();
	uart0_init();
	uart1_init();
	init_VIC();
	init_adc0();
	clear();
	lcd_command_write(0x01);
	lcd_putstring(0,3,"WELCOME TO ");
	lcd_putstring(1,0,"  JSS INDUSTRY");
	intGsm();
	lcd_command_write(0x01);
	lcd_putstring(0,0,"GSM INT COMPLETE...");
	delay(5000);
	sendSMS("9008073248", "Welcome to JSS INDUSTRY");
	delay(5000);
	UARTInit(UART1,UART_BAUD(UART1_HOST_BAUD));

	IO1SET |=BUZZER;
	while(1)
	{
		GAS_CHECK();
		FIRE_CHECK();
		VOLTAGE_CHECK();
		TEMP_MONITOR();
	  			
	}
}

void GAS_CHECK(void)
{
	if(!(IO0PIN&GAS1))
	{
		lcd_putstring(1,0,"  GAS DETECTED   ");
		sendSMS("9008073248", "GAS DETECTED");
		delay(1000);
		IO1SET |=GAS;
		IO1SET |=BUZZER;
		delay(1000);
		strcpy(IOT_BUFF,"UgasDETECTED*"); 
		delay(1000);	
	}
	else
	{
		IO1CLR |=GAS;
		IO1CLR |=BUZZER;
		strcpy(IOT_BUFF,"UgasNORMAL*");
		delay(1000);
	}	
}			

void FIRE_CHECK(void)
{
	if(IO0PIN&FIRE1)
	{
		lcd_putstring(1,0," FIRE DETECTED      ");
		delay(500);
		sendSMS("9008073248", "FIRE DETECTED");	
		delay(1000);
		strcat(IOT_BUFF,"fireFIRE DETECTED*");
		IO1SET |=FIRE;
		IO1SET |=BUZZER;
		delay(1000);	
	}
	else
	{
		strcat(IOT_BUFF,"fireNO FIRE*");
		delay(1000);
		IO1CLR |=FIRE;
		IO1CLR |=BUZZER;
	}	
}

void VOLTAGE_CHECK(void)
{ 
	short adc_value1 = 0; 
	float volts1 = 0.0 ,temp1 = 0.0;
    adc_value1 = adc_read(ADC0,CHANNEL_2);
	volts1 =((adc_value1*3.3))/1023;  
	temp1 = volts1*5.0;   
	sprintf(buf1, "Volt:%.2fV", temp1);
	clear();
	lcd_putstring(0,0, buf1);
	strcat(IOT_BUFF,"voltage");
	strcat(IOT_BUFF,buf1);
	strcat(IOT_BUFF,"*"); 
	delay(1000);
	delay(1000);
	
	if(temp1 > 12)
	{	 					   
		lcd_putstring(1,0,"  OVER VOLTAGE  ");
		sendSMS("9008073248", "OVER VOLTAGE ");
		delay(1000);
		IO1SET |=VOLT;
		IO1SET |=BUZZER;
		delay(1000);
   }
	else
	{
		IO1CLR |=VOLT;
		IO1CLR |=BUZZER;
	}
}

void TEMP_MONITOR(void)
{	  
	short adc_value = 0; 
	float volts = 0.0 ,temp = 0.0;
    adc_value = adc_read(ADC0,CHANNEL_1);
	volts =((adc_value*3.3))/1023;  
	temp = volts * 100;   
	sprintf(buf2, "Temp: %.2fC", temp);
	lcd_putstring(1,0, buf2);
	strcat(IOT_BUFF,"temp");
	strcat(IOT_BUFF,buf2);
	strcat(IOT_BUFF,"*$"); 
	delay(1000);
	UARTSend(UART1,IOT_BUFF,80);
	delay(10);
	
	if(temp > 35)
	{	 					   
		lcd_putstring(1,0,"  TEMP IS MORE  ");
		sendSMS("9008073248", "TEMP IS MORE ");
		delay(1000);

		IO1SET |=TEMP;
		delay(1000);
		IO1SET |=BUZZER;	
		delay(1000);
   }
	else
	{
		IO1CLR |=TEMP;
		IO1CLR |=BUZZER;
	}
} 





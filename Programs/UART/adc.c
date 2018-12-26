#include "adc.h"
#include <LPC214x.H>                       
void init_adc0(void)
{
	PINSEL1 = (PINSEL1 & ~(3 << 28)) | (1 << 28);  //AD0.3
	PINSEL1 = (PINSEL1 & ~(3 << 26)) | (1 << 26);  //AD0.2.
	PINSEL1 = (PINSEL1 & ~(3 << 24)) | (1 << 24);  //AD0.1
}
void init_adc1(void)
{		
}
unsigned short adc_read(unsigned char adc_num, unsigned char ch)
{
	unsigned int i=0;
	switch(adc_num)
	{
		case ADC0:
		AD0CR = 0x00200D00 | (1<<ch);    
		AD0CR |= 0x01000000;
		do
		{
			i = AD0GDR; 
		} while ((i & 0x80000000) == 0); 
		break;
		case ADC1:
		AD1CR = 0x00200D00 | (1<<ch); 
		AD1CR |= 0x01000000; 
		do
		{
			i = AD1GDR; 
		} while ((i & 0x80000000) == 0); 
		break;
	}
	return (i >> 6) & 0x03FF; 
}










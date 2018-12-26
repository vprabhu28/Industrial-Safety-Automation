#include <LPC21xx.H> 
#include "lcd.h"	   
#define LCD_BACK_LIGHT_TIMEOUT 1000          
#define LCD_DATA_DIR	   IO0DIR
#define LCD_DATA_SET	   IO0SET
#define LCD_DATA_CLR	   IO0CLR

#define LCD_CTRL_DIR	   IO0DIR
#define LCD_CTRL_SET       IO0SET
#define LCD_CTRL_CLR       IO0CLR

#define LCDRS		(1 << 15)
#define LCDRW		(1 << 16)
#define LCDEN		(1 << 17)

#define LCD_D4		(1 << 18)
#define LCD_D5		(1 << 19)
#define LCD_D6		(1 << 20)
#define LCD_D7		(1 << 21)

#define LCD_DATA_MASK		(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)
#define LCD_BUSY_FLAG		LCD_D7

#define LCD_IO0PIN			IO0PIN
#define LCD_IO0PIN			IO0PIN
#define MAX_CHAR_IN_ONE_LINE 16
void clear();
void delay(int count)
{
	int j=0,i=0;
	for(j=0;j<count;j++)
  	{
	for(i=0;i<1257;i++);
	}
}

void wait_lcd( void )
{
	  LCD_DATA_DIR &= ~LCD_BUSY_FLAG;
	  LCD_CTRL_CLR |=  LCDRS;	
	  LCD_CTRL_SET |=  LCDRW |LCDEN;
	  while(LCD_IO0PIN & LCD_BUSY_FLAG);		 
	  
	  LCD_CTRL_CLR |= LCDEN | LCDRW;
	  LCD_DATA_DIR |= LCD_DATA_MASK;
	  
	  delay(10);  
}
   
void lcd_command_write( unsigned char command )
{
		unsigned char temp=0;
		unsigned int temp1=0;
		
		temp=command;
		temp=(temp>>4)&0x0F;
		temp1=(temp<<18)&LCD_DATA_MASK;
		
		LCD_CTRL_CLR = LCDRS;
		LCD_CTRL_SET = LCDEN;
		LCD_DATA_CLR = LCD_DATA_MASK;
		LCD_DATA_SET = temp1;
		delay(100);
		LCD_CTRL_CLR = LCDEN;
		
		temp=command;
		temp&=0x0F;
		temp1=(temp<<18)&LCD_DATA_MASK;
		delay(100*2);
		
		LCD_CTRL_CLR |= LCDRS;
		LCD_CTRL_SET |= LCDEN;
		LCD_DATA_CLR = LCD_DATA_MASK;
		LCD_DATA_SET = temp1;
		delay(100);	
		LCD_CTRL_CLR |= LCDEN;
		wait_lcd();
}

void set_lcd_port_output( void )
{
		LCD_CTRL_DIR |= ( LCDEN | LCDRS | LCDRW );
		LCD_CTRL_CLR |= ( LCDEN | LCDRS | LCDRW );	
		LCD_DATA_DIR |= LCD_DATA_MASK;
}

int lcd_gotoxy( unsigned int x, unsigned int y)
{
		int retval = 0;
		
		if( (x > 1) && (y > 15) )
		{
		retval = -1;
		} 
		else 
		{
		if( x == 0 ) 
		{
		lcd_command_write( 0x80 + y );		 
		} 
		else if( x==1 )
		{
		lcd_command_write( 0xC0 + y );		 
		}
		}
		return retval;
} 

void lcd_data_write( unsigned char data )
{
		unsigned char temp=0;
		unsigned int temp1=0;
		
		temp=data;
		temp=(temp>>4)&0x0F;
		temp1=(temp<<18)&LCD_DATA_MASK;
		
		LCD_CTRL_SET |= LCDEN|LCDRS;
		LCD_DATA_CLR = LCD_DATA_MASK;
		LCD_DATA_SET = temp1;
		LCD_CTRL_CLR |= LCDEN;
		
		temp=data;
		temp&=0x0F;
		temp1=(temp<<18)&LCD_DATA_MASK;
		
		LCD_CTRL_SET |= LCDEN|LCDRS;
		LCD_DATA_CLR = LCD_DATA_MASK;
		LCD_DATA_SET = temp1;
		LCD_CTRL_CLR |= LCDEN; 
		wait_lcd();
}

void lcd_putchar( int c )
{
		lcd_data_write( c );
}

void lcd_putstring( unsigned char line,unsigned char pos, char *string )
{
		unsigned char len = MAX_CHAR_IN_ONE_LINE;
		lcd_gotoxy( line,pos);
		while(*string != '\0' && len--)
		{
		lcd_putchar( *string );
		string++;
		}
} 

void init_lcd( void )
{
		set_lcd_port_output();
		delay(100*2);
		lcd_command_write(0x28);    
		lcd_command_write(0x02);      
		lcd_command_write(0x06);     
		lcd_command_write(0x0F);
		lcd_command_write(0x0C);        
}

void test_cmd(void)
{
		lcd_command_write(0x38);    
		lcd_command_write(0x08);    
		lcd_command_write(0x0C);    
		lcd_command_write(0x10);    
		lcd_command_write(0x14);    
		lcd_command_write(0x01);  
		 
}

void clear()
{
lcd_command_write(0x01); 

}
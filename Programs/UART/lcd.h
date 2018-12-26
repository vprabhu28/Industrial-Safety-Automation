extern	void lcd_command_write( unsigned char command);
extern	void lcd_data_write( unsigned char data );
extern	void init_lcd();
extern	void lcd_putstring( unsigned char line,unsigned char pos, char *string );
extern	void delay(int count);  
extern  void test_cmd(void);  
extern  void lcd_putchar( int c );
extern  lcd_gotoxy( unsigned int x, unsigned int y);  
/*
===============================================================================
 Name        : lcd.h
 Author      : Sarah Brown
 Description : Header for lcd functions
===============================================================================
*/

void lcd_write(int command, int isData);
void lcd_init();
void display_string(char *str);
void display_char(char c);
void display_digits(int time_unit);


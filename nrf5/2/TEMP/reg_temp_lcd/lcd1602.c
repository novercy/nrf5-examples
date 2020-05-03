/*
 * LCD 1602 for nrf5-lib
 */

#include "lcd1602.h"

void delay(int n)
{
	while(n--)
		__asm__("nop");
}


void lcd_enable(void)
{
	gpio_set(E);
	delay(100);
	gpio_clear(E);
	delay(500);
}

void lcd_set_data(uint8_t data)
{
	data = (data & 0x0F);
	if (data & 0x01){gpio_set(D4);}
	else {gpio_clear(D4);}
	if ((data>>1) & 0x01){gpio_set(D5);}
	else {gpio_clear(D5);}
	if ((data>>2) & 0x01){gpio_set(D6);}
	else {gpio_clear(D6);}
	if ((data>>3) & 0x01){gpio_set(D7);}
	else {gpio_clear(D7);}
}

void lcd_command (uint8_t cm)
{
	gpio_clear(RS);
	lcd_set_data(cm>>4);
	lcd_enable();
	lcd_set_data(cm & 0x0F);
	lcd_enable();
	delay(100000);
}

void lcd_data_write (uint8_t data)
{
	gpio_set(RS);
	lcd_set_data(data>>4);
	lcd_enable();
	lcd_set_data(data & 0x0F);
	lcd_enable();
	delay(100000);
}

void lcd_init(void)
{
	gpio_clear(E|RS|D4|D5|D6|D7);
	
	delay(150000);
	lcd_set_data(0x03);
	delay(50000);
	lcd_set_data(0x03);
	delay(1600);
	lcd_set_data(0x03);
	delay(1600);
	lcd_set_data(0x02);

	lcd_command(0x28);
	lcd_command(0x0C);
	lcd_command(0x01);
	lcd_command(0x06);
}

void lcd_build_char(uint8_t loc, uint8_t *p) //for hex format
{
	unsigned char i;

	if(loc<8){
		lcd_command(0x40+(loc*8));
		for(i=0;i<8;i++)
			lcd_data_write(p[i]);
	}
	lcd_command(0x80);
}

void lcd_set_row_column(unsigned char row, unsigned char column)
{
	if(row!=0) lcd_command(0xC0+column);
	else lcd_command(0x80+column);
	delay(10000);
}

void lcd_send_text(unsigned char *data)
{
	while (*data)
	{
		lcd_data_write(*data);
		data++;
	}
}

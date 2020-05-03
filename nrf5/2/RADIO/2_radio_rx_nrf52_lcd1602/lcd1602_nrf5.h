/*
 * LCD 1602 for nrf5-lib
 */


#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <libopencm3/nrf5/gpio.h>

/*Set pin*/
#define E GPIO26
#define RS GPIO25
#define D4 GPIO27
#define D5 GPIO28
#define D6 GPIO2
#define D7 GPIO3

void delay(int n);
void lcd_enable(void);
void lcd_set_data(uint8_t data);
void lcd_command (uint8_t cm);
void lcd_data_write (uint8_t data);
void lcd_init(void);
void lcd_build_char(uint8_t loc, uint8_t *p);
void lcd_set_row_column(unsigned char row, unsigned char column);
void lcd_send_text(unsigned char *data);

/*
 * RADIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

/******************************
transmitter: nrf51
receiver: nrf52
******************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/clock.h>
#include <libopencm3/nrf5/radio.h>
#include <libopencm3/nrf5/uarte.h>
#include "lcd1602_nrf5.h"

static void clock_setup(void){
	clock_hfclk_start();
}

static void gpio_setup(void){
	gpio_config(pin25, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin26, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin27, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin28, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin2, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin3, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void led_indicating(void){
	gpio_config(pin20, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_clear(GPIO20);
	delay(1000000);
	gpio_set(GPIO20);
	delay(1000000);
}

static void radio_rx(void){
	/*
	length: S0 = 0 byte, data length = 8 bits, S1 = 0 bit and S1 automatic included in RAM
	max.payload = 255 bytes with 0 byte Add-on (standard BLE packet) and LSB first
	*/
	radio_packet_config(0,8,0,S1INCL_AUTOMATIC,255,0,ENDIAN_LITTLE);
	/*preamble = 8 bits*/
	radio_preamble(PLEN_8BIT);
	/*
	base address length = 4 bytes and logical address 2 (base1, prefix0.ap1)
	base address: 0x5A6B7C8D, prefix address: 0xAC (prefix address size always 1 byte)
	*/
	radio_address_config(4,2,0x5A6B7C8D,0xAC);
	/*set rxaddress, which must be the same with logical address has set above*/
	radio_rxaddress(2);
	/*Radio Mode: Bluetooth Low Energy 1Mbit/s*/
	radio_mode(BLE_1MBIT);
	/*output power 0dbm*/
	radio_output_power(TXPOWER_0DBM);
	/*frequency +75 MHz with Map DEFAULT, so that frequenz = 2475 MHz*/
	radio_frequency(75, MAP_DEFAULT);
	/*
	set crc length 1 byte (x^0 bis x^7) und address excluded in calculation
	polynom: x^7+x^5+x^3+x = 10101010 = 0xAA, init = 0x11
	*/
	radio_crc(1, SKIPADDR_SKIP, 0xAA, 0x11);
}

int main(void){
	clock_setup();
	gpio_setup();
	radio_rx();
	lcd_init();
	lcd_command(0x01); //clear display screen

	char char1[] = "RADIO text:";
	lcd_set_row_column(0,0);
	lcd_send_text(char1);

	char char2[] = "     ";

	/*setup data to receive with 16 bytes likes radio tx*/
	volatile char data[7];
	data[0] = 6; //payload has 6 bytes
	radio_packet_ptr((uint32_t*)&data[0]);
	/*enable radio in rx mode*/
	radio_rx_enable();
	/*start radio*/
	radio_start();

	while(1){
		/*
		When data has been received and crc checked ok, LED will blink and send data 
		content to LCD.
		*/
		if(radio_crc_ok()){
			lcd_set_row_column(1,0);
			lcd_send_text(&data[1]);
			led_indicating();
			radio_crc_ok_reset();
			radio_start();
		}
		else{
			gpio_set(GPIO20);
			lcd_set_row_column(1,0);
			lcd_send_text(char2);
		}
	}

	return 0;
}


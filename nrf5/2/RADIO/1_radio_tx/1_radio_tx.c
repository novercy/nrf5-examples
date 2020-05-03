/*
 * RADIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/clock.h>
#include <libopencm3/nrf5/radio.h>

static void clock_setup(void){
	clock_hfclk_start();
}

static void gpio_setup(void){
	gpio_config(pin13, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin17, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void radio_tx(void){
	radio_packet_config(0,8,0,S1INCL_AUTOMATIC,8,0,ENDIAN_LITTLE);
	radio_preamble(PLEN_8BIT);
	radio_address_config(3,0,0x2B3C4D,0x1A);
	radio_txaddress(0);
	radio_mode(BLE_1MBIT);
	radio_output_power(TXPOWER_0DBM);
	radio_frequency(50, MAP_DEFAULT);
}

int main(void){
	clock_setup();
	gpio_setup();
	radio_tx();

	char data[3];
	data[0] = 1;
	data[1] = 'A';
	radio_tx_enable();

	while(1){
		int button;
	    button = (GPIO_IN >> pin13)&1;

		if(button == 0){
			gpio_clear(GPIO17);
			radio_send_waiting((uint32_t*)&data[0]);
		}
		if(button == 1){
			gpio_set(GPIO17);
		}
	}

	return 0;
}


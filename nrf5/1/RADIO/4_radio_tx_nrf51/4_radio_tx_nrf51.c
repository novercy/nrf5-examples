/*
 * RADIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

/******************************
tx: nrf51 (4_radio_tx_nrf51)
rx: nrf52 (4_radio_rx_nrf52)
******************************/

#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/clock.h>
#include <libopencm3/nrf5/radio.h>

static void clock_setup(void){
	clock_hfclk_start();
}

static void gpio_setup(void){
	gpio_config(pin17, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin21, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void radio_tx(void){
	// no option S1INCL for nrf51
	radio_packet_config(0,8,0,S1INCL_AUTOMATIC,8,0,ENDIAN_LITTLE);
	// preamble is always 8 bit
	radio_address_config(3,0,0x2B3C4D,0x1A);
	radio_txaddress(0);
	// no option Ble_2Mbit for nrf51
	radio_mode(BLE_1MBIT);
	radio_output_power(TXPOWER_0DBM);
	// only option 2400+frequency (map_default)
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
	    button = (GPIO_IN >> pin17)&1;

		if(button == 0){
			gpio_clear(GPIO21);
			radio_send_waiting((uint32_t*)&data[0]);
		}
		if(button == 1){
			gpio_set(GPIO21);
		}
	}

	return 0;
}


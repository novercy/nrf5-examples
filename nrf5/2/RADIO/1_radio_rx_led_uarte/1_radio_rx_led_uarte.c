/*
 * RADIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/clock.h>
#include <libopencm3/nrf5/radio.h>
#include <libopencm3/nrf5/uarte.h>

static void clock_setup(void){
	clock_hfclk_start();
}

static void gpio_setup(void){
	gpio_config(pin11, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_set(GPIO11);
}

static void led_indicating(void){
	gpio_config(pin20, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_clear(GPIO20);
	for (int i = 0; i < 1000000; i++){
		__asm__("nop");
	}
	gpio_set(GPIO20);
	for (int i = 0; i < 1000000; i++){
  		__asm__("nop");
	}
}

static void uarte_setup(void){
	uarte_set_baudrate(UARTE0, BAUD9600);
	uarte_config(UARTE0, HWFC_DISABLED, PARITY_EXCLUDED);
	uarte_tx_pin(UARTE0, pin11);
	uarte_enable(UARTE0);
}

static void radio_rx(void){
	radio_packet_config(0,8,0,S1INCL_AUTOMATIC,8,0,ENDIAN_LITTLE);
	radio_preamble(PLEN_8BIT);
	radio_address_config(3,0,0x2B3C4D,0x1A);
	radio_rxaddress(0);
	radio_mode(BLE_1MBIT);
	radio_frequency(50, MAP_DEFAULT);
}

int main(void){
	clock_setup();
	gpio_setup();
	uarte_setup();
	radio_rx();

	char data[3];
	data[0] = 1;
	radio_packet_ptr((uint32_t*)&data[0]);
	radio_rx_enable();
	radio_start();

	while(1){
		if(radio_packet_completed()){
			uarte_send_waiting(UARTE0, (uint8_t*)&data[1], 1);
			led_indicating();
			radio_packet_completed_reset();
			radio_start();
		}
		else{
			gpio_set(GPIO20);
		}
	}

	return 0;
}


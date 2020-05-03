/*
 * RADIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

/******************************
transmitter: nrf51
receiver: nrf52
******************************/

#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/clock.h>
#include <libopencm3/nrf5/radio.h>

static void clock_setup(void){
	clock_hfclk_start();
}

static void gpio_setup(void){
	//button 1
	gpio_config(pin17, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
	//led indicating
	gpio_config(pin21, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void radio_tx(void){
	/*
	length: S0 = 0 byte, data length = 8 bits, S1 = 0 bit, (S1INCl not for nrf51)
	max.payload = 255 bytes with 0 byte Add-on (standard BLE packet) and LSB first
	*/
	radio_packet_config(0,8,0,S1INCL_AUTOMATIC,255,0,ENDIAN_LITTLE);
	/*preamble is always 8 bits length (nrf51)*/
	/*
	base address length = 4 bytes and logical address 2 (base1, prefix0.ap1)
	base address: 0x5A6B7C8D, prefix address: 0xAC (prefix address size always 1 byte)
	*/
	radio_address_config(4,2,0x5A6B7C8D,0xAC);
	/*set txaddress, which must be the same with logical address has set above*/
	radio_txaddress(2);
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
	radio_tx();

	/*setup data with 16 bytes*/
	char data[7]={6,'H','A','L','L','O',' '}; //Payload has 6 Bytes
	/*enable radio in tx mode*/
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


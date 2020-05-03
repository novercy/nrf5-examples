/*
 * TIMER example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/timer.h>

static void gpio_setup(void){
	gpio_config(pin17, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void timer_setup(void){
	timer_mode(TIMER1, MODE_TIMER);
	timer_prescaler(TIMER1, 7);
	timer_bitmode(TIMER1, BITMODE_16);
	//enable interrupt timer
	nvic_enable_irq(NVIC_TIMER1_IRQ);
	//enable interrupt for CC1
	timer_interrupt_enable(TIMER1, CC1);
}

int main(void){
	gpio_setup();
	timer_setup();
	timer_capture_enable(TIMER1, CC1);
	timer_cc_set(TIMER1, CC1, 12345);
	timer_start(TIMER1);
	while(1);
	return 0;
}

void timer1_isr(void){
	if(timer_compare_matched(TIMER1, CC1)){
		gpio_toggle(GPIO17);
		timer_compare_reset(TIMER1, CC1);
	}
}

/*
 * TIMER example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/timer.h>

static void gpio_setup(void){
	gpio_config(pin13, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin14, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin17, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin18, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void timer_setup(void){
	timer_mode(TIMER1, MODE_TIMER);
	timer_mode(TIMER2, MODE_TIMER);
	timer_prescaler(TIMER1, 4);
	timer_prescaler(TIMER2, 7);
	timer_bitmode(TIMER1, BITMODE_16);
	timer_bitmode(TIMER2, BITMODE_16);
	//enable interrupt timer
	nvic_enable_irq(NVIC_TIMER1_IRQ);
	nvic_enable_irq(NVIC_TIMER2_IRQ);
	//enable interrupt for CC1
	timer_interrupt_enable(TIMER1, CC1);
	timer_interrupt_enable(TIMER2, CC2);
}

int main(void){
	gpio_setup();
	timer_setup();
	timer_capture_enable(TIMER1, CC1);
	timer_capture_enable(TIMER2, CC2);
	timer_cc_set(TIMER1, CC1, 12345);
	timer_cc_set(TIMER2, CC2, 65000);
	timer_start(TIMER1);
	timer_start(TIMER2);

	while(1);
	return 0;
}

void timer1_isr(void){
	if(GPIO_IN &= GPIO13){
		gpio_set(GPIO17);
  	}
  	else{
  		if(timer_compare_matched(TIMER1, CC1)){
			gpio_toggle(GPIO17);
			timer_compare_reset(TIMER1, CC1);
		}
	}
}

void timer2_isr(void){
  	if(GPIO_IN &= GPIO14){
		gpio_set(GPIO18);
  	}
  	else{
  		if(timer_compare_matched(TIMER2, CC2)){
			gpio_toggle(GPIO18);
			timer_compare_reset(TIMER2, CC2);
		}
	}
}

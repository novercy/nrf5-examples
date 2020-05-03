/*
 * PWM example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/pwm.h>
#include <libopencm3/nrf5/gpio.h>

	uint16_t ram[] = {1000};//10% Brightness (PWM0), led off in 8us*1000=8ms, on in 72ms
//	uint16_t ram[] = {5000};//50%, off 40ms, on 40ms
//	uint16_t ram[] = {100}; //1%, off 0,8ms, on 79,2ms
//	uint16_t ram[] = {9000};//90%, off 72ms, on 0,8ms

static void pwm_setup(void){
	//set prescaler for 1 Pulse = 16MHz/128 => t(Pulse) = 8us
	pwm_prescaler(PWM0, DIV_128);
	pwm_prescaler(PWM1, DIV_128);
	//set countertop 10000 for PWM0 => 1 cycle = 80ms (max 32768)
	pwm_counter_top(PWM0, 10000);
	pwm_counter_top(PWM1, 32000);
	//set counter mode
	pwm_counter_mode(PWM0, MODE_UP);
	pwm_counter_mode(PWM1, MODE_UPANDDOWN);//mode UPANDDOWN for double time
	//set Decoder: Load individual, then 4 Ram Location will be used, and refresh count
	pwm_decoder(PWM0, LOAD_COMMON, MODE_REFRESHCOUNT);
	pwm_decoder(PWM1, LOAD_COMMON, MODE_REFRESHCOUNT);
	//pointer und size of Result buffer, without refresh and delay
	pwm_sequence_config(PWM0, SEQ0, &ram[0], sizeof(ram)/sizeof(uint16_t), REFRESH_CONTINUOUS, 0);
	pwm_sequence_config(PWM1, SEQ0, &ram[0], sizeof(ram)/sizeof(uint16_t), REFRESH_CONTINUOUS, 0);
	//set output pin17 and pin18
	pwm_output_pin(PWM0, OUTPUT0, pin17, CONNECT_CONNECTED);
	pwm_output_pin(PWM1, OUTPUT0, pin18, CONNECT_CONNECTED);
	//enable pwm
	pwm_enable(PWM0);
	pwm_enable(PWM1);
	//start sequence
	pwm_start_seq(PWM0, SEQ0);
	pwm_start_seq(PWM1, SEQ0);
}

int main(void){
  	pwm_setup();

	return 0;
}
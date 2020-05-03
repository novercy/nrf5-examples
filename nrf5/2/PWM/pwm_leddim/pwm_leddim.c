/*
 * PWM example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/pwm.h>
#include <libopencm3/nrf5/gpio.h>

	uint16_t Datenram[] = { 
	100,200,300,400,500,600,700,800,900,
	1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,
	2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,
	3000,3100,3200,3300,3400,3500,3600,3700,3800,3900,
	4000,4100,4200,4300,4400,4500,4600,4700,4800,4900,
	5000,5100,5200,5300,5400,5500,5600,5700,5800,5900,
	6000,6100,6200,6300,6400,6500,6600,6700,6800,6900,
	7000,7100,7200,7300,7400,7500,7600,7700,7800,7900,
	8000,8100,8200,8300,8400,8500,8600,8700,8800,8900,
	9000,9100,9200,9300,9400,9500,9600,9700,9800,9900,
	};

static void pwm_setup(void){
	pwm_prescaler(PWM0, DIV_16);
	pwm_counter_top(PWM0, 10000);
	pwm_counter_mode(PWM0, MODE_UPANDDOWN);
	pwm_decoder(PWM0, LOAD_COMMON, MODE_REFRESHCOUNT);
	pwm_sequence_config(PWM0, SEQ0, &Datenram[0], sizeof(Datenram)/sizeof(uint16_t), REFRESH_CONTINUOUS, 0);
	pwm_output_pin(PWM0, OUTPUT0, pin17, CONNECT_CONNECTED);
	pwm_enable(PWM0);
	pwm_start_seq(PWM0, SEQ0);
}

int main(void){
  	pwm_setup();

	while(1){
		if(pwm_sequence_end(PWM0, SEQ0)){
			pwm_sequence_end_reset(PWM0, SEQ0);
			pwm_start_seq(PWM0, SEQ0);
		}
	}
	
	return 0;
}
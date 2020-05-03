/*
 * PWM example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/pwm.h>
#include <libopencm3/nrf5/gpio.h>

  uint16_t ram1[] = {400};
  uint16_t ram2[] = {1000};
  uint16_t ram3[] = {5000};
  uint16_t ram4[] = {15000};

static void gpio_setup(void){
  /*configure buttons*/
  //Set INPUT_CONNECT for reacting buttons (input) with LED, otherwise auto blink
  gpio_config(pin13, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin14, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin15, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin16, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
}

static void pwm_setup(void){
	pwm_prescaler(PWM0, DIV_128);//Prescaler for 1 Pulse = 16MHz/128 => t(Pulse) = 8us
	pwm_counter_top(PWM0, 32000);//Countertop = 32000 => 1 cycle = 256ms
	pwm_counter_mode(PWM0, MODE_UP);
	//set Decoder: Load individual, so that 4 Ram Location used, with refreshcount
	pwm_decoder(PWM0, LOAD_COMMON, MODE_REFRESHCOUNT);
	pwm_output_pin(PWM0, OUTPUT0, pin17, CONNECT_CONNECTED);//Output pin17 (LED 1)
	pwm_enable(PWM0);//Enable PWM
}

int main(void){
	gpio_setup();
  	pwm_setup();
  	gpio_set(GPIO17);//initial

  	/**LED 1 blinks with various intensity depends on which button has triggered*/
  	while(1){
  	  	int button1, button2, button3, button4;
  	  	button1 = (GPIO_IN >> pin13)&1;
  	    button2 = (GPIO_IN >> pin14)&1;
  	    button3 = (GPIO_IN >> pin15)&1;
  	    button4 = (GPIO_IN >> pin16)&1;

  	    if(button1 == 0){
  	      /*configurate and start sequence*/
	      pwm_sequence_config(PWM0, SEQ0, &ram1[0], sizeof(ram1)/sizeof(uint16_t), REFRESH_CONTINUOUS, 0);
	      pwm_start_seq(PWM0, SEQ0);
	    }
	    if(button2 == 0){
	      pwm_sequence_config(PWM0, SEQ0, &ram2[0], sizeof(ram2)/sizeof(uint16_t), REFRESH_CONTINUOUS, 0);
	      pwm_start_seq(PWM0, SEQ0);
	    }
	    if(button3 == 0){
	      pwm_sequence_config(PWM0, SEQ0, &ram3[0], sizeof(ram3)/sizeof(uint16_t), REFRESH_CONTINUOUS, 0);
	      pwm_start_seq(PWM0, SEQ0);
	    }
	    if(button4 == 0){
	      pwm_sequence_config(PWM0, SEQ0, &ram4[0], sizeof(ram4)/sizeof(uint16_t), REFRESH_CONTINUOUS, 0);
	      pwm_start_seq(PWM0, SEQ0);
	    }
  	}
	return 0;
}
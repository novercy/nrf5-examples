/*
 * GPIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/gpio.h>

static void gpio_setup(void){
  /*LEDs*/
  gpio_config(pin17, DIR_OUTPUT, INPUT_CONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin18, DIR_OUTPUT, INPUT_CONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  /*Buttons*/
  gpio_config(pin13, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin14, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
  }


int main(void){
  gpio_setup();
  gpio_set(GPIO_ALL); //initial

  while(1){
  	/**LED on when pressing button*/
    int button1;
    button1 = gpio_read(GPIO13)>>pin13;
    if(button1 == 1){
      gpio_set(GPIO17); //LED off
    }
    if(button1 == 0){
      gpio_clear(GPIO17); //LED on
    }
  

    /**LED on/of when triggering button*/
    int button2;
    button2 = gpio_read(GPIO14)>>pin14;
    if(button2 == 1){
      gpio_set(GPIO18);
    }
    if(button2 == 0){
      gpio_toggle(GPIO18);
      for (int i = 0; i < 1000000; i++) {
        __asm__("nop");
      }
    }
  }
  
  return 0;
}

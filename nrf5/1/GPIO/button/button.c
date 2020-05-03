/*
 * GPIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/gpio.h>

static void gpio_setup(void){
  /*LEDs*/
  gpio_config(pin21, DIR_OUTPUT, INPUT_CONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  /*Button*/
  gpio_config(pin17, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
}


int main(void){
  gpio_setup();
  gpio_set(GPIO21);

  while(1){
    int button;
    button = (GPIO_IN >> pin17)&1;
    if(button == 1){
      gpio_set(GPIO21); //LED off
    }
    if(button == 0){
      gpio_clear(GPIO21); //LED on
    }
  }
  
  return 0;
}

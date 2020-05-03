/*
 * GPIO example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/gpio.h>

static void gpio_setup(void){
  gpio_config(pin22, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

int main(void){
  gpio_setup();

  while(1){
    gpio_toggle(GPIO22);
    for (int i = 0; i < 1000000; i++) {
      __asm__("nop");
    }
  }

  return 0;
}

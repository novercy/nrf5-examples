/*
 * GPIOTE example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/gpiote.h>

#define channel 0
#define pin 17

static void gpiote_setup(void){
//mode task for setting output at selected pin
  gpiote_tasks_out(channel);
  gpiote_config(channel, MODE_TASK, pin, POLARITY_LOTOHI, OUTINIT_LOW);
}


int main(void){
  gpiote_setup();

  while(1){
    gpiote_tasks_set(channel);
    for (int i = 0; i < 10000000; i++) {
      __asm__("nop");
    }
    gpiote_tasks_clr(channel);
    for (int i = 0; i < 10000000; i++) {
      __asm__("nop");
    }
  }

  return 0;
}

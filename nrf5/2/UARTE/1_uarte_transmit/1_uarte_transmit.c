/*
 * UARTE example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/uarte.h>
#include <libopencm3/nrf5/gpio.h>

static void gpio_setup(void){
  gpio_config(pin11, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_set(GPIO11);
}

static void uarte_setup(void){
  uarte_set_baudrate(UARTE0, BAUD9600);
  uarte_config(UARTE0, HWFC_DISABLED, PARITY_EXCLUDED);
  uarte_tx_pin(UARTE0, pin11);
  uarte_enable(UARTE0);
}

int main(void){
  gpio_setup();
  uarte_setup();

  uint8_t text[] = "Hallo Welt\n\r";

  while(1){
    uarte_send_waiting(UARTE0, &text[0], sizeof(text));
    for (int i = 0; i < 10000000; i++) {
      __asm__("nop");
    }
  }

  return 0;
}
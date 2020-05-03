/*
 * UARTE example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/uarte.h>
#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/cm3/nvic.h>

static void gpio_setup(void){
  gpio_config(pin12, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin11, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_set(GPIO11);
  gpio_config(pin18, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void uarte_setup(void){
  uarte_set_baudrate(UARTE0, BAUD9600);
  uarte_config(UARTE0, HWFC_DISABLED, PARITY_EXCLUDED);
  uarte_rx_pin(UARTE0, pin12);
  uarte_tx_pin(UARTE0, pin11);
  uarte_enable(UARTE0);
  uarte_interrupt_enable(UARTE0, RXDRDY | ENDRX);
  nvic_enable_irq(NVIC_UARTE0_UART0_IRQ);
}

int main(void){
  static uint8_t text[] = "hallo";
  uarte_setup();
  gpio_setup();
  gpio_set(GPIO18);
  uarte_rx_packet(UARTE0, &text[0], sizeof(text));
  uarte_rx_start(UARTE0);

  while(1);

  return 0;
}


void uarte0_uart0_isr(void){
  if(uarte_received_rxdrdy(UARTE0)){
    gpio_toggle(GPIO18);
    uarte_send_waiting(UARTE0, uarte_rx_received_ptr(UARTE0), 1);
    uarte_rx_init(UARTE0, RXDRDY);
    uarte_rx_init(UARTE0, ENDRX);
    uarte_rx_start(UARTE0);
  }

  return;
}
/*
 * SPI example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <libopencm3/nrf5/spim.h>
#include <libopencm3/nrf5/spis.h>
#include <libopencm3/nrf5/gpio.h>

static uint8_t datatx[] = "test";

static void led_setup(void){
  gpio_config(pin17, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static void spim_setup(void){
  // sck pin28, mosi pin3, miso pin4
  gpio_config(pin28, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  SPIM_PSEL_SCK(SPIM0) = SPIM_PSEL_SCK_PIN(pin28);
  gpio_config(pin3, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  SPIM_PSEL_MOSI(SPIM0) = SPIM_PSEL_MOSI_PIN(pin3);
  gpio_config(pin4, DIR_INPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  SPIM_PSEL_MISO(SPIM0) = SPIM_PSEL_MISO_PIN(pin4);
  // set frequency
  SPIM_FREQUENCY(SPIM0) = SPIM_FREQUENCY_FREQUENCY_K125;
  // set configuration
  SPIM_CONFIG(SPIM0) =  SPIM_CONFIG_ORDER_MsbFirst |
                        SPIM_CONFIG_CPHA_Leading   |
                        SPIM_CONFIG_CPOL_ActiveHigh;
  // data pointer
  SPIM_TXD_PTR(SPIM0) = (uint32_t)datatx;
  SPIM_TXD_MAXCNT(SPIM0) = (uint32_t)sizeof(datatx);
  // enable spim
  SPIM_ENABLE(SPIM0) = SPIM_ENABLE_ENABLE_Enabled;
}

int main(void){
  led_setup();
  spim_setup();

  while(1){
    // start transmit
    SPIM_TASKS_START(SPIM0) = 1;
    while(!SPIM_EVENTS_STARTED(SPIM0));
    
    while(!SPIM_EVENTS_ENDTX(SPIM0));
    if(SPIM_EVENTS_ENDTX(SPIM0)){
      gpio_toggle(GPIO17);
      for (int i = 0; i < 5000000; i++) {
        __asm__("nop");
      }
      SPIM_EVENTS_ENDTX(SPIM0) = 0;
    }
  }

  return 0;
}


/*
 * TEMPERATURE example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <stdint.h>
#include <math.h>
#include <libopencm3/nrf5/temp.h>
#include <libopencm3/nrf5/uarte.h>
#include <libopencm3/nrf5/gpio.h>

static char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void gpio_setup(void){
  //uarte
  gpio_config(pin11, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_set(GPIO11);
  //button
  gpio_config(pin13, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
  //led
  gpio_config(pin17, DIR_OUTPUT, INPUT_CONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
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

  int grad;
  char text[2];

  while(1){
    TEMP_TASKS_START = 1;
    while(TEMP_EVENTS_DATARDY == 0); // wait to measurement finishs
    TEMP_EVENTS_DATARDY = 0;
    //1 step = 0,25
    grad = (uint32_t)TEMP_TEMP * 0.25;

    itoa(grad, text);

    int button = (GPIO_IN >> pin13)&1;

    if(button == 1){
      gpio_set(GPIO17); //led off
    }

    if(button == 0){
      gpio_clear(GPIO17); //led on
      uarte_send_waiting(UARTE0, &text[0], sizeof(text));
      for (int j = 0; j < 5000000; j++){
          __asm__("NOP");
      }
    }
  }
  
  return 0;
}

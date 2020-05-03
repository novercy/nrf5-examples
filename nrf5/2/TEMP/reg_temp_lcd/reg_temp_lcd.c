/*
 * TEMPERATURE example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <libopencm3/nrf5/temp.h>
#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/nrf5/clock.h>
#include "lcd1602.h"

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

void clock_setup(void){
	clock_hfclk_start();
}

void gpio_setup(void){
  gpio_config(pin25, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin26, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin27, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin28, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin2, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  gpio_config(pin3, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

int main(void){
  clock_setup();
  gpio_setup();

  lcd_init();
  lcd_command(0x01);

  int grad;
  char text[2];

  while(1){
    TEMP_TASKS_START = 1;
    while(TEMP_EVENTS_DATARDY == 0); //wait to measurement finishes
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
      lcd_set_row_column(0,0);
      lcd_send_text(text);
      for (int j = 0; j < 5000000; j++){
          __asm__("NOP");
      }
    }
  }
  
  return 0;
}

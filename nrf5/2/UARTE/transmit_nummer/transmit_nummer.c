/*
 * UARTE example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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

int main(void){
  int nummer = 1234;
  char text[4];
  itoa(nummer, text);

  /*config pin for transmitter*/
  gpio_config(pin11, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
  GPIO_OUTSET = GPIO11;

  /*enable UARTE*/
  UART_ENABLE(UARTE0) = UARTE_ENABLE_ENABLE_Enabled;
  /*TX pin*/
  UARTE0_PSEL.TXD = (11);
  /*without Flow Control and Parity*/
  UART_CONFIG(UARTE0) |= UARTE_CONFIG_HWFC_Disabled | UARTE_CONFIG_PARITY_Excluded;
  /*Baudrate*/
  UART_BAUDRATE(UARTE0) = UARTE_BAUDRATE_BAUDRATE_Baud9600;
  
  UARTE0_TXD.PTR = (uint32_t)&text[0];
  UARTE0_TXD.MAXCNT = sizeof(text);

  while(1){
      UARTE0_TASKS_STARTTX = 1;
      UARTE0_EVENTS_TXDRDY = 0; //reset sending event at first
      UARTE0_EVENTS_ENDTX = 0;
      while(UARTE0_EVENTS_ENDTX == 0);
      for (int j = 0; j < 5000000; j++){
          __asm__("NOP");
      }
  }

  return 0;
  
}
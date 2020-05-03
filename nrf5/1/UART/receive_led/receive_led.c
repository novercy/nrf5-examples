/*
 * UART example for nrf5-lib
 * Gerrit Maus <funk@maus.xyz>, Luong Le <novercy@live.com>
 */

#include <stdint.h>
#include <libopencm3/nrf5/uart.h>
#include <libopencm3/nrf5/gpio.h>
#include <libopencm3/cm3/nvic.h>

static void uart_setup(void){
	/*set pin 11 as input (RX), pin 9 as output (TX) and set high*/
	gpio_config(pin11, DIR_INPUT, INPUT_CONNECT, PULL_UP, DRIVE_S0S1, SENSE_DISABLED);
	gpio_config(pin9, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
	GPIO_OUTSET = GPIO9;
	/*enable UART0*/
	UART_ENABLE(UART0) = UART_ENABLE_ENABLE_Enabled;
	/*set RX and TX pins*/
	UART_PSELRXD(UART0) = UART_PSELRXD_PIN(pin11);
	UART_PSELTXD(UART0) = UART_PSELTXD_PIN(pin9);
	/*config UART without FlowControl and Parity*/
  	UART_CONFIG(UART0) = UART_CONFIG_HWFC_Disabled | UART_CONFIG_PARITY_Excluded;
  	/*set Baudrate*/
  	UART_BAUDRATE(UART0) = UART_BAUDRATE_BAUDRATE_Baud9600;
  	/*enable interrupt for RXDRDY Event (Ready to Read)*/
  	UART_INTENSET(UART0) = UART_INTENSET_RXDRDY_Set;
  	/*enable UART0 interrupt*/
  	nvic_enable_irq(NVIC_UART0_IRQ);
}

static void gpio_setup(void){
  gpio_config(pin22, DIR_OUTPUT, INPUT_DISCONNECT, PULL_DISABLED, DRIVE_S0S1, SENSE_DISABLED);
}

static uint32_t uart_receive(void){
	return UART_RXD(UART0);
}


int main(void){
	uart_setup();
	gpio_setup();
	/*start to receive*/
	GPIO_OUT &= 0;
	UART_TASKS_STARTRX(UART0) = 1;

	while(1){
		__asm__("NOP");
	}

	return 0;
}

void uart0_isr(void){
	static uint8_t data1 = 't';
	char text[] = "A";

	if(UART_EVENTS_RXDRDY(UART0) == 1){
		//led1 on when receive data
		gpio_clear(GPIO22);
		for (int j = 0; j < 3000000; j++){
          __asm__("NOP");
        }
        gpio_set(GPIO22);
		UART_EVENTS_RXDRDY(UART0) = 0; //reset RX event and ready to receive next data
		data1 = uart_receive(); //data is assigned

		UART_EVENTS_TXDRDY(UART0) = 0; //init sending event
      	UART_TASKS_STARTTX(UART0) = 1; //start sending
		UART_TXD(UART0) = text[0];
	}

	return;
}

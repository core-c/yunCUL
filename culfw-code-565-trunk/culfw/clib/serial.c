
// This is the Arduino Yun version of serial.c
// It uses Serial1 instead of Serial
// The Yun has a hardware support for Serial1 only,
// so instead of accessing registers: UDR0,UBRR0,UCSR0A,UCSR0B,UCSR0C (and some USART0 bitvalues),
// we access: UDR1,UBRR1,UCSR1A,UCSR1B,UCSR1C registers (and some USART1 bitvalues).
// The Arduino Yun Serial1 port is mapped to /dev/ttyATH0 on the OpenWRT side of the Yun.
// NOTE: We can only access/init the serial connection after u-boot has finished.

#include <avr/interrupt.h>
#include <avr/io.h>
#include "ringbuffer.h"
#include "ttydata.h"
#include "display.h"
    
#include "led.h"
#include "serial.h"

void (*usbinfunc)(void);

// TX complete (data buffer empty) 
ISR(USART_UDRE_vect)
{
     if (TTY_Tx_Buffer.nbytes) {
	  
	  UDR1 = rb_get(&TTY_Tx_Buffer);
	  
     } else {
	  
	  UCSR1B &= ~_BV(UDRIE1);
	  
     }
     
}

// RX complete
ISR(USART_RX_vect)
{

     LED_TOGGLE();

     /* read UART status register and UART data register */ 
     uint8_t data = UDR1;
     uint8_t usr  = UCSR1A;
     
     if ((usr & (_BV(FE1)|_BV(DOR1))) == 0)
	  rb_put(&TTY_Rx_Buffer, data);
     
}

void uart_init(unsigned int baudrate) 
{
     /* Set baud rate */
     if ( baudrate & 0x8000 ) 
     {
	  UCSR1A = (1<<U2X1);  //Enable 2x speed 
	  baudrate &= ~0x8000;
     }

     UBRR1 = baudrate;
     // toch maar ophakken in Low/High delen,
     // want ik zie slechts registers: UBRRH1 & UBRRL1 (geen UBRR1)
     //UBRR1H = (baudrate >> 8) & 0xFF;
     //UBRR1L = baudrate & 0xFF;
     // oops!.. UBRRH1 & UBRRL1 zijn niet gedefinieerd..
     // hmmm.. in datasheet heten ze UBRRH1/UBRRL1, in iom32u4.h heten ze UBRR1H&UBRR1L
     // OK: er is een macro UBRR1 in iom32u4.h (hakt zelf de 16b write uit over 2 registers)
     
     /* Enable USART receiver and transmitter and receive complete interrupt */
     UCSR1B = _BV(RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
    
     /* Set frame format: asynchronous, 8data, no parity, 1stop bit */   
     UCSR1C = (1<<UCSZ10)|(1<<UCSZ11);
     
}

void uart_task(void) 
{
     input_handle_func(DISPLAY_USB);
     uart_flush();
}

void uart_flush(void) 
{
     if (!bit_is_set(UCSR1B, UDRIE1) && TTY_Tx_Buffer.nbytes)
	  UCSR1B |= _BV(UDRIE1);
     
}

#ifndef _BOARD_H
#define _BOARD_H

#include <stdint.h>

/* if you have an Arduino with only 8MHz disable the next line */
#define HAS_16MHZ_CLOCK

/* if you are using a CC1101 module for 868MHz disable the next line */
//#define HAS_CC1100_433


#define SPI_PORT				PORTB
#define SPI_DDR					DDRB
#define SPI_SS					6
#define SPI_MISO				3
#define SPI_MOSI				2
/* die aufgelötete gelbe LED ist an PB5/SCLK angeschlossen! */
#define SPI_SCLK				1

#define CC1100_CS_DDR			SPI_DDR
#define CC1100_CS_PORT			SPI_PORT
#define CC1100_CS_PIN			SPI_SS


/* CC1101 GDO0 Tx / Temperature Sensor */
#if 0
#define CC1100_OUT_DDR			DDRC
#define CC1100_OUT_PORT			PORTC
#define CC1100_OUT_PIN			PC0
#define CC1100_OUT_IN			PINC
#define CCTEMP_MUX				CC1100_OUT_PIN
#else
#define CC1100_OUT_DDR			DDRD
#define CC1100_OUT_PORT			PORTD
#define CC1100_OUT_PIN			PD1
#define CC1100_OUT_IN			PIND
#define CCTEMP_MUX				CC1100_OUT_PIN
#endif

/* CC1101 GDO2 Rx Interrupt */
#define CC1100_IN_DDR			DDRD
#define CC1100_IN_PORT			PIND
#define CC1100_IN_PIN			PD0
#define CC1100_IN_IN			PIND

#define CC1100_INT				INT0
#define CC1100_INTVECT			INT0_vect
#define CC1100_ISC				ISC00
#define CC1100_EICR				EICRA

/* externe LED, on Yun digital pin 8*/
#define LED_DDR					DDRB
#define LED_PORT				PORTB
#define LED_PIN					4

//#define LED_ON_DDR			DDRB
//#define LED_ON_PORT			PORTB
//#define LED_ON_PIN			1


#define BOARD_ID_STR			"yunCUL868"
#define BOARD_ID_STR433			"yunCUL433"

/* define this device as a 433 MHz one */
/* this isn't done like a CUL by reading a port pin but instead a fixed value of 0 for mark433_pin is used */ 
#define MULTI_FREQ_DEVICE
#define MARK433_PIN				mark433_pin
#define MARK433_BIT				0
extern const uint8_t mark433_pin;

#define HAS_UART
#define UART_BAUD_RATE			38400

/* ATMega328P has only one UART, no need to define the UART to use */
// The Arduino Yun has only the hardware USART1. We map the USART_RX_vect anyway..
// (they are referenced in (at least) serial.c)
#define USART_RX_vect			USART1_RX_vect
#define USART_UDRE_vect			USART1_UDRE_vect

#define TTY_BUFSIZE				128


#define RCV_BUCKETS				2      //                 RAM: 25b * bucket
#define FULL_CC1100_PA                // PROGMEM:  108b
#define HAS_RAWSEND                   //
#define HAS_FASTRF                    // PROGMEM:  468b  RAM:  1b
#define HAS_ASKSIN
/* Intertechno Senden einschalten */
#define HAS_INTERTECHNO
#define HAS_TCM97001
/* Intertechno Empfang einschalten */
#define HAS_IT
#define HAS_REVOLT
#define HAS_CC1101_RX_PLL_LOCK_CHECK_TASK_WAIT
#define HAS_CC1101_PLL_LOCK_CHECK_MSG
#define HAS_CC1101_PLL_LOCK_CHECK_MSG_SW
/* HAS_MBUS requires about 1kB RAM, if you want to use it you
   should consider disabling other unneeded features
   to avoid stack overflows
*/
//#define HAS_MBUS

#define HAS_ASKSIN_FUP
#define HAS_MORITZ
#define HAS_RWE
#define HAS_ESA
#define HAS_TX3
#define HAS_UNIROLL
#define HAS_HOERMANN
#define HAS_HOERMANN_SEND
#define HAS_MEMFN
#define HAS_SOMFY_RTS
#define HAS_FHT_80b						// PROGMEM: 1374b, RAM: 90b
#define HAS_FHT_8v						// PROGMEM:  586b  RAM: 23b
#define HAS_FHT_TF
#define FHTBUF_SIZE				174		//                 RAM: 174b
#define HAS_KOPP_FC
#define HAS_ZWAVE						// PROGMEM:  882


// UJE: the relais connected to the yunCUL. on the Yun pin 9 (PB5)
// The FHEM command to change the relais to the OFF state:	set myCUL raw S00
// The FHEM command to change the relais to the ON state:	set myCUL raw S01
#define HAS_YUN_RELAIS

#define YUN_RELAIS_DDR			DDRB
#define YUN_RELAIS_PORT			PORTB
#define YUN_RELAIS_PIN			5

#define yun_relais_init()		YUN_RELAIS_DDR  |= _BV(YUN_RELAIS_PIN)
#define YUN_RELAIS_ON()			YUN_RELAIS_PORT |= _BV(YUN_RELAIS_PIN)
#define YUN_RELAIS_OFF( )		YUN_RELAIS_PORT &= ~_BV(YUN_RELAIS_PIN)


#endif

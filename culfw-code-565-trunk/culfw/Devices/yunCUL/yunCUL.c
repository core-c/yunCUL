/* Copyright Rudolf Koenig, 2008.
   Released under the GPL Licence, Version 2
   Inpired by the MyUSB USBtoSerial demo, Copyright (C) Dean Camera, 2008.
*/

#include <avr/boot.h>
#include <avr/power.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

// Arduino Yun has an ATmega32u4
// TODO: i think the next define can be removed here..
#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include <string.h>

// UJE yunCUL specific details
#include "board.h"

#include "spi.h"
#include "cc1100.h"
#include "clock.h"
#include "delay.h"
#include "display.h"
#include "serial.h"
#include "fncollection.h"
#include "led.h"
#include "ringbuffer.h"
#include "rf_receive.h"
#include "rf_send.h"
#include "rf_moritz.h"
#include "ttydata.h"
#include "fastrf.h"
#include "rf_router.h"
#include "i2cmaster.h"
#include "intertechno.h"
#include "adcw.h"
#include "cctemp.h"
#include "fht.h"
#include "memory.h"

// UJE: OLED
#ifdef HAS_YUN_OLED
#include "oled_ssd1306.h"
#endif

#ifdef HAS_ASKSIN
#include "rf_asksin.h"
#endif
#ifdef HAS_MORITZ
#include "rf_moritz.h"
#endif
#ifdef HAS_RWE
#include "rf_rwe.h"
#endif
#ifdef HAS_INTERTECHNO
#include "intertechno.h"
#endif
#ifdef HAS_SOMFY_RTS
#include "somfy_rts.h"
#endif
#ifdef HAS_MBUS
#include "rf_mbus.h"
#endif
#ifdef HAS_KOPP_FC
#include "kopp-fc.h"
#endif
#ifdef HAS_ZWAVE
#include "rf_zwave.h"
#endif


#ifdef HAS_CC1100_433
const uint8_t mark433_pin = 0x00;
#else
const uint8_t mark433_pin = 0xff;
#endif

const PROGMEM t_fntab fntab[] = {

#ifdef HAS_ASKSIN
  { 'A', asksin_func },
#endif
  { 'B', prepare_boot },
#ifdef HAS_MBUS
  { 'b', rf_mbus_func },
#endif
  { 'C', ccreg },
#ifdef HAS_RWE
  { 'E', rwe_func },
#endif
  { 'e', eeprom_factory_reset },
  { 'F', fs20send },
#ifdef HAS_FASTRF
  { 'f', fastrf_func },
#endif
#ifdef HAS_RAWSEND
  { 'G', rawsend },
#endif
#ifdef HAS_HOERMANN_SEND
  { 'h', hm_send },
#endif
#ifdef HAS_INTERTECHNO
  { 'i', it_func },
#endif
#ifdef HAS_RAWSEND
  { 'K', ks_send },
#endif
#ifdef HAS_KOPP_FC
  { 'k', kopp_fc_func },
#endif
#ifdef HAS_BELFOX
  { 'L', send_belfox },
#endif
  { 'l', ledfunc },
#ifdef HAS_RAWSEND
  { 'M', em_send },
#endif
#ifdef HAS_MEMFN
  { 'm', getfreemem },
#endif
#ifdef HAS_RFNATIVE
  { 'N', native_func },
#endif
#ifdef HAS_YUN_OLED
  { 'O', yun_oled_func },
#endif
  { 'R', read_eeprom },
#ifdef HAS_YUN_RELAIS
  { 'S', yun_relais_func },
#endif
  { 'T', fhtsend },
  { 't', gettime },
#ifdef HAS_UNIROLL
  { 'U', ur_send },
#endif
#ifdef HAS_RF_ROUTER
  { 'u', rf_router_func },
#endif
  { 'V', version },
  { 'W', write_eeprom },
  { 'X', set_txreport },
  { 'x', ccsetpa },
#ifdef HAS_SOMFY_RTS
  { 'Y', somfy_rts_func },
#endif
#ifdef HAS_MORITZ
  { 'Z', moritz_func },
#endif
#ifdef HAS_ZWAVE
  { 'z', zwave_func },
#endif
  { 0, 0 },
  { 0, 0 },
};

int
main(void)
{
	// wait for u-boot to finish..
	// On an Arduino Yun this will last about 1.5 minute (90 seconds)
	// To be on the safe side, we wait 120 seconds.. (that makes 600 loop-cycles).
	// ^^That's the old method.
	// Now we blink the LED for just a second, and continue the program (while Linux is still booting).
	// Once Linux has finished booting, it resets the AVR (using the script: reset-mcu).
	// The LED will blink again (for a second), program continues,
	// .. but this time, the serial connection (AVR<->Linux) will be opened succesfully, and the program works.

	my_delay_ms(250);
	my_delay_ms(250);

	// UJE: the relais connected to the yunCUL
#ifdef HAS_YUN_RELAIS
	yun_relais_init();
#endif

	my_delay_ms(250);
	my_delay_ms(250);

	led_init();
	for (int i=0; i<15; i++) { // 3 sec  600
		LED_ON();
		my_delay_ms(50);
		LED_OFF();
		my_delay_ms(150);
	}

	// UJE: OLED init and display splashscreen..
#ifdef HAS_YUN_OLED
	oled_init();
#endif

	my_delay_ms(250);
	my_delay_ms(250);
	
//	led_init();
	for (int i=0; i<15; i++) { // 3 sec  600
		LED_ON();
		my_delay_ms(200);
		LED_OFF();
		my_delay_ms(200);
	}

	oled_println(4,1,0, "UJE");

	for (int j=0; j<20; j++)
		for (int i=0; i<20; i++) {
			LED_ON();
			my_delay_ms(20-j);
			LED_OFF();
			my_delay_ms(j);
		}

  wdt_disable();
#ifdef HAS_16MHZ_CLOCK
  /* set clock to 16MHz/2 = 8Mhz */
  clock_prescale_set(clock_div_2);
#endif

//  LED_ON_DDR  |= _BV( LED_ON_PIN );
//  LED_ON_PORT |= _BV( LED_ON_PIN );

  //led_init();
  LED_ON();

  spi_init();
  // init_adcw();

  //eeprom_factory_reset("xx");
  eeprom_init();

  // Setup the timers. Are needed for watchdog-reset
  OCR0A  = 249;                            // Timer0: 0.008s = 8MHz/256/250 == 125Hz
  TCCR0B = _BV(CS02);
  TCCR0A = _BV(WGM01);
  TIMSK0 = _BV(OCIE0A);

  TCCR1A = 0;
  TCCR1B = _BV(CS11) | _BV(WGM12);         // Timer1: 1us = 8MHz/8


  MCUSR &= ~(1 << WDRF);                   // Enable the watchdog
  wdt_enable(WDTO_2S);

//#ifdef HAS_UART
#ifdef HAS_16MHZ_CLOCK
  uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
#else
  uart_init( UART_BAUD_SELECT_DOUBLE_SPEED(UART_BAUD_RATE,F_CPU) );
#endif

  fht_init();
  tx_init();
  input_handle_func = analyze_ttydata;

  display_channel = DISPLAY_USB;
//#endif

#ifdef HAS_RF_ROUTER
  rf_router_init();
  display_channel |= DISPLAY_RFROUTER;
#endif

  LED_OFF();

  sei();

  for(;;) {
    uart_task();
    RfAnalyze_Task();
    Minute_Task();
#ifdef HAS_FASTRF
    FastRF_Task();
#endif
#ifdef HAS_RF_ROUTER
    rf_router_task();
#endif
#ifdef HAS_ASKSIN
    rf_asksin_task();
#endif
#ifdef HAS_MORITZ
    rf_moritz_task();
#endif
#ifdef HAS_RWE
    rf_rwe_task();
#endif
#ifdef HAS_KOPP_FC
    kopp_fc_task();
#endif
#ifdef HAS_MBUS
    rf_mbus_task();
#endif
#ifdef HAS_ZWAVE
    rf_zwave_task();
#endif
  }

}

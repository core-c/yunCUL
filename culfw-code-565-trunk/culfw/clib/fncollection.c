#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "display.h"
#include "delay.h"
#include "fncollection.h"
#include "cc1100.h"
#include "../version.h"
#ifdef HAS_USB
#include <Drivers/USB/USB.h>
#endif
#include "clock.h"
#include "mysleep.h"
#include "fswrapper.h"
#include "fastrf.h"
#include "rf_router.h"
#include "ethernet.h"
#include <avr/wdt.h>
// UJE: OLED 128x32
#include "oled_ssd1306.h"

uint8_t led_mode = 1;   // Start off, but toggle on transmission

// The relais connected to the UJE yunCUL
#ifdef HAS_YUN_RELAIS
uint8_t yun_relais_state = 0; // relais off
#endif

#ifdef XLED
#include "xled.h"
#endif

//////////////////////////////////////////////////
// EEprom

// eeprom_write_byte is inlined and it is too big
__attribute__((__noinline__)) 
void
ewb(uint8_t *p, uint8_t v)
{
  eeprom_write_byte(p, v);
  eeprom_busy_wait();
}

// eeprom_read_byte is inlined and it is too big
__attribute__((__noinline__)) 
uint8_t
erb(uint8_t *p)
{
  return eeprom_read_byte(p);
}

static void
display_ee_bytes(uint8_t *a, uint8_t cnt)
{
  while(cnt--) {
    DH2(erb(a++));
    if(cnt)
      DC(':');
  }

}

static void
display_ee_mac(uint8_t *a)
{
  display_ee_bytes( a, 6 );
}

#ifdef HAS_ETHERNET
static void
display_ee_ip4(uint8_t *a)
{
  uint8_t cnt = 4;
  while(cnt--) {
    DU(erb(a++),1);
    if(cnt)
      DC('.');
  }
  
}
#endif

void
read_eeprom(char *in)
{
  uint8_t hb[2], d;
  uint16_t addr;

#ifdef HAS_ETHERNET
  if(in[1] == 'i') {
           if(in[2] == 'm') { display_ee_mac(EE_MAC_ADDR);
    } else if(in[2] == 'd') { DH2(erb(EE_USE_DHCP));
    } else if(in[2] == 'a') { display_ee_ip4(EE_IP4_ADDR);
    } else if(in[2] == 'n') { display_ee_ip4(EE_IP4_NETMASK);
    } else if(in[2] == 'g') { display_ee_ip4(EE_IP4_GATEWAY);
    } else if(in[2] == 'N') { display_ee_ip4(EE_IP4_NTPSERVER);
    } else if(in[2] == 'o') { DH2(erb(EE_IP4_NTPOFFSET));
    } else if(in[2] == 'p') {
      DU(eeprom_read_word((uint16_t *)EE_IP4_TCPLINK_PORT), 0);
    }
  } else 
#endif
  if(in[1] == 'M') { display_ee_mac(EE_DUDETTE_MAC); }  
  else if(in[1] == 'P') { display_ee_bytes(EE_DUDETTE_PUBL, 16); }  
  else {
    hb[0] = hb[1] = 0;
    d = fromhex(in+1, hb, 2);
    if(d == 2)
      addr = (hb[0] << 8) | hb[1];
    else
      addr = hb[0];

    d = erb((uint8_t *)addr);
    DC('R');                    // prefix
    DH(addr,4);                 // register number
    DS_P( PSTR(" = ") );
    DH2(d);                    // result, hex
    DS_P( PSTR(" / ") );
    DU(d,2);                    // result, decimal
  }
  DNL();
}

void
write_eeprom(char *in)
{
  uint8_t hb[6], d = 0;

#ifdef HAS_ETHERNET
  if(in[1] == 'i') {
    uint8_t *addr = 0;
           if(in[2] == 'm') { d=6; fromhex(in+3,hb,6); addr=EE_MAC_ADDR;
    } else if(in[2] == 'd') { d=1; fromdec(in+3,hb);   addr=EE_USE_DHCP;
    } else if(in[2] == 'a') { d=4; fromip (in+3,hb,4); addr=EE_IP4_ADDR;
    } else if(in[2] == 'n') { d=4; fromip (in+3,hb,4); addr=EE_IP4_NETMASK;
    } else if(in[2] == 'g') { d=4; fromip (in+3,hb,4); addr=EE_IP4_GATEWAY;
    } else if(in[2] == 'p') { d=2; fromdec(in+3,hb);   addr=EE_IP4_TCPLINK_PORT;
    } else if(in[2] == 'N') { d=4; fromip (in+3,hb,4); addr=EE_IP4_NTPSERVER;
    } else if(in[2] == 'o') { d=1; fromhex(in+3,hb,1); addr=EE_IP4_NTPOFFSET;
#ifdef HAS_NTP
      extern int8_t ntp_gmtoff;
      ntp_gmtoff = hb[0];
#endif
    }
    for(uint8_t i = 0; i < d; i++)
      ewb(addr++, hb[i]);

  } else 
#endif
  {
    uint16_t addr;
    d = fromhex(in+1, hb, 3);
    if(d < 2)
      return;
    if(d == 2)
      addr = hb[0];
    else
      addr = (hb[0] << 8) | hb[1];
      
    ewb((uint8_t*)addr, hb[d-1]);

    // If there are still bytes left, then write them too
    in += (2*d+1);
    while(in[0]) {
      if(!fromhex(in, hb, 1))
        return;
      ewb((uint8_t*)++addr, hb[0]);
      in += 2;
    }
  }
}

void
eeprom_init(void)
{

  if(erb(EE_MAGIC_OFFSET)   != VERSION_1 ||
     erb(EE_MAGIC_OFFSET+1) != VERSION_2)
       eeprom_factory_reset(0);

// UJE: the relais connected to the yunCUL
#ifdef HAS_YUN_RELAIS
	//! The next line is commented out, because we do not want the relais to enable at startup
	//yun_relais_state = erb(EE_YUN_RELAIS);
#endif

  led_mode = erb(EE_LED);
#ifdef XLED
  switch (led_mode) {
    case 0:
      xled_pattern = 0;
      break;
    case 1:
      xled_pattern = 0xffff;
      break;
    case 2:
      xled_pattern = 0xff00;
      break;
    case 3:
      xled_pattern = 0xa000;
      break;
    case 4:
      xled_pattern = 0xaa00;
      break;
  }
#endif
#ifdef HAS_SLEEP
  sleep_time = erb(EE_SLEEPTIME);
#endif
}

void
eeprom_factory_reset(char *in)
{

  ewb(EE_MAGIC_OFFSET  , VERSION_1);
  ewb(EE_MAGIC_OFFSET+1, VERSION_2);

  cc_factory_reset();

  ewb(EE_RF_ROUTER_ID, 0);
  ewb(EE_RF_ROUTER_ROUTER, 0);
  ewb(EE_REQBL, 0);
  ewb(EE_LED, 1);
// UJE: the relais connected to the yunCUL
#ifdef HAS_YUN_RELAIS
	ewb(EE_YUN_RELAIS, 0); // relais off
#endif
#ifdef HAS_LCD
  ewb(EE_CONTRAST,   0x40);
  ewb(EE_BRIGHTNESS, 0x80);
  ewb(EE_SLEEPTIME, 30);
#endif
#ifdef HAS_ETHERNET
  ethernet_reset();
#endif
#ifdef HAS_FS
  ewb(EE_LOGENABLED, 0x00);
#endif
#ifdef HAS_RF_ROUTER
  ewb(EE_RF_ROUTER_ID, 0x00);
  ewb(EE_RF_ROUTER_ROUTER, 0x00);
#endif

  if(in[1] != 'x')
    prepare_boot(0);
}

// LED
void
ledfunc(char *in)
{
  fromhex(in+1, &led_mode, 1);
#ifdef XLED
  switch (led_mode) {
    case 0:
      xled_pattern = 0; 
      break;
    case 1:
      xled_pattern = 0xffff; 
      break;
    case 2:
      xled_pattern = 0xff00; 
      break;
    case 3:
      xled_pattern = 0xa000; 
      break;
    case 4:
      xled_pattern = 0xaa00; 
      break;
  }
#else
  if(led_mode & 1)
    LED_ON();
  else
    LED_OFF();
#endif

  ewb(EE_LED, led_mode);
}


// UJE: the relais connected to the yunCUL
void yun_relais_func(char *in) {
	fromhex(in+1, &yun_relais_state, 1);
	if(yun_relais_state & 1)
		YUN_RELAIS_ON();
	else
		YUN_RELAIS_OFF();
	//ewb(EE_YUN_RELAIS, yun_relais_state);
}


// UJE: OLED 128x32 connected to the yunCUL
// *in text has the be in the format: SFBt
// 		char S = text Size.				value can be one of '1', '2', '3', '4'
//		char F = Foreground color.		value can be one of '0', '1'				(0 = BLACK, 1 = WHITE)
//		char B = Background color.		value can be one of '0', '1'				(0 = BLACK, 1 = WHITE)
//		char[] t = text					where every space ' ' must be annotated by a '_'
// There are special cases:
//		input text == "++" ? means: turn on display
//		input text == "--" ? means: turn off display
void yun_oled_func(char *in) {
	if (in[1]==0 || in[2]==0) return; // no S, or no F (or no ++, no --)
	if (in[1]=='+' && in[2]=='+') { oled_enable(SSD1306_DISPLAYON); return; }
	if (in[1]=='-' && in[2]=='-') { oled_enable(SSD1306_DISPLAYOFF); return; }
	if (in[3]==0 || in[4]==0) return; // no B, or no text
	// font size
	uint8_t s = 2;
	uint8_t logo = 0;
	switch(in[1]) {
		case '1':
			s = 1;
			break;
		case '2':
			s = 2;
			break;
		case '3':
			s = 3;
			break;
		case '4':
			s = 4;
			break;
		case '5':
			logo = 5; // 5 = print 16x16 Homekit logo, then text..
			break;
		case '6':
			logo = 6; // 6 = print 16x16 FHEM logo, then text..
			break;
		case '7':
			logo = 7; // 7 = print 16x16 Heater logo, then text..
			break;
	}
	// foreground color
	uint16_t c = (in[2] == '1')? WHITE : BLACK;
	// background color
	uint16_t bg = (in[3] == '0')? BLACK : WHITE;
	// the text (minus the leading command 'Osfb',   convert '_' to ' ')
	char *str = in + 4;
	for (uint16_t i=0; str[i]!=0; i++) if (str[i] == '_') str[i] = ' ';
	if (oled_enable(SSD1306_DISPLAYON) != 0) return; // if display is off?, turn it on
	if (logo)
		oled_printlnLogo(logo, str);
	else
		oled_println(s, c, bg, str);
}


//////////////////////////////////////////////////
// boot
void
prepare_boot(char *in)
{
  uint8_t bl = 0;
  if(in)
    fromhex(in+1, &bl, 1);

  if(bl == 0xff)             // Allow testing
    while(1);
    
  if(bl)                     // Next reboot we'd like to jump to the bootloader.
    ewb( EE_REQBL, 1 );      // Simply jumping to the bootloader from here
                             // wont't work. Neither helps to shutdown USB
                             // first.
                             
#ifdef HAS_USB
  USB_ShutDown();            // ??? Needed?
#endif
#ifdef HAS_FS
  fs_sync(&fs);              // Sync the filesystem
#endif


  TIMSK0 = 0;                // Disable the clock which resets the watchdog
  cli();
  
  wdt_enable(WDTO_15MS);       // Make sure the watchdog is running 
  while (1);                 // go to bed, the wathchdog will take us to reset
}

void
version(char *in)
{
#if defined(CUL_HW_REVISION)
  if (in[1] == 'H') {
    DS_P( PSTR(CUL_HW_REVISION) );
    DNL();
    return;
  }
#endif

#ifdef MULTI_FREQ_DEVICE     // check 433MHz version marker
  if (!bit_is_set(MARK433_PIN, MARK433_BIT))
    DS_P( PSTR("V " VERSION " " BOARD_ID_STR433) );
  else
#endif
  DS_P( PSTR("V " VERSION " " BOARD_ID_STR) );
  DNL();
}

void
dumpmem(uint8_t *addr, uint16_t len)
{
  for(uint16_t i = 0; i < len; i += 16) {
    uint8_t l = len;
    if(l > 16)
      l = 16;
    DH(i,4);
    DC(':'); DC(' ');
    for(uint8_t j = 0; j < l; j++) {
      DH2(addr[j]);
      if(j&1)
        DC(' ');
    }
    DC(' ');
    for(uint8_t j = 0; j < l; j++) {
      if(addr[j] >= ' ' && addr[j] <= '~')
        DC(addr[j]);
      else
        DC('.');
    }
    addr += 16;
    DNL();
  }
  DNL();
}

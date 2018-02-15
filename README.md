##yunCUL



####Description

culfw for Arduino Yún.



####Changes from the original culfw

- Added code for a new device ***yunCUL*** to ***culfw-code-565-trunk/culfw/Devices/***
- Adjusted ***culfw-code-565-trunk/culfw/clib/rf_send.h*** to get rid of the "not enough credits" bug
- Adjusted ***culfw-code-565-trunk/culfw/clib/serial.c*** to use the Yún's _Serial1_ hardware COM-port instead of _Serial_
- Adjusted ***culfw-code-565-trunk/culfw/clib/twimaster.c*** to initialize the I²C at 400 KHz instead of 100 KHz
- Adjusted ***culfw-code-565-trunk/culfw/clib/fncollection.h*** & ***fncollection.c*** to add support for OLED & yunCUL relais
- Added ***culfw-code-565-trunk/culfw/clib/oled_ssd1306.h*** & ***oled_ssd1306.c*** with the OLED code


####Compiling the code

You can compile the code on any Linux system. I even compiled it on a Yún with yun-gcc.
If you successfully compiled the code, you need to flash the ***yunCUL.hex*** and ***yunCUL.eep*** files into the Yún's AVR.

####Find out on what COM-port the Arduino Yún connects

Simply use the Arduino IDE, and check what COM-port the Yún is reporting.

####Flashing the files

- Open a command-prompt
- Change to the folder containing the files: yunCUL.hex & yunCUL.eep
- Invoke avrdude to program the files into flash & EEPROM memory

```

        cd C:\Users\C\Desktop\yunCUL
        C:\Users\C\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino9/bin/avrdude -CC:\Users\C\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino9/etc/avrdude.conf -v -patmega32u4 -cavr109 -PCOM6 -b57600 -D -Uflash:w:yunCUL.hex:i -Ueeprom:w:yunCUL.eep:i

```

> Note that the above lines are just an example.
> If you enable all the compiler verbose settings in the Arduino IDE preferences, and you upload _some_ sketch to your Yún, you can find out the exact paths of your own avrdude location.

You can extend the ***PATH*** environment variable to include the avrdude folder.
Here is an example using an Arduino Uno as ISP, to program the Yún:
> Note: You need to set fuse- & lock bits to be able to flash to bootloader address-space

```

        set PATH=%PATH%;C:\Users\C\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino9\bin
        cd C:\Users\C\Desktop\yunCUL
        avrdude -CC:\Users\C\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino9/etc/avrdude.conf -v -patmega32u4 -cstk500v1 -PCOM7 -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xfb:m -Uhfuse:w:0xd8:m -Ulfuse:w:0xff:m 
        avrdude -CC:\Users\C\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino9/etc/avrdude.conf -v -patmega32u4 -cstk500v1 -PCOM7 -b19200 -Uflash:w:yunCUL.hex:i -Ulock:w:0x2F:m

```
For our latest yunCUL AVR software we chose to use the full flash memory-range. This means that there is no longer a bootloader.
Because of the lack of a bootloader, the Yún will not report itself on a COM-port when connected with a USB-cable. The bootloader handles the COM-port connection, but it is absent.
This makes the yunCUL more hidden for the people who don't know what they're doing. At the same time it makes flashing the software into the yunCUL a bit less easy. You have to program the yunCUL using a programming-device. We used an Arduino Uno to flash the yunCUL.
To use an Uno as a programmer, you have to:
- Flash the Uno with the Arduino sketch ***YunSerialTerminal***
- Connect the ISCP header of the Yún to the Uno, as explained on this website:
 [ Arduino as your programmer](https://learn.sparkfun.com/tutorials/installing-an-arduino-bootloader)
- The Uno's COM-port is visible to host computer (whatever system you are using, the Uno-programmer will appear on _some_ COM-port).
 avr-dude commandline argument: *** -PCOM7 ***
- The Uno acts as an STK500 programmer.
 avr-dude commandline argument: *** -cstk500v1 ***
- You are programming the Yún, so select that MCU to flash.
 avr-dude commandline argument *** -patmega32u4 ***


####Links

[The CUL homepage](https://wiki.fhem.de/wiki/CUL)
[DIY](https://wiki.fhem.de/wiki/Selbstbau_CUL)
[The culfw source code](https://sourceforge.net/p/culfw/code/HEAD/tarball)

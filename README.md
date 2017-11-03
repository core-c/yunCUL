##yunCUL



##Description

culfw for Arduino Yún.






##Compiling the code

You can compile the code on any Linux system. I even compiled it on a Yún with yun-gcc.
If you successfully compiled the code, you need to flash the ***yunCUL.hex*** and ***yunCUL.eep*** files into the Yún's AVR.

##Find out on what COM-port the Arduino Yún connects

Simply use the Arduino IDE, and check what COM-port the Yún is reporting.

##Flashing the files

- Open a command-prompt
- Change to the folder containing the files: yunCUL.hex & yunCUL.eep
- Invoke avrdude to program the files into flash & EEPROM memory

```
  cd C:\Users\C\Desktop\yunCUL
  C:\Users\C\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino9/bin/avrdude -CC:\Users\C\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino9/etc/avrdude.conf -v -patmega32u4 -cavr109 -PCOM6 -b57600 -D -Uflash:w:yunCUL.hex:i -Ueeprom:w:yunCUL.eep:i
```

> Note that the above lines are just an example.
> If you enable all the compiler verbose settings in the Arduino IDE preferences, and you upload _some_ sketch to your Yún, you can find out the exact paths of your own avrdude location.





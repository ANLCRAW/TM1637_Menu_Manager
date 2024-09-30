# TM1637-Menu-Manager

# ToDoList

  * 
  * 

# Burn Bootloader

  Open CMD on Windows.  
  Type "cd C:\Users\Programming\Downloads\avrdude-v7.3-windows-x64"  
  
  Upload Bootloader with fuses set (enable CKOUT and BOD 2,7V):  
  avrdude -c arduino_as_isp -p m328p -P COMxx -U lfuse:w:0xBF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m -U flash:w:C:\Users\Programming\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\bootloaders\optiboot\optiboot_atmega328.hex
  
  Default Fuse Setting for Atmel328P:  
  avrdude -c arduino_as_isp -p m328p -P COMxx -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0xFD:m -U flash:w:C:\Users\Programming\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\bootloaders\optiboot\optiboot_atmega328.hex
  
  Read fuses:  
  avrdude -c arduino_as_isp -p m328p -P comxx -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

 

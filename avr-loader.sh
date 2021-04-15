#!/bin/bash
killall picocom
killall picocom
sleep 1
path_avrdude="../avrdude-6.3"
$path_avrdude/avrdude -C$path_avrdude/avrdude.conf -pm328p -carduino -P/dev/ttyUSB0 -b57600 -e -Uflash:w:$1/atmega328p_test.hex:a


#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
  EEPROM.write(0, 'a');
  EEPROM.write(1, 'w');
  EEPROM.write(2, 's');
  EEPROM.write(3, 1);
  EEPROM.write(4, 'd');
  EEPROM.write(5, '1');
  EEPROM.write(6, '2');
  EEPROM.write(7, '3');
  EEPROM.write(8, '4');
  EEPROM.write(9, '5');
  EEPROM.write(10, '6');
  EEPROM.write(11, '7');
  EEPROM.write(12, '8');
  EEPROM.write(13, '9');
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1);
}

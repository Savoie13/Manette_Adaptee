/*
  WriteEEPROM

  Code qui doit être téléchargé avant de télécharger Full_controllerV5.ino disponible au
  https://github.com/Savoie13/Manette_Adaptee/tree/main/Programmation/Full_controllerV5. 
  Il permet de mettre des valeurs au bonnes adresses de l'EEPROM du Arduino Pro Micro pour
  que lors du téléchargement du code principal "Full_controllerV5" les boutons de la 
  manette aies des valeurs de base. Dans ce code on met les boutons 0 à 14 à a, w, s, 
  "clic gauche", d, 1, 2, 3, 4, 5, 6, 7, 8, 9 respectivement.
    
  Hardware:
  - Arduino Micro Pro.
  - Cable micro USB.

  created 21/02/2023
  updated 11/04/2023
  by Mathis Savoie
*/

#include <EEPROM.h>

void setup() {
  // put your setup code here, to run once:
  EEPROM.write(0, 'd');
  EEPROM.write(1, 's');
  EEPROM.write(2, 'w');
  EEPROM.write(3, 1); //clic gauche
  EEPROM.write(4, 'a');
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

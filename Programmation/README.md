# Programmation
Ce dossier contient tous les programmes utiles au fonctionnement et à la conception de la manette adaptée. 
Aller voir le README.md dans les différents répertoires afin d'avoir plus d'information. 

Le programme dans le répertoire WriteEEPROM est le premier programme à télécharger dans l'arduino pro micro. Il permet d'installer la configuration par défaut dans l'EEPROM du pro micro.

Par la suite, il faut programmer la dernière version de Full_controllerV(x) dans l'arduino afin d'avoir le programme qui fait fonctionner la manette et réceptionne les JSON envoyé par l'ESP8266.

Le répertoire Code_Serveur_Spiffs contient le code Arduino IDE du L'ESp8266 ainsi que le répertoire "data" qui contient les fichiers nécéssaires pour le fonctionnement du serveur web. 

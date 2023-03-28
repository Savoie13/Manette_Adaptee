# Programmation
Ce dossier contient tous les programmes utiles au fonctionnement et à la conception de la manette adaptée. 
Le programme dans le répertoire WriteEEPROM est le premier programme à télécharger dans l'arduino pro micro. Il permet d'installer la configuration par défaut dans l'EEPROM du pro micro.

Par la suite, il faut programmer la dernière version de Full_controllerV(x) dans l'arduino afin d'avoir le programme qui fait fonctionner la manette et réceptionne les JSON envoyé par l'ESP8266.

Les répertoires "clavier", "mouse_control" contiennent des codes de base permettant d'utiliser l'arduino pro micro en clavier/sourie. Le répertoire "JSONRECEIVE" contient le code afin de réceptionner le JSON envoyé par l'ESP8266.

Le répertoire "ESP_MANETTE_WEB" contient les fichiers et documents que nous avons de besoin afin d'installer le programme de l'ESP8266 et les fichiers pour configurer le serveur web. 

/*
  Manette_Adaptée

  Manette de jeux vidéo à l'ordinateur pour les personnes à mobilitées réduites ou ayant un handicap cognitif leur empêchant d'utiliser une manette standard.
  
  Contrôle le curseur d'ordinateur avec un joystick et contrôle différentes touches du clavier et les boutons de la souris avec des boutons
    branchés sur Arduino Pro Micro. Contrôle aussi la réception de JSON arrivant sur son deuxième port série Serial1 pour modifier le
    fonctionnement de ses boutons. Les boutons de la manette sont préconfigurés de manière à ce qu'elle soit déja fonctionnelle pour la 
    plupart des jeux sans utiliser la fonction de configuration externe par JSON. 
    
  Hardware:
  - Arduino Micro Pro.
  - Cable micro USB.
  - 5 boutons branchés au pattes D10, D16, D15, D14 et D9 du Arduino Pro Micro.
  - Joystick 2 axes branché au pattes A0 et A1 du Arduino Pro Micro.
  - 5 résistances de 10K.

  Fonctionnement du joystick: 
    Le programme lit ses entrées analogiques A0 et A1 variants de 0 à 1023 et 
    converti ces valeurs entre -6 et 6. Quand la valeur des deux axes est 0,
    le curseur arrête de bouger et lorsque ces valeurs sont supérieurs à 0 le
    curseur bouge en fonction de la librairie Mouse.h. 

  Fonctionnement des boutons:
    Avec les entrées digitales configurées en entrées, le programme détecte
    lorsque les boutons sont appuyés et fait un Keyboard.press() de la valeur
    ASCII qui à été préalablement configuré pour ce bouton. Un Keyboard.release()
    est ensuite fait lorsque le bouton est relâché pour empêché que la touche
    du clavier soit encore appuyée. De plus, quand la valeur configuré pour le 
    bouton est en dessous de 3, le programme fait un Mouse.press() du clic
    gauche (1) ou du clic droit (2) de la souris au lieu d'un Keyboard.press()
    et puis fait unu Mouse.release().

  Fonctionnement de la réception de JSON:
    Le programme détecte lorsque que quelque chose est envoyé sur le port Serial1
    (TX1 et RX1). Il vérifie si c'est un JSON valide et renvoie un message d'erreur 
    sur le port série USB si ce n'en ai pas un. Si le JSON est valide il est séparé 
    et les valeurs décimales sont mises dans les variables des boutons et seront les 
    nouvelles valeurs des boutons. Par exemple, si la nouvelle valeur de la variable
    "bleu" est 48, le bouton bleu contrôlera la touche "1" du clavier.

  created 12/12/2022
  updated 16/02/2023
  by Mathis Savoie
*/

#include "Mouse.h"            //librairie pour le contrôle du curseur de la souris et ses boutons.
#include "Keyboard.h"         //librairie pour le contrôle du clavier de l'ordinateur.
#include <ArduinoJson.h>      //Librairie pour la réception et le décodage de JSON.
#include <EEPROM.h>

// Numéros de pates pour les boutons et le joystick
const int boutonBleu = 16;
const int boutonRouge = 14;
const int boutonVert = 15;
const int boutonBlanc = 20;
const int boutonJaune = 21;
const int boutonJack1 = 2;
const int boutonJack2 = 3;
const int boutonJack3 = 4;
const int boutonJack4 = 5;
const int boutonJack5 = 6;
const int boutonJack6 = 7;
const int boutonJack7 = 8;
const int boutonJack8 = 9;
const int boutonJack9 = 10;

const int xAxis = A0;       // axe X du joystick
const int yAxis = A1;       // axe Y du joystick

const String idBouton[14] = {"b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "b10", "b11", "b12", "b13", "b14"};

// Paramètres pour la lecture du joystick
int range = 12;             // range de valeurs pour le contrôle du curseur
int responseDelay = 5;      // délai de réponse de la souris en ms
int threshold = range / 4;  // seuil du joystick
int center = range / 2;     // valeur du joystick quand il ne bouge pas

int previousButtonState[14] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};

int button[14];

int buttonState[14];

void setup() 
{
  // put your setup code here, to run once:
  readEEPROM();
  //déclare les boutons en entrée pullup
  pinMode(boutonBleu, INPUT_PULLUP);
  pinMode(boutonRouge, INPUT_PULLUP);
  pinMode(boutonVert, INPUT_PULLUP);
  pinMode(boutonBlanc, INPUT_PULLUP);
  pinMode(boutonJaune, INPUT_PULLUP);
  pinMode(boutonJack1, INPUT_PULLUP);
  pinMode(boutonJack2, INPUT_PULLUP);
  pinMode(boutonJack3, INPUT_PULLUP);
  pinMode(boutonJack4, INPUT_PULLUP);
  pinMode(boutonJack5, INPUT_PULLUP);
  pinMode(boutonJack6, INPUT_PULLUP);
  pinMode(boutonJack7, INPUT_PULLUP);
  pinMode(boutonJack8, INPUT_PULLUP);
  pinMode(boutonJack9, INPUT_PULLUP);
  
  //Prend contrôle de la souris
  Mouse.begin(); 
  //Prend contrôle du Clavier
  Keyboard.begin();
  
  Serial1.begin(115200);        //démarre le port série 1 pour la réception de JSONs
  delay(5000);                //délai de 5 seconde au démarrage de la manette. Aide pour Mouse.h et Keyboard.h
}

void loop() 
{
  // Lit et met à l'échelle les axes
  int xReading = readAxis(A0);
  int yReading = readAxis(A1);

  //vérifie l'état des boutons
  buttonState[0] = digitalRead(boutonBleu);
  buttonState[1] = digitalRead(boutonRouge);
  buttonState[2] = digitalRead(boutonVert);
  buttonState[3] = digitalRead(boutonBlanc);
  buttonState[4] = digitalRead(boutonJaune);
  buttonState[5] = digitalRead(boutonJack1);
  buttonState[6] = digitalRead(boutonJack2);
  buttonState[7] = digitalRead(boutonJack3);
  buttonState[8] = digitalRead(boutonJack4);
  buttonState[9] = digitalRead(boutonJack5);
  buttonState[10] = digitalRead(boutonJack6);
  buttonState[11] = digitalRead(boutonJack7);
  buttonState[12] = digitalRead(boutonJack8);
  buttonState[13] = digitalRead(boutonJack9);

  Mouse.move(xReading, yReading, 0);                  //bouge le curseur en fonction de la position du joystick

  for(int i = 0; i < 14; i++)
  {
    if(buttonState[i] == HIGH && previousButtonState[i] == LOW)
    {
      if(button[i] > 4)
        Keyboard.press(button[i]);
      else
        Mouse.press(button[i]);
    }
    if(buttonState[i] == LOW && previousButtonState[i] == HIGH)
    {
      if(button[i] > 4)
        Keyboard.release(button[i]);
      else
        Mouse.release(button[i]);
    }
  }

  for(int i = 0; i < 14; i++)
  {
    previousButtonState[i] = buttonState[i];
  }

  if (Serial1.available())  //si quelque chose est reçu sur le port série 1
  {
    //Lit le JSON et le met dans la variable jsonString
    String jsonString = Serial1.readStringUntil('\n');
    Serial.println(jsonString);
    //Alloue un buffer pour l'objet JSON
    StaticJsonDocument<200> doc;
    

    //Sépare le JSON et le met dans le buffer
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error)  //si il n'arrive pas à séparer le JSON
    {
      Serial.println("Error parsing JSON data");    //envoie un message d'erreur sur le port série USB
      return;
    }

    int id = doc["id"];
    int valeur = doc["valeur"];

    for(int i = 0; i < 14; i++)
    {
      if(id == i + 1)
      {
        button[i] = valeur;
      }
    }

    

    sendJSONConfig();
    
    writeEEPROM();
  }
  
  delay(responseDelay);   //délai de réponse de la souris
}

/*
  Lit un axe (A0 ou A1 pour X ou Y) et le met à l'échelle selon la variable "range" au début du code
*/

int readAxis(int thisAxis) 
{
  // Lit l'entrée analogique
  int reading = analogRead(thisAxis);

  //Met la donnée 0 à 1023 à l'échelle selon "range"
  reading = map(reading, 0, 1023, 0, range);

  //S'assure que la position initiale du Joystick est 0
  int distance = reading - center;
  if (abs(distance) < threshold) {
    distance = 0;
  }

  //Retourne la distance entre -6 et 6 pour faire bouger le curseur
  return distance*-1;
}

void readEEPROM()
{
  for(int i = 0; i < 14; i++)
  {
    button[i] = EEPROM.read(i);
  }
  return;
}

void writeEEPROM()
{
  for(int i = 0; i < 14; i++)
  {
    EEPROM.write(i, button[i]);
  }
  return;
}

void sendJSONConfig()
{
  StaticJsonDocument<512> docRetour;
  
  for(int i = 0; i < 14; i++)
  {   
    if(button[i] > 32 && button[i] < 128)
    {
      char convertC = button[i];
      String convertS = String(convertC);
      docRetour[idBouton[i]] = convertS;
    }
    else
    {
      switch(button[i])
      {
        case 1:
          docRetour[idBouton[i]] = "Clic Gauche";
          break;
        case 2:
          docRetour[idBouton[i]] = "Clic Droit";
          break;
        case 4:
          docRetour[idBouton[i]] = "Clic Milieu";
          break;
        case 13:
          docRetour[idBouton[i]] = "ENTER";
          break;
        case 27:
          docRetour[idBouton[i]] = "ESC";
          break;
        case 32:
          docRetour[idBouton[i]] = "ESPACE";
          break;
        case 128:
          docRetour[idBouton[i]] = "Ctrl";
          break;
        case 129:
          docRetour[idBouton[i]] = "Shift";
          break;
        case 178:
          docRetour[idBouton[i]] = "Backspace";
          break;
        case 179:
          docRetour[idBouton[i]] = "Tab";
          break;
        case 215:
          docRetour[idBouton[i]] = "Flèche droite";
          break;
        case 216:
          docRetour[idBouton[i]] = "Flèche gauche";
          break;
        case 217:
          docRetour[idBouton[i]] = "Flèche bas";
          break;
        case 218:
          docRetour[idBouton[i]] = "Flèche haut";
          break;
        default:
          break;
      }
      
    }
      
  }

    
  serializeJson(docRetour, Serial1);
  Serial1.print("\n");
}

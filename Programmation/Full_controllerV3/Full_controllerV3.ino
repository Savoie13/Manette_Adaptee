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
  updated 14/02/2023
  by Mathis Savoie
*/

#include "Mouse.h"            //librairie pour le contrôle du curseur de la souris et ses boutons.
#include "Keyboard.h"         //librairie pour le contrôle du clavier de l'ordinateur.
#include <ArduinoJson.h>      //Librairie pour la réception et le décodage de JSON.

// Numéros de pattes pour les boutons et le joystick
const int boutonBleu = 10;
const int boutonRouge = 16;
const int boutonVert = 15;
const int boutonBlanc = 14;
const int boutonJaune = 9;
const int xAxis = A0;       // axe X du joystick
const int yAxis = A1;       // axe Y du joystick

// Paramètres pour la lecture du joystick
int range = 12;             // range de valeurs pour le contrôle du curseur
int responseDelay = 5;      // délai de réponse de la souris en ms
int threshold = range / 4;  // seuil du joystick
int center = range / 2;     // valeur du joystick quand il ne bouge pas

//dernière état des boutons
int previousButtonStateBleu = HIGH; 
int previousButtonStateRouge = HIGH;
int previousButtonStateVert = HIGH;
int previousButtonStateBlanc = HIGH;
int previousButtonStateJaune = HIGH;

//paramètres de base des boutons
int bleu = 'a';
int rouge = 'w';
int vert = 1;     //1 pour le clic gauche de la souris
int blanc = 's';
int jaune = 'd';

void setup() {
  //déclare les boutons en entrée pullup
  pinMode(boutonBleu, INPUT_PULLUP);
  pinMode(boutonRouge, INPUT_PULLUP);
  pinMode(boutonVert, INPUT_PULLUP);
  pinMode(boutonBlanc, INPUT_PULLUP);
  pinMode(boutonJaune, INPUT_PULLUP);
  
  //Prend contrôle de la souris
  Mouse.begin(); 
  //Prend contrôle du Clavier
  Keyboard.begin();
  
  Serial1.begin(9600);        //démarre le port série 1 pour la réception de JSONs
  delay(5000);                //délai de 5 seconde au démarrage de la manette. Aide pour Mouse.h et Keyboard.h
}

void loop() {   
  // Lit et met à l'échelle les axes
  int xReading = readAxis(A0);
  int yReading = readAxis(A1);
  
  //vérifie l'état des boutons
  int buttonStateBleu = digitalRead(boutonBleu);
  int buttonStateRouge = digitalRead(boutonRouge);
  int buttonStateVert = digitalRead(boutonVert);
  int buttonStateBlanc = digitalRead(boutonBlanc);
  int buttonStateJaune = digitalRead(boutonJaune);
  
  Mouse.move(xReading, yReading, 0);                  //bouge le curseur en fonction de la position du joystick
  
  //Si le bouton bleu est appuyé
  if (buttonStateBleu == HIGH && previousButtonStateBleu == LOW) 
  {
    if(bleu > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.press(bleu);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.press(bleu);
  }
  //Si le bouton bleu n'est plus appuyé
  if (buttonStateBleu == LOW && previousButtonStateBleu == HIGH) 
  {
    // and it's currently pressed:
    if(bleu > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(bleu);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(bleu);
  }
  
  //Si le bouton rouge est appuyé
  if (buttonStateRouge == HIGH && previousButtonStateRouge == LOW) 
  {
    // and it's currently pressed:
    if(rouge > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.press(rouge);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.press(rouge);  
  }
  //Si le bouton rouge n'est plus appuyé
  if (buttonStateRouge == LOW && previousButtonStateRouge == HIGH) 
  {
    // and it's currently released:
    if(rouge > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(rouge);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(rouge);
  }
  
  //Si le bouton vert est appuyé
  if (buttonStateVert == HIGH && previousButtonStateVert == LOW) 
  {
    // and it's currently pressed:
    if(vert > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.press(vert);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.press(vert);
  }
  //Si le bouton vert n'est plus appuyé
  if (buttonStateVert == LOW && previousButtonStateVert == HIGH) 
  {
    // and it's currently released:
    if(vert > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(vert);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(vert);
  }
  
  //Si le bouton blanc est appuyé
  if (buttonStateBlanc == HIGH && previousButtonStateBlanc == LOW) 
  {
    // and it's currently pressed:
    if(blanc > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.press(blanc);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.press(blanc);
  }
  //Si le bouton blanc n'est plus appuyé
  if (buttonStateBlanc == LOW && previousButtonStateBlanc == HIGH) 
  {
    // and it's currently released:
    if(blanc > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(blanc);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(blanc);
  }
  
  //Si le bouton jaune est appuyé
   if (buttonStateJaune == HIGH && previousButtonStateJaune == LOW) 
   {
      // and it's currently pressed:
     if(jaune > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jaune);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jaune);
  }
  //Si le bouton jaune n'est plus appuyé
  if (buttonStateJaune == LOW && previousButtonStateJaune == HIGH) 
  {
      // and it's currently released:
    if(jaune > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jaune);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jaune);
  }
  
  //Enregistre le dernier état des boutons
  previousButtonStateBleu = buttonStateBleu;
  previousButtonStateRouge = buttonStateRouge;
  previousButtonStateVert = buttonStateVert;
  previousButtonStateBlanc = buttonStateBlanc;
  previousButtonStateJaune = buttonStateJaune;


  if (Serial1.available())  //si quelque chose est reçu sur le port série 1
  {
    //Lit le JSON et le met dans la variable jsonString
    String jsonString = Serial1.readStringUntil('\n');

    //Alloue un buffer pour l'objet JSON
    StaticJsonDocument<200> doc;

    //Sépare le JSON et le met dans le buffer
    DeserializationError error = deserializeJson(doc, jsonString);
    
    if (error)  //si il n'arrive pas à séparer le JSON
    {
      Serial.println("Error parsing JSON data");    //envoie un message d'erreur sur le port série USB
      return;
    }

    //Va chercher les valeurs dans le JSON pour les mettre dans les variables des boutons
    bleu = doc["bleu"];
    rouge = doc["rouge"];
    vert = doc["vert"];
    blanc = doc["blanc"];
    jaune = doc["jaune"];

    //Affiche les nouvelles valeurs des boutons sur le port série USB
    Serial.print("Bouton bleu: ");
    Serial.println(bleu);
    Serial.print("Bouton rouge: ");
    Serial.println(rouge);
    Serial.print("Bouton vert: ");
    Serial.println(vert);
    Serial.print("Bouton blanc: ");
    Serial.println(blanc);
    Serial.print("Bouton jaune: ");
    Serial.println(jaune);
    
  }
  
  delay(responseDelay);   //délai de réponse de la souris
}

/*
  Lit un axe (A0 ou A1 pour X ou Y) et le met à l'échelle selon la variable "range" au début du code
*/

int readAxis(int thisAxis) {
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
  return distance;
}

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
  - 1 à 14 boutons branchés au pattes D2, D3, D4, D5, D6, D7, D8, D9, D10, D14, D15, D16, D20, D20 du Arduino Pro Micro. Le code fonctionne peut importe le nombre de boutons utilisés.
  - Joystick 2 axes branché au pattes A0 et A1 du Arduino Pro Micro.
  - 14 résistances de pulldown de 10KΩ branchées sur les 14 entrées digitales citées plus haut.

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
    du clavier "reste collée". De plus, quand la valeur configuré pour le 
    bouton est en dessous de 3, le programme fait un Mouse.press() du clic
    gauche (1) ou du clic droit (2) de la souris au lieu d'un Keyboard.press()
    et puis fait unu Mouse.release().

  Fonctionnement de la réception de JSON:
    Le programme détecte lorsque quelque chose est envoyé sur le port Serial1
    (TX1 et RX1). Il vérifie si c'est un JSON valide et renvoie un message d'erreur 
    sur le port série USB si ce n'en ai pas un. Si le JSON est valide il est séparé 
    et les valeurs décimales sont mises dans les variables des boutons et seront les 
    nouvelles valeurs des boutons. Par exemple, si la nouvelle valeur de la variable
    "bleu" est 48, le bouton bleu contrôlera la touche "1" du clavier.

  created 12/12/2022
  updated 20/02/2023
  by Mathis Savoie
*/

#include "Mouse.h"            //librairie pour le contrôle du curseur de la souris et ses boutons.
#include "Keyboard.h"         //librairie pour le contrôle du clavier de l'ordinateur.
#include <ArduinoJson.h>      //Librairie pour la réception et le décodage de JSON.

// Numéros de pattes pour les boutons et le joystick
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
int previousButtonStateJack1 = HIGH;
int previousButtonStateJack2 = HIGH;
int previousButtonStateJack3 = HIGH;
int previousButtonStateJack4 = HIGH;
int previousButtonStateJack5 = HIGH;
int previousButtonStateJack6 = HIGH;
int previousButtonStateJack7 = HIGH;
int previousButtonStateJack8 = HIGH;
int previousButtonStateJack9 = HIGH;


//paramètres de base des boutons
int bleu = 'a';
int rouge = 'w';
int vert = 1;     //1 pour le clic gauche de la souris
int blanc = 's';
int jaune = 'd';
int jack1 = '1';
int jack2 = '2';
int jack3 = '3';
int jack4 = '4';
int jack5 = '5';
int jack6 = '6';
int jack7 = '7';
int jack8 = '8';
int jack9 = '9';


void setup() {
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
  int buttonStateJack1 = digitalRead(boutonJack1);
  int buttonStateJack2 = digitalRead(boutonJack2);
  int buttonStateJack3 = digitalRead(boutonJack3);
  int buttonStateJack4 = digitalRead(boutonJack4);
  int buttonStateJack5 = digitalRead(boutonJack5);
  int buttonStateJack6 = digitalRead(boutonJack6);
  int buttonStateJack7 = digitalRead(boutonJack7);
  int buttonStateJack8 = digitalRead(boutonJack8);
  int buttonStateJack9 = digitalRead(boutonJack9);
  
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
    if(bleu > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(bleu);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(bleu);
  }
  
  //Si le bouton rouge est appuyé
  if (buttonStateRouge == HIGH && previousButtonStateRouge == LOW) 
  {
    if(rouge > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.press(rouge);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.press(rouge);  
  }
  //Si le bouton rouge n'est plus appuyé
  if (buttonStateRouge == LOW && previousButtonStateRouge == HIGH) 
  {
    if(rouge > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(rouge);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(rouge);
  }
  
  //Si le bouton vert est appuyé
  if (buttonStateVert == HIGH && previousButtonStateVert == LOW) 
  {
    if(vert > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.press(vert);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.press(vert);
  }
  //Si le bouton vert n'est plus appuyé
  if (buttonStateVert == LOW && previousButtonStateVert == HIGH) 
  {
    if(vert > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(vert);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(vert);
  }
  
  //Si le bouton blanc est appuyé
  if (buttonStateBlanc == HIGH && previousButtonStateBlanc == LOW) 
  {
    if(blanc > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.press(blanc);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.press(blanc);
  }
  //Si le bouton blanc n'est plus appuyé
  if (buttonStateBlanc == LOW && previousButtonStateBlanc == HIGH) 
  {
    if(blanc > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(blanc);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(blanc);
  }
  
  //Si le bouton jaune est appuyé
  if (buttonStateJaune == HIGH && previousButtonStateJaune == LOW) 
  {
     if(jaune > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jaune);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jaune);
  }
  //Si le bouton jaune n'est plus appuyé
  if (buttonStateJaune == LOW && previousButtonStateJaune == HIGH) 
  {
    if(jaune > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jaune);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jaune);
  }
  //Si le bouton jack1 est appuyé
  if (buttonStateJack1 == HIGH && previousButtonStateJack1 == LOW) 
  {
     if(jack1 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack1);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack1);
  }
  //Si le bouton jack1 n'est plus appuyé
  if (buttonStateJack1 == LOW && previousButtonStateJack1 == HIGH) 
  {
    if(jack1 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack1);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack1);
  }
  //Si le bouton jack2 est appuyé
  if (buttonStateJack2 == HIGH && previousButtonStateJack2 == LOW) 
  {
     if(jack2 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack2);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack2);
  }
  //Si le bouton jack2 n'est plus appuyé
  if (buttonStateJack2 == LOW && previousButtonStateJack2 == HIGH) 
  {
    if(jack2 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack2);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack2);
  }
  //Si le bouton jack3 est appuyé
  if (buttonStateJack3 == HIGH && previousButtonStateJack3 == LOW) 
  {
     if(jack3 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack3);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack3);
  }
  //Si le bouton jack3 n'est plus appuyé
  if (buttonStateJack3 == LOW && previousButtonStateJack3 == HIGH) 
  {
    if(jack3 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack3);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack3);
  }
  //Si le bouton jack4 est appuyé
  if (buttonStateJack4 == HIGH && previousButtonStateJack4 == LOW) 
  {
     if(jack4 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack4);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack4);
  }
  //Si le bouton jack4 n'est plus appuyé
  if (buttonStateJack4 == LOW && previousButtonStateJack4 == HIGH) 
  {
    if(jack4 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack4);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack4);
  }
  //Si le bouton jack5 est appuyé
  if (buttonStateJack5 == HIGH && previousButtonStateJack5 == LOW) 
  {
     if(jack5 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack5);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack5);
  }
  //Si le bouton jack5 n'est plus appuyé
  if (buttonStateJack5 == LOW && previousButtonStateJack5 == HIGH) 
  {
    if(jack5 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack5);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack5);
  }
  //Si le bouton jack6 est appuyé
  if (buttonStateJack6 == HIGH && previousButtonStateJack6 == LOW) 
  {
     if(jack6 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack6);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack6);
  }
  //Si le bouton jack6 n'est plus appuyé
  if (buttonStateJack6 == LOW && previousButtonStateJack6 == HIGH) 
  {
    if(jack6 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack6);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack6);
  }
  //Si le bouton jack7 est appuyé
  if (buttonStateJack7 == HIGH && previousButtonStateJack7 == LOW) 
  {
     if(jack7 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack7);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack7);
  }
  //Si le bouton jack7 n'est plus appuyé
  if (buttonStateJack7 == LOW && previousButtonStateJack7 == HIGH) 
  {
    if(jack7 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack7);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack7);
  }
  //Si le bouton jack8 est appuyé
  if (buttonStateJack8 == HIGH && previousButtonStateJack8 == LOW) 
  {
     if(jack8 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack8);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack8);
  }
  //Si le bouton jack8 n'est plus appuyé
  if (buttonStateJack8 == LOW && previousButtonStateJack8 == HIGH) 
  {
    if(jack8 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack8);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack8);
  }
  //Si le bouton jack9 est appuyé
  if (buttonStateJack9 == HIGH && previousButtonStateJack9 == LOW) 
  {
     if(jack9 > 3)              //si le bouton contrôle les boutons du clavier
       Keyboard.press(jack9);
     else                      //si le bonton contrôle les boutons de la souris
       Mouse.press(jack9);
  }
  //Si le bouton jack9 n'est plus appuyé
  if (buttonStateJack9 == LOW && previousButtonStateJack9 == HIGH) 
  {
    if(jack9 > 3)              //si le bouton contrôle les boutons du clavier
      Keyboard.release(jack9);
    else                      //si le bonton contrôle les boutons de la souris
      Mouse.release(jack9);
  }

  
  //Enregistre le dernier état des boutons
  previousButtonStateBleu = buttonStateBleu;
  previousButtonStateRouge = buttonStateRouge;
  previousButtonStateVert = buttonStateVert;
  previousButtonStateBlanc = buttonStateBlanc;
  previousButtonStateJaune = buttonStateJaune;
  previousButtonStateJack1 = buttonStateJack1;
  previousButtonStateJack2 = buttonStateJack2;
  previousButtonStateJack3 = buttonStateJack3;
  previousButtonStateJack4 = buttonStateJack4;
  previousButtonStateJack5 = buttonStateJack5;
  previousButtonStateJack6 = buttonStateJack6;
  previousButtonStateJack7 = buttonStateJack7;
  previousButtonStateJack8 = buttonStateJack8;
  previousButtonStateJack9 = buttonStateJack9;

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
    
    switch(id)
    {
      case 1:
        bleu = valeur;
        break;
      case 2:
        rouge = valeur;
        break;
      case 3:
        vert = valeur;
        break;
      case 4:
        blanc = valeur;
        break;
      case 5:
        jaune = valeur;
        break;
      case 6:
        jack1 = valeur;
        break;
      case 7:
        jack2 = valeur;
        break;
      case 8:
        jack3 = valeur;
        break;
      case 9:
        jack4 = valeur;
        break;
      case 10:
        jack5 = valeur;
        break;
      case 11:
        jack6 = valeur;
        break;
      case 12:
        jack7 = valeur;
        break;
      case 13:
        jack8 = valeur;
        break;
      case 14:
        jack9 = valeur;
        break;
    }
    
    //Va chercher les valeurs dans le JSON pour les mettre dans les variables des boutons 

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
    Serial.print("Jack 1: ");
    Serial.println(jack1);
    Serial.print("Jack 2: ");
    Serial.println(jack2);
    Serial.print("Jack 3: ");
    Serial.println(jack3);
    Serial.print("Jack 4: ");
    Serial.println(jack4);
    Serial.print("Jack 5: ");
    Serial.println(jack5);
    Serial.print("Jack 6: ");
    Serial.println(jack6);
    Serial.print("Jack 7: ");
    Serial.println(jack7);
    Serial.print("Jack 8: ");
    Serial.println(jack8);
    Serial.print("Jack 9: ");
    Serial.println(jack9);
    

    
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

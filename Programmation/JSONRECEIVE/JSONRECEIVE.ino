#include <ArduinoJson.h>
#include "Keyboard.h"
#include "Mouse.h"

const int boutonBleu = 10;
const int boutonRouge = 16;
const int boutonVert = 15;
const int boutonBlanc = 14;
const int boutonJaune = 9;

int responseDelay = 5;      // response delay of the mouse, in ms

int previousButtonStateBleu = HIGH; 
int previousButtonStateRouge = HIGH;
int previousButtonStateVert = HIGH;
int previousButtonStateBlanc = HIGH;
int previousButtonStateJaune = HIGH;

int bleu = 97;
int rouge = 119;
int vert = 122;
int blanc = 115;
int jaune = 100;


void setup() 
{
  //declare the buttons as input_pullup
  pinMode(boutonBleu, INPUT_PULLUP);
  pinMode(boutonRouge, INPUT_PULLUP);
  pinMode(boutonVert, INPUT_PULLUP);
  pinMode(boutonBlanc, INPUT_PULLUP);
  pinMode(boutonJaune, INPUT_PULLUP);

  Mouse.begin();
  Keyboard.begin();
  Serial1.begin(9600); // start the serial communication at 9600 baud rate
  delay(5000);
}

void loop() 
{
  int buttonStateBleu = digitalRead(boutonBleu);
  int buttonStateRouge = digitalRead(boutonRouge);
  int buttonStateVert = digitalRead(boutonVert);
  int buttonStateBlanc = digitalRead(boutonBlanc);
  int buttonStateJaune = digitalRead(boutonJaune);
  
  //replace button press with a left mouse click
  if (buttonStateVert == HIGH && previousButtonStateVert == LOW) {
      // and it's currently pressed:
    Mouse.press(MOUSE_LEFT);
  }
  if (buttonStateVert == LOW && previousButtonStateVert == HIGH) {
      // and it's currently pressed:
    Mouse.release(MOUSE_LEFT);
  }
  
  //replaces button press with D
  if (buttonStateJaune == HIGH && previousButtonStateJaune == LOW) {
      // and it's currently pressed:
    Keyboard.press(jaune);
  }
  if (buttonStateJaune == LOW && previousButtonStateJaune == HIGH) {
      // and it's currently released:
    Keyboard.release(jaune);
  }
  
  //replaces button press with S
  if (buttonStateBlanc == HIGH && previousButtonStateBlanc == LOW) {
      // and it's currently pressed:
    Keyboard.press(blanc);
  }
  if (buttonStateBlanc == LOW && previousButtonStateBlanc == HIGH) {
      // and it's currently released:
    Keyboard.release(blanc);
  }
  
  //replaces button press with W
  if (buttonStateRouge == HIGH && previousButtonStateRouge == LOW) {
      // and it's currently pressed:
    Keyboard.press(rouge);
  }
  if (buttonStateRouge == LOW && previousButtonStateRouge == HIGH) {
      // and it's currently released:
    Keyboard.release(rouge);
  }
  
  //replaces button press with A
   if (buttonStateBleu == HIGH && previousButtonStateBleu == LOW) {
      // and it's currently pressed:
    Keyboard.press(bleu);
  }
  if (buttonStateBleu == LOW && previousButtonStateBleu == HIGH) {
      // and it's currently released:
    Keyboard.release(bleu);
  }

  previousButtonStateBleu = buttonStateBleu;
  previousButtonStateRouge = buttonStateRouge;
  previousButtonStateVert = buttonStateVert;
  previousButtonStateBlanc = buttonStateBlanc;
  previousButtonStateJaune = buttonStateJaune;
  
  if (Serial1.available()) 
  {
    // Read the incoming JSON data from the serial connection
    String jsonString = Serial1.readStringUntil('\n');

    // Allocate a buffer for the JSON object
    StaticJsonDocument<200> doc;

    // Parse the JSON data and store it in the buffer
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) 
    {
      Serial.println("Error parsing JSON data");
      return;
    }

    // Extract the data from the JSON object
    bleu = doc["bleu"];
    rouge = doc["rouge"];
    vert = doc["vert"];
    blanc = doc["blanc"];
    jaune = doc["jaune"];

    // Use the extracted data as needed
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
  
  delay(responseDelay);
}

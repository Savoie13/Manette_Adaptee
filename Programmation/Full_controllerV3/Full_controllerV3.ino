/*
  Full_controller

  Controls the mouse from a joystick and keyboard keys (A,W,S,D) with boutons on an Arduino Leonardo, Micro or Due.
  
  Uses the joystick's push button to click the left mouse button.

  Hardware:
  - 2-axis joystick connected to pins A0 and A1
  - 5 pushbuttons connected to pin D15, D10, D14, D9 and D16

  The mouse movement is always relative. This sketch reads two analog inputs
  that range from 0 to 1023 (or less on either end) and translates them into
  ranges of -6 to 6.
  The sketch assumes that the joystick resting values are around the middle of
  the range, but that they vary within a threshold.

  WARNING: When you use the Mouse.move() command, the Arduino takes over your
  mouse! Make sure you have control before you use the command. This sketch
  includes a pushbutton to toggle the mouse control state, so you can turn on
  and off mouse control.

  created 12/12/2022
  updated 12/12/2022
  by Mathis Savoie
*/

#include "Mouse.h"
#include "Keyboard.h"
#include <ArduinoJson.h>

// set pin numbers for switch, joystick axes, and LED:
const int boutonBleu = 10;
const int boutonRouge = 16;
const int boutonVert = 15;
const int boutonBlanc = 14;
const int boutonJaune = 9;

const int xAxis = A0;       // joystick X axis
const int yAxis = A1;       // joystick Y axis

// parameters for reading the joystick:
int range = 12;             // output range of X or Y movement
int responseDelay = 5;      // response delay of the mouse, in ms
int threshold = range / 4;  // resting threshold
int center = range / 2;     // resting position value

bool mouseIsActive = true;  // whether or not to control the mouse

int previousButtonStateBleu = HIGH; 
int previousButtonStateRouge = HIGH;
int previousButtonStateVert = HIGH;
int previousButtonStateBlanc = HIGH;
int previousButtonStateJaune = HIGH;

int bleu = 97;
int rouge = 119;
int vert = 32;
int blanc = 115;
int jaune = 100;

void setup() {
  //declare the buttons as input_pullup
  pinMode(boutonBleu, INPUT_PULLUP);
  pinMode(boutonRouge, INPUT_PULLUP);
  pinMode(boutonVert, INPUT_PULLUP);
  pinMode(boutonBlanc, INPUT_PULLUP);
  pinMode(boutonJaune, INPUT_PULLUP);
  
  // take control of the mouse:
  Mouse.begin(); 
  //take control of the keyboard
  Keyboard.begin();
  
  Serial1.begin(9600);
  delay(5000);
}

void loop() {   
  // read and scale the two axes:
  int xReading = readAxis(A0);
  int yReading = readAxis(A1);
  
  //checking the state of the button
  int buttonStateBleu = digitalRead(boutonBleu);
  int buttonStateRouge = digitalRead(boutonRouge);
  int buttonStateVert = digitalRead(boutonVert);
  int buttonStateBlanc = digitalRead(boutonBlanc);
  int buttonStateJaune = digitalRead(boutonJaune);
  
  // if the mouse control state is active, move the mouse:
  if (mouseIsActive) 
  {
    Mouse.move(xReading, yReading, 0);
  }

  
  //replace button press with a left mouse click
  if (buttonStateBleu == HIGH && previousButtonStateBleu == LOW) {
      // and it's currently pressed:
    Keyboard.press(bleu);
  }
  if (buttonStateBleu == LOW && previousButtonStateBleu == HIGH) {
      // and it's currently pressed:
    Keyboard.release(bleu);
  }
  
  //replaces button press with D
  if (buttonStateRouge == HIGH && previousButtonStateRouge == LOW) {
      // and it's currently pressed:
    Keyboard.press(rouge);
  }
  if (buttonStateRouge == LOW && previousButtonStateRouge == HIGH) {
      // and it's currently released:
    Keyboard.release(rouge);
  }
  
  //replaces button press with S
  if (buttonStateVert == HIGH && previousButtonStateVert == LOW) {
      // and it's currently pressed:
    Keyboard.press(vert);
  }
  if (buttonStateVert == LOW && previousButtonStateVert == HIGH) {
      // and it's currently released:
    Keyboard.release(vert);
  }
  
  //replaces button press with W
  if (buttonStateBlanc == HIGH && previousButtonStateBlanc == LOW) {
      // and it's currently pressed:
    Keyboard.press(blanc);
  }
  if (buttonStateBlanc == LOW && previousButtonStateBlanc == HIGH) {
      // and it's currently released:
    Keyboard.release(blanc);
  }
  
  //replaces button press with A
   if (buttonStateJaune == HIGH && previousButtonStateJaune == LOW) {
      // and it's currently pressed:
    Keyboard.press(jaune);
  }
  if (buttonStateJaune == LOW && previousButtonStateJaune == HIGH) {
      // and it's currently released:
    Keyboard.release(jaune);
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

/*
  reads an axis (0 or 1 for x or y) and scales the analog input range to a range
  from 0 to <range>
*/

int readAxis(int thisAxis) {
  // read the analog input:
  int reading = analogRead(thisAxis);

  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;

  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

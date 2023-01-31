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


// set pin numbers for switch, joystick axes, and LED:
const int mouseButton = 2;  // input pin for the mouse pushButton
const int buttonD = 9;          
const int buttonS = 14;
const int buttonW = 16;   
const int buttonA = 10;
const int xAxis = A0;       // joystick X axis
const int yAxis = A1;       // joystick Y axis

// parameters for reading the joystick:
int range = 12;             // output range of X or Y movement
int responseDelay = 5;      // response delay of the mouse, in ms
int threshold = range / 4;  // resting threshold
int center = range / 2;     // resting position value

bool mouseIsActive = true;  // whether or not to control the mouse
int previousButtonStateD = HIGH; 
int previousButtonStateS = HIGH;
int previousButtonStateW = HIGH;
int previousButtonStateA = HIGH;
int previousMouseButtonState = LOW;

void setup() {
  //declare the buttons as input_pullup
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonS, INPUT_PULLUP);
  pinMode(buttonW, INPUT_PULLUP);
  pinMode(buttonA, INPUT_PULLUP);
  // take control of the mouse:
  Mouse.begin();
  //take control of the keyboard
  Keyboard.begin();
  Serial1.begin(9600);
  delay(5000);
}

void loop() { 

  String incomingString = "";
  boolean stringReady = false;
  
  // read and scale the two axes:
  int xReading = readAxis(A0);
  int yReading = readAxis(A1);
  //checking the state of the button
  int buttonStateD = digitalRead(buttonD);
  int buttonStateS = digitalRead(buttonS);
  int buttonStateW = digitalRead(buttonW);
  int buttonStateA = digitalRead(buttonA);
  int mouseButtonState = digitalRead(mouseButton);
  
  // if the mouse control state is active, move the mouse:
  if (mouseIsActive) 
  {
    Mouse.move(xReading, yReading, 0);
  }

  
  //replace button press with a left mouse click
  if (mouseButtonState == LOW && previousMouseButtonState == HIGH) {
      // and it's currently pressed:
    Mouse.press(MOUSE_LEFT);
  }
  if (mouseButtonState == HIGH && previousMouseButtonState == LOW) {
      // and it's currently pressed:
    Mouse.release(MOUSE_LEFT);
  }
  
  //replaces button press with D
  if (buttonStateD == HIGH && previousButtonStateD == HIGH) {
      // and it's currently pressed:
    Keyboard.press('d');
  }
  if (buttonStateD == LOW && previousButtonStateD == HIGH) {
      // and it's currently released:
    Keyboard.release('d');
  }
  
  //replaces button press with S
  if (buttonStateS == HIGH && previousButtonStateS == LOW) {
      // and it's currently pressed:
    Keyboard.press('s');
  }
  if (buttonStateS == LOW && previousButtonStateS == HIGH) {
      // and it's currently released:
    Keyboard.release('s');
  }
  
  //replaces button press with W
  if (buttonStateW == HIGH && previousButtonStateW == LOW) {
      // and it's currently pressed:
    Keyboard.press('w');
  }
  if (buttonStateW == LOW && previousButtonStateW == HIGH) {
      // and it's currently released:
    Keyboard.release('w');
  }
  
  //replaces button press with A
   if (buttonStateA == HIGH && previousButtonStateA == LOW) {
      // and it's currently pressed:
    Keyboard.press('a');
  }
  if (buttonStateA == LOW && previousButtonStateA == HIGH) {
      // and it's currently released:
    Keyboard.release('a');
  }

  previousButtonStateD = buttonStateD;
  previousButtonStateS = buttonStateS;
  previousButtonStateW = buttonStateW;
  previousButtonStateA = buttonStateA;
  previousMouseButtonState = mouseButtonState;

  while (Serial1.available()){
    incomingString = Serial1.readString();
    stringReady = true;
  }

  if(stringReady){
    Serial.println("String reçu: " + incomingString);
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

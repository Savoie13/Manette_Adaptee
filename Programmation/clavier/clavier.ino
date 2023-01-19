#include "Keyboard.h"

//declaring button pins
const int buttonD = 10;          
const int buttonS = 16;
const int buttonW = 14;   
const int buttonA = 15;
//const int buttonPin4 = 6;


int previousButtonStateD = HIGH; 
int previousButtonStateS = HIGH;
int previousButtonStateW = HIGH;
int previousButtonStateA = HIGH;
//int previousButtonState4 = HIGH;



void setup() {
  //declare the buttons as input_pullup
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonS, INPUT_PULLUP);
  pinMode(buttonW, INPUT_PULLUP);
  pinMode(buttonA, INPUT_PULLUP);
  //pinMode(buttonPin4, INPUT_PULLUP);
  
  Keyboard.begin();
}

void loop() {
  //checking the state of the button
  int buttonStateD = digitalRead(buttonD);
  int buttonStateS = digitalRead(buttonS);
  int buttonStateW = digitalRead(buttonW);
  int buttonStateA = digitalRead(buttonA);
  //int buttonState4 = digitalRead(buttonPin4);
  
 //replaces button press with AWSD
  if (buttonStateD == HIGH && previousButtonStateD == HIGH) {
      // and it's currently pressed:
    Keyboard.press('d');
  }

  if (buttonStateD == LOW && previousButtonStateD == HIGH) {
      // and it's currently released:
    Keyboard.release('d');
  }
  
  //replaces button press with DOWN arrow
  if (buttonStateS == HIGH && previousButtonStateS == LOW) {
      // and it's currently pressed:
    Keyboard.press('s');
  }

  if (buttonStateS == LOW && previousButtonStateS == HIGH) {
      // and it's currently released:
    Keyboard.release('s');
  }
  
  //replaces button press with RIGHT arrow
  if (buttonStateW == HIGH && previousButtonStateW == LOW) {
      // and it's currently pressed:
    Keyboard.press('w');
  }

  if (buttonStateW == LOW && previousButtonStateW == HIGH) {
      // and it's currently released:
    Keyboard.release('w');
  }
  
  //replaces button press with LEFT arrow
   if (buttonStateA == HIGH && previousButtonStateA == LOW) {
      // and it's currently pressed:
    Keyboard.press('a');
  }

  if (buttonStateA == LOW && previousButtonStateA == HIGH) {
      // and it's currently released:
    Keyboard.release('a');
  }
  
//replaces button press with SPACE BAR
/*if (buttonState4 == LOW && previousButtonState4 == HIGH) {
      // and it's currently pressed:
    Keyboard.press(32);
  }

  if (buttonState4 == HIGH && previousButtonState4 == LOW) {
      // and it's currently released:
    Keyboard.release(32);
  }
  */
  //checking the previous state of the button
  
  previousButtonStateD = buttonStateD;
  previousButtonStateS = buttonStateS;
  previousButtonStateW = buttonStateW;
  previousButtonStateA = buttonStateA;
  //previousButtonState4 = buttonState4;

}

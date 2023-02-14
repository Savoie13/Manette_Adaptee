#include <Arduino.h>
#include <ArduinoJson.h>


#define BUTTON_SWITCH 16

bool mode = true;


void setup() {
  pinMode(BUTTON_SWITCH, INPUT_PULLUP);
  Serial.begin(9600); // start the serial communication at 9600 baud rate
}

void loop() {
  if(digitalRead(BUTTON_SWITCH) == HIGH)
  {
    while(digitalRead(BUTTON_SWITCH) == HIGH);
    if(mode == true)
    {
      // Create the JSON object
      StaticJsonDocument<200> doc;
      doc["bleu"] = 49;
      doc["rouge"] = 50;
      //doc["vert"] = 51;
      //doc["blanc"] = 52;
      //doc["jaune"] = 53;
      // Serialize the JSON object to a string
      String jsonString;
      serializeJson(doc, jsonString);
    
      // Send the serialized JSON string to the Arduino
      Serial.println(jsonString);
      mode = false;
    }
    else
    {
      // Create the JSON object
      StaticJsonDocument<200> doc;
      doc["bleu"] = 97;
      doc["rouge"] = 119;
      doc["vert"] = 1;
      doc["blanc"] = 115;
      doc["jaune"] = 100;
      // Serialize the JSON object to a string
      String jsonString;
      serializeJson(doc, jsonString);
    
      // Send the serialized JSON string to the Arduino
      Serial.println(jsonString);
      mode = true;
    }
  }
  delay(10); // wait for 1 second
}

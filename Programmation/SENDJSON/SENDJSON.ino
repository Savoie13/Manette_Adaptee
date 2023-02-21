#include <Arduino.h>
#include <ArduinoJson.h>


#define BUTTON_SWITCH 16

bool mode = true;
int i = 0;
int id[14] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
int valeur[14] = {49,50,51,52,53,54,55,56,57,97,98,99,100,101};

void setup() {
  pinMode(BUTTON_SWITCH, INPUT_PULLUP);
  Serial.begin(9600); // start the serial communication at 9600 baud rate
}

void loop() {
  if(digitalRead(BUTTON_SWITCH) == HIGH)
  {
    while(digitalRead(BUTTON_SWITCH) == HIGH);
    // Create the JSON object
    StaticJsonDocument<200> doc;
    doc["id"] = id[i];
    doc["valeur"] = valeur[i];
    // Serialize the JSON object to a string
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Send the serialized JSON string to the Arduino
    Serial.println(jsonString);
    mode = false;
    i++;
    if(i == 14)
    {
      i = 0;
    }
  }
  delay(10); // wait for 1 second
}

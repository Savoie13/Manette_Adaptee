#include <Arduino.h>
#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600); // start the serial communication at 9600 baud rate
}

void loop() {
  // Create the JSON object
  StaticJsonDocument<200> doc;
  doc["bleu"] = 49;
  doc["rouge"] = 50;
  doc["vert"] = 51;
  doc["blanc"] = 52;
  doc["jaune"] = 53;

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);

  // Send the serialized JSON string to the Arduino
  Serial.println(jsonString);
  
  delay(30000); // wait for 1 second
}

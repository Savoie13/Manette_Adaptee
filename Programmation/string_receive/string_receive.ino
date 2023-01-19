
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  String incomingString = "";
  boolean stringReady = false;

  while (Serial1.available()){
    incomingString = Serial1.readString();
    stringReady = true;
  }

  if(stringReady){
    Serial.println("Received String: " + incomingString);
  }
}

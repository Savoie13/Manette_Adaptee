
// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include <Wire.h>
#include <WebSocketsServer.h>
#include <DNSServer.h>


const byte DNS_PORT = 53;
DNSServer dnsServer;
AsyncWebSocket ws("/ws");
AsyncWebServer server(80);
AsyncEventSource events("/events");
char jsonData = 0;

// Classe pour gérer le portail captif
class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    if(request->url() == "/ws")
      return false;
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html","text/html", false);
  }
};

// Fonction pour configurer le serveur Web
void setupServer(){
  
  // Chemin qui envoie le fichier index.html lorsque le serveur envoie une requête HTTP_GET
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html","text/html", false); 
      Serial.println("Client Connected");
  });
   
  // Chemin qui envoie le fichier JavaScript lorsque le serveur envoie une requête HTTP_GET
  server.on("/commands.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/commands.js", "text/js");
  });
   
  // Chemin qui envoie le fichier bootstrap.css lorsque le serveur envoie une requête HTTP_GET
  server.on("/bootstrap.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.css", "text/css");
  });
  
  // Chemin qui envoie une image(icon) lorsque le serveur envoie une requête HTTP_GET
  server.on("/manette.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/manette.png", "image/x-icon");
  });
}

void setup(){
  Serial.begin(115200);
   
  if(!SPIFFS.begin()){
    Serial.println("Une erreur est survenue lors du montage SPIFFS");
    return;
  }
  
  Serial.println("Configuration en mode point d'accès");
  WiFi.mode(WIFI_AP); 
  WiFi.softAP("Wifi_Manette");
  
  Serial.print("AP IP address: "); // Montre ce message avant l'IP dans le terminal
  Serial.println(WiFi.softAPIP()); // Montre l'adresse IP dans le terminal
  Serial.println("Setting up Async WebServer"); // Affiche le commencement du setup du serveur
  setupServer();
  
  //Serial.println("Démarrage du serveur DNS");
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); 

  initWebSocket();

  server.begin();
  //Serial.println("Site Web prêt à l'utilisation !");
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  Serial.println((char*)data);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        //Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void loop(){
  dnsServer.processNextRequest();
  ws.cleanupClients();
  if (Serial.available())  //si quelque chose est reçu sur le port série 1
  {
    //Lit le JSON et le met dans la variable jsonString
    String jsonString = Serial.readStringUntil('\n');
   //send text to a client
   ws.textAll(jsonString);
  }
}

 

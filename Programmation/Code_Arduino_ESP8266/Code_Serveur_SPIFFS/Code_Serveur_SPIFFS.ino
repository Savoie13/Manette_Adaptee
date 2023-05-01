/*
 * Ce code est destiné à être utilisé avec une carte ESP8266 pour créer un point d'accès Wi-Fi qui peut être utilisé pour contrôler un appareil 
 * à distance. Le code utilise plusieurs bibliothèques, notamment la bibliothèque ESPAsyncTCP pour la communication asynchrone avec TCP/IP, la 
 * bibliothèque ESPAsyncWebServer pour créer un serveur web asynchrone, la bibliothèque FS pour manipuler le système de fichiers de la carte 
 * ESP8266, la bibliothèque Wire pour communiquer avec des périphériques via I2C, la bibliothèque WebSocketsServer pour la communication asynchrone 
 * via WebSockets et la bibliothèque DNSServer pour utiliser le serveur DNS.
 * 
 * Le code utilise également une instance de DNSServer pour gérer les requêtes DNS, une instance de WebSocket pour gérer les connexions WebSocket, une 
 * instance de serveur web pour écouter les requêtes sur le port 80 et une instance d'EventSource pour envoyer des événements.
 * 
 * Le code utilise une classe pour gérer le portail captif et une fonction pour configurer le serveur web. Il utilise également une fonction pour envoyer 
 * les messages reçus en "WebSocket" vers la patte TX de l'ESP8266 afin de l'envoyer vers l'Arduino Pro Micro.
 * 
 * Enfin, le code vérifie l'état du WebSocket en fonction du type d'événement envoyé par le serveur en WebSocket, notamment WS_EVT_DATA pour envoyer 
 * les messages vers la patte TX et WS_EVT_DISCONNECTED pour fermer la connexion.
 */
 
#include <ESP8266WiFi.h> // Bibliothèque pour utiliser la fonctionnalité WiFi de la carte ESP8266
#include <ESPAsyncTCP.h> // Bibliothèque pour utiliser la communication asynchrone avec TCP/IP
#include <ESPAsyncWebServer.h> // Bibliothèque pour utiliser un serveur web asynchrone
#include "FS.h" // Bibliothèque pour manipuler le système de fichiers de la carte ESP8266
#include <Wire.h> // Bibliothèque pour communiquer avec des périphériques via I2C
#include <WebSocketsServer.h> // Bibliothèque pour utiliser la communication asynchrone via WebSockets
#include <DNSServer.h> // Bibliothèque pour utiliser le serveur DNS

const byte DNS_PORT = 53; // Numéro de port utilisé par le serveur DNS
DNSServer dnsServer; // Instance de DNSServer pour gérer les requêtes DNS
AsyncWebSocket ws("/ws"); // Instance de WebSocket pour gérer les connexions WebSocket
AsyncWebServer server(80); // Instance de serveur web pour écouter les requêtes sur le port 80
AsyncEventSource events("/events"); // Instance d'EventSource pour envoyer des événements

char jsonData = 0; // Variable pour stocker les données reçues depuis le port série

/*
 * Classe: CaptiveRequestHandler
 * Héritage: public AsyncWebHandler
 * Méthodes:
 * canHandle(AsyncWebServerRequest *request) : bool
 * handleRequest(AsyncWebServerRequest *request) : void
 * 
 * Description: Cette classe est utilisée pour gérer les requêtes d'un serveur web asynchrone. Elle hérite de la classe 
 * AsyncWebHandler et implémente les méthodes canHandle et handleRequest. La méthode canHandle vérifie si l'URL de la requête 
 * est "/ws", et la méthode handleRequest envoie le fichier "index.html" en réponse à la requête.
 */
class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    if(request->url() == "/ws") // Si l'URL de la requête est "/ws"
      return false;
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html","text/html", false); // Envoie le fichier "index.html" en réponse à la requête
  }
};

/*
 * Cette fonction "setupServer" configure un serveur web et définit les chemins qui envoient des fichiers en réponse à des requêtes HTTP_GET. Plus précisément :
 * 
 * Le chemin "/" envoie le fichier "index.html" en réponse à une requête HTTP_GET, et affiche "Client Connected" dans le terminal.
 * Le chemin "/commands.js" envoie le fichier "commands.js" en réponse à une requête HTTP_GET.
 * Le chemin "/bootstrap.css" envoie le fichier "bootstrap.css" en réponse à une requête HTTP_GET.
 * Le chemin "/manette.png" envoie une image au format "x-icon" en réponse à une requête HTTP_GET.
 */
void setupServer(){

  // Chemin qui envoie le fichier index.html lorsque le serveur envoie une requête HTTP_GET
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/index.html","text/html", false);
  Serial.println("Client Connected"); // Affiche dans le terminal que le client est connecté
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

/*
 * Cette méthode "setup" initialise les composants nécessaires pour la mise en place d'un point d'accès WiFi à l'aide d'un ESP8266. 
 * Elle configure le "baud rate" de la communication série à 115200, démarre le montage SPIFFS pour stocker les fichiers du site web, 
 * configure le point d'accès WiFi de l'ESP8266, affiche l'adresse IP dans le terminal, initialise le serveur web asynchrone, démarre 
 * le serveur DNS pour le portail captif, lance le portail captif, initialise la communication Websocket et démarre le serveur web.
 */
void setup(){
  Serial.begin(115200); // préréglage du "baud rate" à 115200
   
  if(!SPIFFS.begin()){ // si le montage SPIFFS à une erreur...
    Serial.println("Une erreur est survenue lors du montage SPIFFS");
    return;
  }
  
  Serial.println("Configuration en mode point d'accès");
  WiFi.mode(WIFI_AP); // met le WiFi de l'ESP8266 en point d'accès
  WiFi.softAP("Wifi_Manette"); // renomme le point d'accès pour "Wifi_Manette"
  
  Serial.print("AP IP address: "); // Montre ce message avant l'IP dans le terminal
  Serial.println(WiFi.softAPIP()); // Montre l'adresse IP dans le terminal
  Serial.println("Setting up Async WebServer"); // Affiche le commencement du setup du serveur
  setupServer();
  
  //Serial.println("Démarrage du serveur DNS");
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // démarre le serveur DNS pour le portail captif
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // envoie une requête pour lancé le portail captif

  initWebSocket();

  server.begin(); // démarre le serveur
  //Serial.println("Site Web prêt à l'utilisation !");
}

/*
 * La méthode "handleWebSocketMessage" reçoit un pointeur void "arg" et deux autres arguments "data" et "len" qui contiennent les données 
 * reçues dans un message WebSocket. Elle convertit le pointeur "arg" en une structure "AwsFrameInfo" pour vérifier le type d'argument reçu, 
 * puis elle affiche les données reçues sur le port série.
 */
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg; // vérifie le type d'argument reçu
  Serial.println((char*)data); // envoie vers le port série (la patte TX)
}

/* 
 * Cette méthode est un gestionnaire d'événements pour le serveur WebSocket. Elle est appelée chaque fois qu'un événement se produit 
 * sur le serveur, comme la connexion ou la déconnexion d'un client, la réception de données ou une erreur. Le code dans cette méthode 
 * effectue différentes actions en fonction du type d'événement qui se produit. En particulier, lorsque l'événement WS_EVT_DATA se 
 * produit, la méthode appelle la fonction handleWebSocketMessage() pour traiter les données reçues.
 */
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      // Ce code est exécuté lorsque le client se connecte au serveur WebSocket.
      //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      // Ce code est exécuté lorsque le client se déconnecte du serveur WebSocket.
      //Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      // Ce code est exécuté lorsque le serveur WebSocket reçoit des données d'un client.
      handleWebSocketMessage(arg, data, len); // Appelle la fonction handleWebSocketMessage() pour traiter les données.
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      // Ces événements ne sont pas gérés ici.
      break;
  }
}


/*
 * La fonction "initWebSocket()" initialise un WebSocket et ajoute un gestionnaire d'événements pour gérer les connexions entrantes. 
 * Elle utilise la variable "ws" pour stocker le WebSocket et appelle la méthode "onEvent" pour traiter les événements. Enfin, elle 
 * ajoute le WebSocket comme gestionnaire de requêtes sur le serveur web en utilisant la méthode "addHandler" de l'objet "server".
 */
void initWebSocket() {
  ws.onEvent(onEvent); // Événement lorsqu'un message est reçu ou qu'un client se connecte / déconnecte 
  server.addHandler(&ws); // Ajout du WebSocket en tant que gestionnaire de requêtes sur le serveur web
}

/*
 * La fonction "loop()" est une boucle d'exécution qui gère le serveur DNS, nettoie les clients inactifs et vérifie la disponibilité 
 * de nouvelles données sur le port série 1. Si de nouvelles données sont disponibles, elle lit les données JSON reçues depuis le port 
 * série et les envoie à tous les clients connectés via le WebSocket.
 */
void loop(){
  dnsServer.processNextRequest(); // Gestion du serveur DNS
  ws.cleanupClients(); // Nettoyage des clients inactifs
  if (Serial.available())  // Vérification de la disponibilité de nouvelles données sur le port série 1
  {
    // Lecture des données JSON reçues depuis le port série et stockage dans une variable
    String jsonString = Serial.readStringUntil('\n');
    // Envoi des données JSON à tous les clients connectés
    ws.textAll(jsonString);
  }
}


 

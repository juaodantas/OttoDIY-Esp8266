#include <ESP8266WiFi.h>

//const char* ssid = "Net-Virtua-6929";
//const char* password = "20369290";
const char* ssid = "Lab 95";
const char* password = "laboratorio95";
//const char* ssid = "Melow";
//const char* password = "12345678";


WiFiServer server(80);

#include <Servo.h>
#include <Oscillator.h>
#include <US.h>
#include <Otto.h>

Otto Otto;  //This is Otto!
//---------------------------------------------------------
//-- First step: Make sure the pins for servos are in the right position
/*
         ---------------
        |     O   O     |
        |---------------|
  YR D5==> |               | <== YL D0
         ---------------
            ||     ||
  RR D6==>   -----   ------  <== RL D7
         |-----   ------|
*/
#define PIN_YL D0 //servo[2]
#define PIN_YR D5 //servo[3]
#define PIN_RL D7 //servo[4]
#define PIN_RR D6 //servo[5]


void setup() {

  Serial.begin(9600);
  delay(10);

  // Conecta-se à rede WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Inicie o servidor
  server.begin();
  Serial.println("Server started");

  // Imprimir o endereço IP
  delay(3000);
  Serial.println(WiFi.localIP());

  Otto.init(PIN_YL,PIN_YR,PIN_RL,PIN_RR,true);
  Otto.home();
  delay(50);

  pinMode(LED_BUILTIN ,OUTPUT);
  
}

void loop() {
  
  // Verifique se um cliente está conectado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Espere até que o cliente envie alguns dados
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Leia a primeira linha do pedido
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Corresponder a solicitação
  if (req.indexOf("down") != -1) { 
    
          Serial.println("BACK");       
          Otto.walk(1, 1000, -1); 
           
  }
  if (req.indexOf("up") != -1) { 
    
         Serial.println("FWD");        
          Otto.walk(1, 1000, 1); //2 steps FORWARD
  }
   if (req.indexOf("left") != -1) {  
                                    
          Serial.println("LEFT");
          Otto.turn(1,1000,-1);
  
    
  }
  if (req.indexOf("right") != -1) {
    
        Serial.println("RIGHT");
        Otto.turn(1,1000,1);
  }
  if (req.indexOf("dance") != -1) {
    
        Serial.println("DANCE");
        Otto.moonwalker(1,1000,30,1); 
  }
    

// Prepare a resposta
  client.flush();

  
  String html = "<!DOCTYPE html>";  // string do html 
  html += "<html>";
  html += "<head>";
  html += " <meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">";
  html += "<meta http-equiv=\"Refresh\" content=\"5\">";
  html += "<link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/icon?family=Material+Icons\">";
  html += "<link rel=\"stylesheet\" href=\"https://code.getmdl.io/1.3.0/material.indigo-pink.min.css\">";
  html += "<link rel=\"stylesheet\" href=\"https://code.getmdl.io/1.3.0/material.blue_grey-indigo.min.css\"/>";
  html += "<script defer src=\"https://code.getmdl.io/1.3.0/material.min.js\"></script>";
  html += "<style>";
         
  html += "#wrapper{";
  html += "   height: 10em;";
  html += "   position: relative; ";
  html += "}";
  html += "#controls{";
  html += "   margin: 0;";
  html += "   position: absolute;";
  html += "   top: 50%;";
  html += "   left: 50%;";
  html += "   margin-right: -50%;";
  html += "   transform: translate(-50%, -50%)";
  html += "}";
  html += ".luminosidade{";
  html += "   font-family: Cambria, Cochin, Georgia, Times, 'Times New Roman', serif;";
  html += "   font-size: 25px;";
  html += "   margin: 0;";
  html += "   position: absolute;";
  html += "   top: 50%;";
  html += "   left: 50%;";
  html += "   margin-right: -50%;";
  html += "   transform: translate(-50%, -50%)";
  html+= "}";
  html += "</style>";
  html += "</head>";
  html += "<title> Control </title>";
  html += "<body>"; 
  html += "<div id=\"wrapper\">";
  html += "<div id=\"controls\">";
  html += "<a href=\"left\"<button id=\"esquerda\" class=\"mdl-button mdl-js-button mdl-button--fab mdl-js-ripple-effect mdl-button--colored\"> <i class=\"material-icons\"> arrow_back </i> </button></a>";
  html += "<a href=\"down\"<button id=\"baixo\"  class=\"mdl-button mdl-js-button mdl-button--fab mdl-js-ripple-effect mdl-button--colored\"> <i class=\"material-icons\">arrow_downward</i> </button></a>";
  html += "<a href=\"up\"<button id=\"cima\"  class=\"mdl-button mdl-js-button mdl-button--fab mdl-js-ripple-effect mdl-button--colored\"><i class=\"material-icons\">arrow_upward </i></button></a>";
  html += "<a href=\"right\"<button id=\"direita\" class=\"mdl-button mdl-js-button mdl-button--fab mdl-js-ripple-effect mdl-button--colored\"><i class=\"material-icons\">arrow_forward</i></button></a>";
  html += "</div>";
  html += "<div class=\"container\">";
  html += "<a href=\"right\"<button id=\"dance\" class=\"mdl-button mdl-js-button mdl-button--fab mdl-js-ripple-effect mdl-button--colored\"><i class=\"material-icons\">music_note</i></button></a>";
  html += "</div>";
  html += "</div>";
  html += "</body>"; 
  html += "</html>";

  
  client.print(html); // Envia a resposta para o cliente
  delay(1);
  Serial.println("Client disonnected");

    Serial.println(WiFi.localIP());
  delay(2000);      

  digitalWrite(LED_BUILTIN,HIGH); 
}

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "SuaRedeWiFi";
const char* password = "SuaSenhaWiFi";
const char* ap_ssid = "EstufaAuto";
const char* ap_password = "SuaSenhaAP"; // Senha para a rede Wi-Fi própria
const int wifiTimeout = 30; // Timeout em segundos
const int checkInterval = 10; // Intervalo de verificação em segundos

WebServer server(80);
bool connected = false;
unsigned long lastCheckTime = 0;

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello from ESP32!</h1>");
}

/*void handleCommand() {
  String message = "Received command: ";
  message += server.uri(); // Obter o caminho do URL solicitado
  Serial.println(message);
  server.send(200, "text/plain", "Command received");
}*/

void setup() {
  Serial.begin(115200);

  // Inicia o servidor web independentemente da conexão Wi-Fi
  server.on("/", handleRoot);
  //server.on("/command", handleCommand); // Definir rota para lidar com comandos
  server.begin();
  
  Serial.println("HTTP server started");

  // Iniciar a conexão Wi-Fi
  connectToWiFi();
}

void loop() {
  server.handleClient();

  // Verificar a conexão Wi-Fi periodicamente
  unsigned long currentTime = millis();
  if (currentTime - lastCheckTime >= checkInterval * 1000) {
    lastCheckTime = currentTime;
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Connection lost. Reconnecting...");
      //connectToWiFi();
    }
  }
}

void connectToWiFi() {
  // Tentar se conectar à rede Wi-Fi predefinida
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && ((millis() - startTime) < wifiTimeout * 1000)) {
    delay(1000);
    Serial.println("Connecting...");
  }

  if (WiFi.status() == WL_CONNECTED) {
    connected = true;
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    //Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect to WiFi. Starting Access Point...");
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Access Point created");
    Serial.print("IP Address: ");
    //Serial.println(WiFi.softAPIP()); // Obtenha o endereço IP do Access Point
  }
}

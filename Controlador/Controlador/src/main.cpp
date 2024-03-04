#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <String.h>
#include <ESPmDNS.h>

#define DEBUG_MODE

#ifdef DEBUG_MODE
  #define DEBUG(texto)    Serial.println(texto)
#else
  #define DEBUG(texto)    
#endif

#ifndef SILENT_MODE
  #define PRINTLN(texto)  Serial.println(texto)
  #define PRINT(texto)    Serial.print(texto)
#else
  #define PRINTLN(texto) 
  #define PRINT(texto)
#endif 

// Nome do Hostname
const char *hostname = "estufa-webserver";

// SSID e senha da rede Wi-Fi
const char *ssid = "LaptopDe";
const char *password = "21021992";

// SSID e senha do Wi-Fi a ser criado
const char *ssidAP = "ControladorEstufa";
const char *passwordAP = "EstufaAuto";

// Cria um objeto WebServer
WiFiServer server(80);

void WiFiReconnect(){
  PRINTLN("WiFi desconectado, tentando reconectar...");
  setupWifi();
}

void setupWifi() {
  bool conectado = false;
  PRINTLN("Configurando Wifi");
  // Set o hostname para identificação na rede
  WiFi.setHostname(hostname);

  // Tenta conectar na rede fornecida
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Verifica se conseguiu entrar
  DEBUG(String("Tentando conectar em ") + ssid);
  for (int i = 0; i < 10; i++) {
    //Se conectado termina a função
    if (WiFi.status() == WL_CONNECTED) {
      conectado = true;
      DEBUG("Conectado no wifi");
      PRINT("IP address: ");
      PRINTLN(WiFi.localIP());
      WiFi.onEvent(WiFiReconnect, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
      break;
    }
    //senão timeout
    delay(500);
    Serial.print(".");
  }
  
  // se não conectou, tenta criar uma AP
  if (conectado == false)
  {
    PRINTLN("Wifi não conectado, trocando para modo AP");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssidAP, passwordAP);
    Serial.println("IP address:");
    Serial.println(WiFi.softAPIP());
  }

  // Configura o nome de host mDNS
  if (!MDNS.begin(hostname)) {
    Serial.println("Erro ao configurar o mDNS");
    return;
  }
  else {
    Serial.println("mDNS configurado");
  }
}

void setup()
{
  Serial.begin(115200);

  // Configura o Wifi
  setupWifi();

  server.begin();
}

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
String header;
// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

void loop()
{
  WiFiClient client = server.available();
  if (client)
  { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0)
            {
              Serial.println("GPIO 26 on");
              output26State = "on";
              // digitalWrite(output26, HIGH);
            }
            else if (header.indexOf("GET /26/off") >= 0)
            {
              Serial.println("GPIO 26 off");
              output26State = "off";
              // digitalWrite(output26, LOW);
            }
            else if (header.indexOf("GET /27/on") >= 0)
            {
              Serial.println("GPIO 27 on");
              output27State = "on";
              // digitalWrite(output27, HIGH);
            }
            else if (header.indexOf("GET /27/off") >= 0)
            {
              Serial.println("GPIO 27 off");
              output27State = "off";
              // digitalWrite(output27, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button
            if (output26State == "off")
            {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button
            if (output27State == "off")
            {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

#include <String.h>
#include <WiFiServer.h>

#include "mywifi.h"
#include "interfaceWeb.h"
#include "configs.h"

namespace InterfaceWeb
{
    // Cria um objeto WebServer
    WiFiServer server(80);

    void setupServer()
    {
        server.begin();
    }

    ulong currentTime = millis();
    ulong previousTime = 0;
    const long timeout = 2000;
    String header;
    String output26State = "off";
    String output27State = "off";
    WiFiClient client;

    String obterStyle(const char *chave)
    {
        auto it = styles.find(chave);
        if (it != styles.end())
        {
            return it->second;
        }
        else
        {
            return "Key not found";
        }
    }

    void sendHead()
    {
        client.println("<html style=\"width:1280px;height:1024px;position:absolute;left:0px;top:0px;\">");
        client.println("<head>");
        client.println("<meta charset=\"UTF-8\">");

        client.println("<style>");
        client.println(obterStyle("body"));
        client.println(obterStyle("h1"));
        client.println(obterStyle(".text"));
        client.println(obterStyle(".space-before"));
        client.println(obterStyle(".form-field"));
        client.println(obterStyle(".menu"));
        client.println(obterStyle(".menu li"));
        client.println(obterStyle(".menu li a"));
        client.println(obterStyle(".menu li a:hover"));
        client.println(obterStyle(".submenu"));
        client.println(obterStyle(".submenu li"));
        client.println(obterStyle(".submenu li a"));
        client.println(obterStyle(".menu li:hover"));
        client.println(obterStyle(".status"));
        client.println(obterStyle(".status label"));
        client.println(obterStyle(".status span"));
        client.println(obterStyle(".substatus"));
        client.println(obterStyle(".controller-info"));
        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".subcontainer"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".tooltip"));
        client.println(obterStyle(".space-before"));
        client.println(obterStyle(".barra"));
        client.println(obterStyle(".dotgreen"));
        client.println(obterStyle(".dotred"));
        client.println(obterStyle(".controller-name"));
        client.println(obterStyle(".network-info"));
        client.println("</style>");

        client.println("<body>");
        client.println("<span><h1>Estufa Automatizada</h1></span>");
        client.println("<ul class=\"menu\">");
        client.println("<li><a href=\" homeHorta.html \">Home</a></li>");
        client.println("<li>");
        client.println("<a href=\" #\">Configurações</a>");
        client.println("<ul class=\"submenu\">");
        client.println("<li><a href=\"ConfigControlador.html\">Controlador</a></li>");
        client.println("<li><a href=\"configSensores.html\">Config. Sensores</a></li>");
        client.println("<li><a href=\"calibraSensor.html\">Calibração Sensores</a></li>");
        client.println("<li><a href=\"calibraAtuador.html\">Calibração Atuadores</a></li>");
        client.println("<li><a href=\"configLog.html\">Log</a></li>");
        client.println("</ul>");
        client.println("</li>");
        client.println("<li><a href=\"log.html\">Log</a></li>");
        client.println("<li><a href=\"#\">Reiniciar</a></li>");
        client.println("<li><a href=\"ajuda.html\">Ajuda</a></li>");
        client.println("</ul>");

        client.println("<div class=\"space-before\"></div>");

        client.println("<div class=\"barra\">");
        client.println("<div class=\"dotgreen\"></div>");
        client.println("<div class=\"text\">Conexão</div>");
        client.println("<div class=\"dotred\"></div>");
        client.println("<div class=\"text\">Irmão</div>");
        client.println("</div>");
        client.println("<div class=\"space-before\"></div>");

        client.println("<div class=\"controller-info\">");
        client.println("<div class=\"controller-name\">");
        client.println(configs::getNomeControlador());
        client.println("</div>");
        client.println("<div class=\"network-info\">");
        client.print("Rede conectada: ");
        client.println(mywifi::getSSID());
        client.println("<br>");
        client.print("IP: ");
        client.println(mywifi::getIP().c_str());
        client.println("</div>");
        client.println("</div>");
        client.println("<br>");

        client.println("<div class=\"status\">");
        client.println("<label for=\"estado\" style=\"font-weight: bold;\">Colheita:</label>");
        client.print("<span id=\"estado\">");
        client.print(configs::getColheita());
        client.println("</span>");
        client.println("</div>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<br>");

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h2 class=\"container-header-text\">Sensores</h2>");
        client.println("</div>");
        client.println("<div class=\"status\">");
        client.println("<label for=\"sensor\">Sensor</label>");
        client.println("<span id=\"sensor\">NOME</span>");
        client.println("<div class=\"substatus\">");
        client.println("<div class=\"status\">");
        client.println("<label for=\"tipo\">Tipo:</label>");
        client.println("<span id=\"tipo\">TIPO</span>");
        client.println("</div>");
        client.println("<div class=\"status\">");
        client.println("<label for=\"valor\">Valor:</label>");
        client.println("<span id=\"valor\">Valor</span>");
        client.println("</div>");
        client.println("<div class=\"status\">");
        client.println("<label for=\"estado\">Estado Atual:</label>");
        client.println("<span id=\"estado\">ESTADO_ATUAL</span>");
        client.println("</div>");
        client.println("</div>");
        client.println("</div>");
        client.println("</div>");

        client.println("</body>");

        client.println("</html>");
    }

    void sendHome()
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
    }

    void loop()
    {
        // Pega um client que esteja disponivel para atender
        client = server.available();
        // se for um client válido começa o tratamento
        if (client)
        {
            currentTime = millis();
            previousTime = currentTime;
            String currentLine = "";
            while (client.connected() && currentTime - previousTime <= timeout)
            {
                currentTime = millis();
                if (client.available())
                {
                    char c = client.read();
                    header += c;
                    if (c == '\n')
                    {
                        if (currentLine.length() == 0)
                        {
                            // Send Home
                            sendHead();
                            break;
                        }
                        else
                        {
                            currentLine = "";
                        }
                    }
                    else if (c != '\r')
                    {
                        currentLine += c;
                    }
                }
            }
            header = "";
            client.stop();
        }
    }
}

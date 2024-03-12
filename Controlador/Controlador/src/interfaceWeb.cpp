#include <String.h>
#include <WiFiServer.h>

#include "mywifi.h"
#include "interfaceWeb.h"
#include "configs.h"
#include "common.h"

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

    using Dictionary = std::unordered_map<const char *, const char *>;

    const Dictionary styles = {
        /*estilo body*/
        {"body", "body {\nbackground-color: #222;\nfont-family: sans-serif;\ncolor: white;}"},
        /*estilo h1*/
        {"h1", "h1 {\ncolor: white;\nfont-weight: bold;\ntext-decoration: underline;\ntext-align: center;\npadding-top: 15px;}"},
        /*estilo .text*/
        {".text", ".text {font-size: 14;\npadding-right: 15px;\n}"},
        /*estilo space-before*/
        {".space-before", ".space-before {\nmargin-bottom: 20px;}"},
        /* Estilos de forms */
        {".form-field", ".form-field {\nmargin: 0px 0px 0px 10px;\ndisplay: inline-block;\nvertical-align: center;}"},
        {".form-label-medio", ".form-label-medio{display: inline-block;padding: 10px 22px 10px 10px;width: 50px;}"},
        {".form-label-curto", ".form-label-curto{display: inline-block;padding: 10px 14px 10px 14px;width: 30px;}"},

        /* Estilos do menu */
        /* Estilo para a barra de menu */
        {".menu", ".menu {\nlist-style-type: none;\nmargin: 0;\npadding: 0;\nbackground-color: #333;\noverflow: hidden;}"},
        /* Estilo para os itens do menu */
        {".menu li", ".menu li {\nfloat: left;}"},
        /* Estilo para os links do menu */
        {".menu li a", ".menu li a {\ndisplay: block;\ncolor: white;\ntext-align: center;\npadding: 14px 16px;\ntext-decoration: none;}"},
        /* Estilo para os links do menu quando passar o mouse sobre eles */
        {".menu li a:hover", ".menu li a:hover {background-color: #111;}"},
        /* Estilo para o submenu */
        {".submenu", ".submenu {display: none; position: absolute;background-color: #444;z-index: 1;}"},
        /* Estilo para os itens do submenu */
        {".submenu li", ".submenu li {float: none;}"},
        /* Estilo para os links do submenu */
        {".submenu li a", ".submenu li a {padding: 10px 16px;}"},
        /* Mostrar o submenu ao passar o mouse sobre o item de menu "Configurações" */
        {".menu li:hover", ".menu li:hover .submenu {display: block;}"},
        /* Estilo para os campos de status */
        {".status", ".status {margin: 10px;}"},
        {".status label", ".status label {display: inline-block;width: 100px;}"},
        {".status span", ".status span {display: inline-block;background-color: #333;color: #FFF;padding: 5px 10px;border-radius: 5px;}"},
        {".substatus", ".substatus {margin-left: 20px;}"},

        /* Estilo para a div do controlador */
        {".controller-info", ".controller-info {\nmargin: 10px;\nbackground-color: #333;\npadding: 10px;\nborder-radius: 10px; }"},
        {".controller-name", ".controller-name {font-weight: bold;}"},
        {".network-info", ".network-info {\nmargin-top: 5px; margin-left: 20px; line-height: 1.5;}"},
        /* Estilo para a div que envolve os status dos sensores */
        {".container", ".container {\nborder: 2px solid #FFF;\nborder-radius: 10px;\npadding: 10px;\nposition: relative; }"},
        {".container-header-text", ".container-header-text {margin: 0;}"},
        {".subcontainer", ".subcontainer {border: 1px solid gray;border-radius: 10px;padding: 10px;position: relative;}"},
        /* Estilo para o cabeçalho "Sensores" */
        {".container-header", ".container-header {\nposition: absolute;\ntop: -13px;\nleft: 10px;\nbackground-color: #222;\npadding: 0 10px;\nfont-size: 12px;}"},
        /*Estilo para dar um espaçamento vertical*/
        {".space-before", ".space-before {margin-bottom: 20px;}"},
        /*Estilo de fundo barra de status das conexões*/
        {".barra", ".barra {display: flex;align-items: center; padding: 10px;}"},
        /*Estilo para fazer o ponto verde */
        {".dotgreen", ".dotgreen {width: 25px;height: 25px;border-radius: 50%;background-color: green;margin-right: 10px;}"},
        /*Estilo para fazer o ponto vermelho*/
        {".dotred", ".dotred {width: 25px;height: 25px;border-radius: 50%;background-color: red;margin-right: 10px;}"},

        //{,},
        /* Estilo para o tooltip */
        {".tooltip", ".tooltip {\nposition: relative;\ndisplay: inline-block;}"}};

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

    void sendStyleIDControlador()
    {
        client.println(obterStyle(".controller-info"));
        client.println(obterStyle(".controller-name"));
        client.println(obterStyle(".network-info"));
    }
    void sendIDControlador()
    {
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
    }
    void sendEndPage()
    {
        client.println("</body>");
        client.println("</html>");
        client.println();
    }
    void sendTitleAndMenu()
    {
        client.println("<span><h1>Estufa Automatizada</h1></span>");
        client.println("<ul class=\"menu\">");
        client.println("<li><a href=\"/\">Home</a></li>");
        client.println("<li>");
        client.println("<a href=\"/config/controlador\">Configurações</a>");
        client.println("<ul class=\"submenu\">");
        client.println("<li><a href=\"/config/controlador\">Controlador</a></li>");
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
    }
    void sendStyleTitleAndMenu()
    {
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
        client.println(obterStyle(".barra"));
        client.println(obterStyle(".dotgreen"));
        client.println(obterStyle(".dotred"));
    }
    void sendHome()
    {
        client.println("<html style=\"width:1280px;height:1024px;position:absolute;left:0px;top:0px;\">");
        client.println("<head>");
        client.println("<meta charset=\"UTF-8\">");

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();

        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".subcontainer"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".tooltip"));

        client.println("</style>");

        client.println("<body>");
        sendTitleAndMenu();

        sendIDControlador();

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

        sendEndPage();
    }

    void sendConfigControlador()
    {
        client.println("<html style=\"width:1280px;height:1024px;position:absolute;left:0px;top:0px;\">");
        client.println("<head>");
        client.println("<meta charset=\"UTF-8\">");

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();
        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".form-field"));
        client.println(obterStyle(".form-label-curto"));
        client.println(obterStyle(".form-label-medio"));
        client.println("</style>");

        client.println("<body>");

        sendTitleAndMenu();

        sendIDControlador();

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h2 class=\"container-header-text\">Controlador</h2>");
        client.println("</div>");
        client.println("<form id=\"DadosControlador\">");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"nome\" class=\"form-label-medio\">Nome:</label>");
        client.println("<input type=\"text\" id=\"nome\" name=\"nome\" placeholder=\"VALOR_NOME\" class=\"form-field\">");
        client.println("<br>");
        client.println("<label for=\"nomeColheita\" class=\"form-label-medio\">Colheita:</label>");
        client.println("<input type=\"text\" id=\"nomeColheita\" name=\"nomeColheita\" placeholder=\"NOME_COLHEITA\" class=\"form-field\">");
        client.println("<br>");
        client.println("<label for=\"hora\" class=\"form-label-medio\">Hora:</label>");
        client.println("<input type=\"time\" id=\"hora\" name=\"hora\" class=\"form-field\">");
        client.println("<br>");
        client.println("<label for=\"data\" class=\"form-label-medio\">Data:</label>");
        client.println("<input type=\"date\" id=\"data\" name=\"data\" class=\"form-field\">");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<label for=\"sync\" class=\"form-label-x-longo\">Sincronizar com servidor:</label>");
        client.println("<br>");
        client.println("<input type=\"text\" id=\"sync\" name=\"sync\" class=\"form-field\">");
        client.println("<button class=\"button\" type=\"button\" onclick=\"enviarDados()\">Sincronizar</button>");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<button class=\"button\" type=\"button\" onclick=\"enviarDados()\">Salvar</button>");
        client.println("</div>");
        client.println("</form>");
        client.println("</div>");
        client.println("<div class=\"space-before\"></div>");

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h2 class=\"container-header-text\">Rede Externa</h2>");
        client.println("</div>");
        client.println("<form id=\"DadosRedeExterna\">");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"ssid\" class=\"form-label-curto\">SSID:</label>");
        client.println("<input type=\"text\" id=\"ssid\" name=\"ssid\" placeholder=\"VALOR_SSID\" class=\"form-field\">");
        client.println("<br>");
        client.println("<label for=\"senha\" class=\"form-label-curto\">Senha:</label>");
        client.println("<input type=\"text\" id=\"senha\" name=\"ssid\" placeholder=\"VALOR_SENHA\" class=\"form-field\">");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<button class=\"button\" type=\"button\" onclick=\"enviarDados()\">Salvar</button>");
        client.println("</div>");
        client.println("</form>");
        client.println("</div>");
        client.println("<div class=\"space-before\"></div>");

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h2 class=\"container-header-text\">Rede Interna</h2>");
        client.println("</div>");
        client.println("<form id=\"DadosRedeInterna\">");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"ssid_esp\" class=\"form-label-curto\">SSID:</label>");
        client.println("<input type=\"text\" id=\"ssid_esp\" name=\"ssid_esp\" placeholder=\"VALOR_SSID_ESP\" class=\"form-field\">");
        client.println("<br>");
        client.println("<label for=\"senha_esp\" class=\"form-label-curto\">Senha:</label>");
        client.println("<input type=\"text\" id=\"senha_esp\" name=\"ssid_esp\" placeholder=\"VALOR_SENHA_ESP\" class=\"form-field\">");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<button class=\"button\" type=\"button\" onclick=\"enviarDados()\">Salvar</button>");
        client.println("</div>");
        client.println("</form>");
        client.println("</div>");

        client.println("<div class=\"space-before\"></div>");
        
        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h2 class=\"container-header-text\">Sensores e Atuadores</h2>");
        client.println("</div>");
        client.println("<form id=\"DadosSensores\">");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"qtdSense\" class=\"form-label-x-longo\">Quantidade sensores:</label>");
        client.println("<input type=\"text\" id=\"qtdSense\" name=\"qtdSense\" placeholder=\"QTD_SENSORES\" class=\"form-field\">");
        client.println("<br>");
        client.println("<label for=\"portaSense\" class=\"form-label-x-longo\">Porta inicial:</label>");
        client.println("<input type=\"text\" id=\"portaSense\" name=\"portaSense\" placeholder=\"PORTA_SENSORES\" class=\"form-field\">");
        client.println("<br>");
        client.println("<br>");
        client.println("<label for=\"qtdAtua\" class=\"form-label-x-longo\">Quantidade atuadores:</label>");
        client.println("<input type=\"text\" id=\"qtdAtua\" name=\"qtdAtua\" placeholder=\"QTD_ATUADORES\" class=\"form-field\">");
        client.println("<br>");
        client.println("<label for=\"portaAtua\" class=\"form-label-x-longo\">Porta inicial:</label>");
        client.println("<input type=\"text\" id=\"portaAtua\" name=\"portaAtua\" placeholder=\"PORTA_ATUADORES\" class=\"form-field\">");
        client.println("<br>");
        client.println("<br>");
        client.println("<label for=\"amostragem\" class=\"form-label-x-longo\">Tempo de amostragem:</label>");
        client.println("<input type=\"text\" id=\"amostragem\" name=\"amostragem\" placeholder=\"AMOSTRAGEM\" class=\"form-field\">");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<button class=\"button\" type=\"button\" onclick=\"enviarDados()\">Salvar</button>");
        client.println("</div>");
        client.println("</form>");
        client.println("</div>");

        client.println("<div class=\"space-before\"></div>");

        sendEndPage();
    }

    void sendHeader()
    {
        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
        // and a content-type so the client knows what's coming, then a blank line:
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection: close");
        client.println();
        /*
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
                // Break out of the while loop*/
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
                    // DEBUG(header);
                    if (c == '\n')
                    {
                        DEBUG(currentLine.c_str());
                        if (currentLine.length() == 0)
                        {
                            // Send Home
                            if (header.indexOf("GET /config/controlador") >= 0)
                            {
                                sendHeader();
                                sendConfigControlador();
                            }
                            else
                            {
                                sendHeader();
                                sendHome();
                            }

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

#include <Arduino.h>
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
    ulong currentTime = millis();
    ulong previousTime = 0;
    const long timeout = 2000;
    String header;
    String postBody;
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
        {".form-label-longo", ".form-label-longo{display: inline-block;padding: 10px 10px 10px 10px;width: 125px;}"},
        {".form-label-x-longo", ".form-label-x-longo{display: inline-block;padding: 10px 10px 10px 10px;width: 200px;}"},
        {".form-label-xx-longo", ".form-label-xx-longo{display: inline-block;padding: 10px 10px 10px 10px;width: 250px;}"},
        {".tooltip", ".tooltip {position: relative;display: inline-block;}"},
        {".question-mark", ".question-mark {border: 1px solid #ccc;border-radius: 50\%;padding: 5px;font-size: 16px;font-weight: bold;}"},
        {".tooltiptext", ".tooltip .tooltiptext {visibility: hidden;width: 120px;background-color: #555;color: #fff;text-align: center;border-radius: 6px;padding: 5px;position: absolute;z-index: 1;top: 130%;left: 50%;transform: translateX(-50%);}"},
        {".button", ".button{padding: 10px 20px;background-color: #333;border: none;color: white;cursor: pointer;border-radius: 5px;}"},
        {".form-dropdown", ".form-dropdown {margin-left: 10px;padding: 5px;}"},
        {".tooltip:hover", ".tooltip:hover .question-mark {background-color: #ccc;}"},
        {".tooltiptext:hover", ".tooltip:hover .tooltiptext {visibility: visible;}"},

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
        {".subcontainer-header", ".subcontainer-header {position: absolute;top: -13px;left: 10px;background-color: #222;padding: 0 10px;font-size: 12px;font-weight: normal;}"},
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
    };

    void setupServer()
    {
        server.begin();
    }

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
        client.println(configs::config.getNomeControlador());
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
        client.println("<li><a href=\"/config/sensores\">Config. Sensores</a></li>");
        client.println("<li><a href=\"/config/calibraSensores\">Calibração Sensores</a></li>");
        client.println("<li><a href=\"/config/calibraAtuadores\">Calibração Atuadores</a></li>");
        client.println("<li><a href=\"/config/Log\">Log</a></li>");
        client.println("</ul>");
        client.println("</li>");
        client.println("<li><a href=\"/log\">Log</a></li>");
        client.println("<li><a href=\"/reset\">Reiniciar</a></li>");
        client.println("<li><a href=\"/help\">Ajuda</a></li>");
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

    void sendHeaderHTML()
    {
        client.println("<!DOCTYPE html>");
        client.println("<html lang=\"pt-BR\">");
        client.println("<head>");
        client.println("<meta charset=\"UTF-8\">");
        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    }

    void sendHome()
    {
        sendHeaderHTML();

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
        client.print(configs::config.getColheita());
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

    void sendScriptConfigControlador()
    {
        client.println("<script>");
        client.println("function enviarDadosRedeExterna() {");
        client.println("\tvar ssid = document.getElementById(\"ssid\").value;");
        client.println("\tvar senha = document.getElementById(\"senha\").value;");

        client.println("\tif (ssid.length < 1 || ssid.length > 31) {");
        client.println("\t\talert(\"O SSID deve ter entre 1 e 31 caracteres.\");");
        client.println("\treturn;");
        client.println("\t}");
        client.println("\tif (senha.length < 8 || senha.length > 63) {");
        client.println("\t\talert(\"A senha deve ter entre 8 e 63 caracteres.\");");
        client.println("\treturn;");
        client.println("\t}");

        client.println("\tvar xhr = new XMLHttpRequest();");
        client.println("\txhr.open(\"POST\", \"/enviar-dados\", true);");
        client.println("\txhr.setRequestHeader(\"Content-Type\", \"application/x-www-form-urlencoded\");");
        client.println("\tvar data = \"RedeExterna\\nssid=\" + encodeURIComponent(ssid) + \"\\nsenha=\" + encodeURIComponent(senha) + \"\\n\";");
        client.println("\txhr.send(data);");
        client.println("\talert(\"Dados enviados com sucesso!\");");
        client.println("}");
        client.println("</script>");
    }

    void sendScriptConfigLog()
    {
        client.println("<script>");
        client.println("function toggleTooltip(elem) {");
        client.println("var tooltip = elem.nextElementSibling;");
        client.println("tooltip.style.visibility = tooltip.style.visibility === \"visible\" ? \"hidden\" : \"visible\";");
        client.println("}");
        client.println("function toggleSensorTodos(elem) {");
        client.println("// Verificar se todos os outros sensores estão marcados");
        client.println("var allSensorsChecked = document.querySelectorAll('input[name^=\"sensor\"]:not(#sensorTodos):checked').length === 3;");

        client.println("// Se todos os outros sensores estiverem marcados, marcar o checkbox \"Todos\"; caso contrário, desmarcá-lo");
        client.println("document.getElementById(\"sensorTodos\").checked = allSensorsChecked;");
        client.println("}");

        client.println("function toggleSelSensorTodos(elem) {");
        client.println("// Marcar/desmarcar todos os outros sensores de acordo com o estado do checkbox \"Todos\"");
        client.println("var checkboxes = document.querySelectorAll('input[name^=\"sensor\"]:not(#sensorTodos)');");
        client.println("checkboxes.forEach(function (checkbox) {");
        client.println("checkbox.checked = elem.checked;");
        client.println("});");
        client.println("}");

        client.println("function toggleAtuadorTodos(elem) {");
        client.println("// Verificar se todos os outros sensores estão marcados");
        client.println("var allAtuadorChecked = document.querySelectorAll('input[name^=\"atuador\"]:not(#atuadorTodos):checked').length === 3;");

        client.println("// Se todos os outros sensores estiverem marcados, marcar o checkbox \"Todos\"; caso contrário, desmarcá-lo");
        client.println("document.getElementById(\"atuadorTodos\").checked = allAtuadorChecked;");
        client.println("}");

        client.println("function toggleSelAtuadorTodos(elem) {");
        client.println("// Marcar/desmarcar todos os outros sensores de acordo com o estado do checkbox \"Todos\"");
        client.println("var checkboxes = document.querySelectorAll('input[name^=\"atuador\"]:not(#atuadorTodos)');");
        client.println("checkboxes.forEach(function (checkbox) {");
        client.println("checkbox.checked = elem.checked;");
        client.println("});");
        client.println("}");
        client.println("</script>");
    }

    void sendConfigControlador()
    {
        sendHeaderHTML();

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();
        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".form-field"));
        client.println(obterStyle(".form-label-curto"));
        client.println(obterStyle(".form-label-medio"));
        client.println(obterStyle(".form-label-x-longo"));
        client.println(obterStyle(".button"));
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
        client.println(String("<input type=\"text\" id=\"nome\" name=\"nome\" placeholder=\"") + configs::config.getNomeControlador() + String("\" class=\"form-field\">"));
        client.println("<br>");
        client.println("<label for=\"nomeColheita\" class=\"form-label-medio\">Colheita:</label>");
        client.println(String("<input type=\"text\" id=\"nomeColheita\" name=\"nomeColheita\" placeholder=\"") + configs::config.getColheita() + String("\" class=\"form-field\">"));
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
        client.println(String("<input type=\"text\" id=\"ssid\" name=\"ssid\" placeholder=\"") + configs::config.getSSID() + String("\" class=\"form-field\">"));
        client.println("<br>");
        client.println("<label for=\"senha\" class=\"form-label-curto\">Senha:</label>");
        client.println(String("<input type=\"password\" id=\"senha\" name=\"senha\" placeholder=\"") + configs::config.getPWD() + String("\" class=\"form-field\">"));
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<button class=\"button\" type=\"button\" onclick=\"enviarDadosRedeExterna()\">Salvar</button>");
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
        sendScriptConfigControlador();
        sendEndPage();
    }

    void sendCalibraSensores()
    {
        sendHeaderHTML();

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();
        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".subcontainer"));
        client.println(obterStyle(".subcontainer-header"));
        client.println(obterStyle(".form-field"));
        client.println(obterStyle(".form-label-curto"));
        client.println(obterStyle(".form-label-medio"));
        client.println(obterStyle(".form-label-longo"));
        client.println(obterStyle(".button"));
        client.println(obterStyle(".form-dropdown"));
        client.println("</style>");

        client.println("<body>");

        sendTitleAndMenu();

        sendIDControlador();

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h3 class=\"container-header-text\">Sensor X</h3>");
        client.println("</div>");
        client.println("<form id=\"formularioSensor\">");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"nomeSensor\" class=\"form-label-longo\">Nome do Sensor:</label>");
        client.println("<input type=\"text\" id=\"nomeSensor\" name=\"nomeSensor\" placeholder=\"NOME_SENSOR\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"tipoSensor\" class=\"form-label-longo\">Tipo do Sensor:</label>");
        client.println("<select id=\"tipoSensor\" name=\"tipoSensor\" class=\"form-dropdown\">");
        client.println("<option value=\"Temperatura\">Temperatura</option>");
        client.println("<option value=\"Umidade\">Umidade</option>");
        client.println("<option value=\"Iluminação\">Iluminação</option>");
        client.println("<option value=\"Outros\">Outros</option>");
        client.println("</select>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label class=\"form-label-configSensor\">Valor Atual:</label>");
        client.println("<label id=\"valorAtual\" class=\"form-label-longo\">N/A</label>");
        client.println("<button type=\"button\" class=\"button\" style=\"margin-left: 37px;\">Atualizar</button>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorForce\" class=\"form-label-longo\">Forçar leitura:</label>");
        client.println("<input type=\"text\" id=\"valorForce\" name=\"valorForce\" placeholder=\"VALOR_FORCADO\" class=\"form-field\">");
        client.println("<button type=\"button\" class=\"button\">Forçar</button>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<div class=\"subcontainer\">");
        client.println("<div class=\"subcontainer-header\">");
        client.println("<div class=\"container-header-text\">Valor Acionamento:</div>");
        client.println("</div>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorAcionamentoSubida\" class=\"form-label-medio\">Subida:</label>");
        client.println("<input type=\"text\" id=\"valorAcionamentoSubida\" name=\"valorAcionamentoSubida\" placeholder=\"Valor de Acionamento Subida\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorAcionamentoDescida\" class=\"form-label-medio\">Descida:</label>");
        client.println("<input type=\"text\" id=\"valorAcionamentoDescida\" name=\"valorAcionamentoDescida\" placeholder=\"Valor de Acionamento Descida\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"atuador\" class=\"form-label-medio\">Aciona:</label>");
        client.println("<select id=\"tipoSensor\" name=\"atuador\" class=\"form-dropdown\">");
        client.println("<option value=\"Atuador 1\">Atuador 1</option>");
        client.println("<option value=\"Atuador 2\">Atuador 2</option>");
        client.println("<option value=\"Atuador 3\">Atuador 3</option>");
        client.println("<option value=\"Atuador 4\">Atuador 4</option>");
        client.println("</select>");
        client.println("<br>");
        client.println("<input type=\"checkbox\" name=\"reverso\" id=\"reverso\" class=\"form-label-medio\" value=\"sim\">");
        client.println("<label for=\"reverso\" class=\"form-label-longo\">Ação reversa</label>");
        client.println("</div>");
        client.println("</div>");
        client.println("<br>");

        client.println("<button type=\"submit\" class=\"button\">Salvar</button>");
        client.println("</form>");
        client.println("</div>");
        sendEndPage();
    }

    void sendCalibraAtuadores()
    {
        sendHeaderHTML();

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();
        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".subcontainer"));
        client.println(obterStyle(".subcontainer-header"));
        client.println(obterStyle(".form-field"));
        client.println(obterStyle(".form-label-curto"));
        client.println(obterStyle(".form-label-longo"));
        client.println(obterStyle(".form-label-x-longo"));
        client.println(obterStyle(".form-label-xx-longo"));
        client.println(obterStyle(".tooltip"));
        client.println(obterStyle(".question-mark"));
        client.println(obterStyle(".tooltiptext"));
        client.println(obterStyle(".button"));
        client.println(obterStyle(".form-dropdown"));
        client.println("</style>");

        client.println("<body>");

        sendTitleAndMenu();

        sendIDControlador();

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h3 class=\"container-header-text\">Atuador X</h3>");
        client.println("</div>");
        client.println("<form id=\"formularioSensor\">");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"nomeAtuador\" class=\"form-label-longo\">Nome do Sensor:</label>");
        client.println("<input type=\"text\" id=\"nomeAtuador\" name=\"nomeAtuador\" placeholder=\"NOME_ATUADOR\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"tipoAtuador\" class=\"form-label-longo\">Tipo do Sensor:</label>");
        client.println("<select id=\"tipoAtuador\" name=\"tipoAtuador\" class=\"form-dropdown\">");
        client.println("<option value=\"Linear\">Linear</option>");
        client.println("<option value=\"OnOff\">On/Off</option>");
        client.println("</select>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label class=\"form-label-longo\">Estado Atual:</label>");
        client.println("<label id=\"estadoAtual\" class=\"form-label-longo\">N/A</label>");
        client.println("<button type=\"button\" class=\"button\" style=\"margin-left: 42px;\">Atualizar</button>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorForce\" class=\"form-label-longo\">Forçar estado:</label>");
        client.println("<input type=\"text\" id=\"valorForce\" name=\"valorForce\" placeholder=\"VALOR_FORCADO\" class=\"form-field\">");
        client.println("<button type=\"button\" class=\"button\">Forçar</button>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"atuadorMin\" class=\"form-label-longo\">Mínimo atuador:</label>");
        client.println("<input type=\"text\" id=\"atuadorMin\" name=\"atuadorMin\" placeholder=\"MIN_ATUADOR\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"atuadorMin\" class=\"form-label-longo\">Mínimo atuador:</label>");
        client.println("<input type=\"text\" id=\"atuadorMin\" name=\"atuadorMin\" placeholder=\"MIN_ATUADOR\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<input type=\"checkbox\" id=\"chkTimeout\" class=\"form-label-medio\" onchange=\"toggleTextBox()\">");
        client.println("<label for=\"chkTimeout\">Controle de Timeout</label>");
        client.println("<input type=\"text\" id=\"txtTimeout\" class=\"form-field\" placeholder=\"Insira o valor de timeout\" disabled>");
        client.println("<br>");
        client.println("<br>");
        client.println("</form>");
        client.println("</div>");
        sendEndPage();
    }

    void sendConfigLog()
    {
        sendHeaderHTML();

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();
        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".subcontainer"));
        client.println(obterStyle(".subcontainer-header"));
        client.println(obterStyle(".form-field"));
        client.println(obterStyle(".form-label-curto"));
        client.println(obterStyle(".form-label-medio"));
        client.println(obterStyle(".form-label-longo"));
        client.println(obterStyle(".form-label-x-longo"));
        client.println(obterStyle(".form-label-xx-longo"));
        client.println(obterStyle(".button"));
        client.println(obterStyle(".form-dropdown"));
        client.println(obterStyle(".tooltip"));
        client.println(obterStyle(".question-mark"));
        client.println(obterStyle(".tooltiptext"));
        client.println(obterStyle(".tooltip:hover"));
        client.println(obterStyle(".tooltiptext:hover"));
        client.println("</style>");

        client.println("<body>");

        sendTitleAndMenu();

        sendIDControlador();

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h2 class=\"container-header-text\">Log</h2>");
        client.println("</div>");
        client.println("<div class=\"form-field\">");
        client.println("<label class=\"form-label-xx-longo\">Tempo de intervalo entre envios:</label>");
        client.println("<input type=\"text\" id=\"tempoIntervalo\" class=\"form-field\" placeholder=\"Insira o tempo em minutos\">");
        client.println("<span class=\"tooltip\">");
        client.println("<span class=\"question-mark\" onclick=\"toggleTooltip(this)\">?</span>");
        client.println("<span class=\"tooltiptext\">Insira o tempo em minutos</span>");
        client.println("</span>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label class=\"form-label-x-longo\">Armazenamento Local:</label>");
        client.println("<input type=\"checkbox\" id=\"armazenamentoLocal\">");
        client.println("<label for=\"armazenamentoLocal\">Ativado</label>");
        client.println("<span class=\"tooltip\">");
        client.println("<span class=\"question-mark\" onclick=\"toggleTooltip(this)\">?</span>");
        client.println("<span class=\"tooltiptext\">Salvar uma cópia no SD local?</span>");
        client.println("</span>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label class=\"form-label-longo\">Nível de Detalhe:</label>");
        client.println("<input type=\"text\" class=\"form-field\" id=\"nivelDetalhe\" placeholder=\"Insira o nível em %\">");
        client.println("<span class=\"tooltip\">");
        client.println("<span class=\"question-mark\" onclick=\"toggleTooltip(this)\">?</span>");
        client.println("<span class=\"tooltiptext\">Se acima de 0%, só serão armazenadas variações maiores.</span>");
        client.println("</span>");
        client.println("</div>");

        client.println("<div class=\"barra\">");
        client.println("<div style=\"margin-right: 5px;\">");
        client.println("<label class=\"form-label-xx-longo\" style=\"font-weight: bold;\">Seleção de Sensores:</label>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"sensorTodos\" name=\"sensorTodos\" onclick=\"toggleSelSensorTodos(this)\">");
        client.println("<label for=\"sensorTodos\">Todos</label>");
        client.println("</div>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"sensor1\" name=\"sensor1\" onclick=\"toggleSensorTodos(this)\">");
        client.println("<label for=\"sensor1\">Sensor 1</label>");
        client.println("</div>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"sensor2\" name=\"sensor2\" onclick=\"toggleSensorTodos(this)\">");
        client.println("<label for=\"sensor2\">Sensor 2</label>");
        client.println("</div>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"sensor3\" name=\"sensor3\" onclick=\"toggleSensorTodos(this)\">");
        client.println("<label for=\"sensor3\">Sensor 3</label>");
        client.println("</div>");
        client.println("</div>");

        client.println("<div style=\"margin-left: 5px;\">");
        client.println("<label class=\"form-label-xx-longo\" style=\"font-weight: bold\">Seleção de Atuadores:</label>");
        client.println("<span class=\"tooltip\">");
        client.println("<span class=\"question-mark\" onclick=\"toggleTooltip(this)\">?</span>");
        client.println("<span class=\"tooltiptext\">Selecionar sensores e atuadores que serão aquisitados.</span>");
        client.println("</span>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"atuadorTodos\" name=\"atuadorTodos\" onclick=\"toggleSelAtuadorTodos(this)\">");
        client.println("<label for=\"atuadorTodos\">Todos</label>");
        client.println("</div>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"atuador1\" name=\"atuador1\" onclick=\"toggleAtuadorTodos(this)\">");
        client.println("<label for=\"atuador1\">Atuador 1</label>");
        client.println("</div>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"atuador2\" name=\"atuador2\" onclick=\"toggleAtuadorTodos(this)\">");
        client.println("<label for=\"atuador2\">Atuador 2</label>");
        client.println("</div>");
        client.println("<div>");
        client.println("<input type=\"checkbox\" id=\"atuador3\" name=\"atuador3\" onclick=\"toggleAtuadorTodos(this)\">");
        client.println("<label for=\"atuador3\">Atuador 3</label>");
        client.println("</div>");
        client.println("</div>");
        client.println("</div>");
        client.println("</div>");

        sendScriptConfigLog();

        sendEndPage();
    }

    void sendConfigSensores()
    {
        sendHeaderHTML();

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();
        client.println(obterStyle(".container"));
        client.println(obterStyle(".container-header"));
        client.println(obterStyle(".container-header-text"));
        client.println(obterStyle(".subcontainer"));
        client.println(obterStyle(".subcontainer-header"));
        client.println(obterStyle(".form-field"));
        client.println(obterStyle(".form-label-curto"));
        client.println(obterStyle(".form-label-medio"));
        client.println(obterStyle(".form-label-longo"));
        client.println(obterStyle(".button"));
        client.println(obterStyle(".form-dropdown"));
        client.println("</style>");

        client.println("<body>");

        sendTitleAndMenu();

        sendIDControlador();

        client.println("<div class=\"container\">");
        client.println("<div class=\"container-header\">");
        client.println("<h3 class=\"container-header-text\">Sensor X</h3>");
        client.println("</div>");
        client.println("<form id=\"formularioSensor\">");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"nomeSensor\" class=\"form-label-longo\">Nome do Sensor:</label>");
        client.println("<input type=\"text\" id=\"nomeSensor\" name=\"nomeSensor\" placeholder=\"NOME_SENSOR\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"tipoSensor\" class=\"form-label-longo\">Tipo do Sensor:</label>");
        client.println("<select id=\"tipoSensor\" name=\"tipoSensor\" class=\"form-dropdown\">");
        client.println("<option value=\"Temperatura\">Temperatura</option>");
        client.println("<option value=\"Umidade\">Umidade</option>");
        client.println("<option value=\"Iluminação\">Iluminação</option>");
        client.println("<option value=\"Outros\">Outros</option>");
        client.println("</select>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label class=\"form-label-longo\">Valor Atual:</label>");
        client.println("<label id=\"valorAtual\" class=\"form-label-curto\" style=\"margin-left: 38px;\">N/A</label>");
        client.println("<button type=\"button\" class=\"button\">Atualizar</button>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<div class=\"subcontainer\">");
        client.println("<div class=\"subcontainer-header\">");
        client.println("<div class=\"container-header-text\">Valor Mínimo:</div>");
        client.println("</div>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorMin\" class=\"form-label-medio\">Entrada:</label>");
        client.println("<input type=\"text\" id=\"valorMin\" name=\"valorMin\" placeholder=\"Valor Mínimo\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorMin\" class=\"form-label-medio\">Real:</label>");
        client.println("<input type=\"text\" id=\"valorMin\" name=\"valorMin\" placeholder=\"Valor Mínimo\" class=\"form-field\">");
        client.println("</div>");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"space-before\"></div>");
        client.println("<div class=\"subcontainer\">");
        client.println("<div class=\"subcontainer-header\">");
        client.println("<div class=\"container-header-text\">Valor Máximo:</div>");
        client.println("</div>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorMax\" class=\"form-label-medio\">Entrada:</label>");
        client.println("<input type=\"text\" id=\"valorMax\" name=\"valorMax\" placeholder=\"Valor Máximo\" class=\"form-field\">");
        client.println("</div>");
        client.println("<br>");
        client.println("<div class=\"form-field\">");
        client.println("<label for=\"valorMax\" class=\"form-label-medio\">Real:</label>");
        client.println("<input type=\"text\" id=\"valorMax\" name=\"valorMax\" placeholder=\"Valor Máximo\" class=\"form-field\">");
        client.println("</div>");
        client.println("</div>");
        client.println("<br>");

        client.println("<button type=\"submit\" class=\"button\">Salvar</button>");
        client.println("</form>");
        client.println("</div>");

        sendEndPage();
    }

    void sendHeader()
    {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println("Connection: close");
        client.println();
    }

    void sendConstrucao()
    {
        sendHeaderHTML();

        client.println("<style>");
        sendStyleTitleAndMenu();
        sendStyleIDControlador();

        client.println("</style>");

        client.println("<body>");

        sendTitleAndMenu();

        sendIDControlador();

        client.println("Página em construção!");
        client.println("<br>");

        sendEndPage();
    }

    void sendLog()
    {
        sendConstrucao();
    }

    void sendHelp()
    {
        sendConstrucao();
    }

    void sendReset()
    {
        sendConstrucao();
    }

    void processPostData(String postBody)
    {
        DEBUG("Processing PostData");
        DEBUG("Mensagem original:");
        DEBUG(postBody);
        // void parseMessage(const std::string& message, std::string& chave, std::vector<std::pair<std::string, std::string>>& parametros) {
        int pos = 0;
        int endLine;

        // Encontrar a posição da primeira quebra de linha
        endLine = postBody.indexOf('\n');

        if (endLine == -1)
        {
            // Se não houver quebra de linha, retornar
            DEBUG("Mensagem não compreendida!");
            return;
        }

        // Extrair a chave
        String chave = postBody.substring(0, endLine);
        String parameters = postBody.substring(endLine + 1);
        DEBUG("Mensagem tratada:");
        DEBUG(chave);
        if (chave == "RedeExterna")
        {
            while (parameters.indexOf('\n') != -1)
            {
                int endParam = parameters.indexOf('=');
                endLine = parameters.indexOf('\n');
                String param = parameters.substring(0, endParam);
                String value = parameters.substring(endParam + 1, endLine);
                DEBUG(String("Parametro: ") + param + String(" Valor: ") + value);
                parameters = parameters.substring(endLine + 1);

                if (param == "ssid")
                {
                    configs::config.setSSID(value);
                }
                else if (param == "senha")
                {
                    configs::config.setPWD(value);
                }
            }
        }

        /*
        // Iterar sobre as linhas restantes
        while (true)
        {
            // Encontrar a próxima quebra de linha
            pos = endLine + 1;
            endLine = message.find('\n', pos);

            // Se não houver mais quebras de linha, parar
            if (endLine == std::string::npos)
            {
                break;
            }

            // Extrair o parâmetro e valor
            std::size_t equalsPos = message.find('=', pos);
            if (equalsPos != std::string::npos && equalsPos < endLine)
            {
                std::string parametro = message.substr(pos, equalsPos - pos);
                std::string valor = message.substr(equalsPos + 1, endLine - equalsPos - 1);
                parametros.push_back(std::make_pair(parametro, valor));
            }
        }

        int startIndex = postBody.indexOf("ssid=");
        int endIndex = postBody.indexOf("&", startIndex);
        String ssidValue = postBody.substring(startIndex + 5, endIndex);
        // Faça o mesmo para outros campos...
        Serial.println("SSID: " + ssidValue);*/
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
                            DEBUG(header);
                            if (header.indexOf("GET /config/controlador") >= 0)
                            {
                                sendHeader();
                                sendConfigControlador();
                            }
                            else if (header.indexOf("GET /config/sensores") >= 0)
                            {
                                sendHeader();
                                sendConfigSensores();
                            }
                            else if (header.indexOf("GET /config/calibraSensores") >= 0)
                            {
                                sendHeader();
                                sendCalibraSensores();
                            }
                            else if (header.indexOf("GET /config/calibraAtuadores") >= 0)
                            {
                                sendHeader();
                                sendCalibraAtuadores();
                            }
                            else if (header.indexOf("GET /config/Log") >= 0)
                            {
                                sendHeader();
                                sendConfigLog();
                            }
                            else if (header.indexOf("GET /reset") >= 0)
                            {
                                sendHeader();
                                sendReset();
                                configs::resetESP32();
                            }
                            else if (header.indexOf("GET /log") >= 0)
                            {
                                sendHeader();
                                sendLog();
                            }
                            else if (header.indexOf("GET /help") >= 0)
                            {
                                sendHeader();
                                sendHelp();
                            }
                            else if (header.indexOf("POST") >= 0)
                            {
                                while (client.available())
                                {
                                    char c = client.read();
                                    postBody += c;
                                }
                                // Processa os dados do corpo da solicitação POST
                                if (postBody != "")
                                {
                                    processPostData(postBody);
                                }
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

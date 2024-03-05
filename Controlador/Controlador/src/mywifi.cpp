#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <String.h>
#include <ESPmDNS.h>

#include "mywifi.h"
#include "common.h"

namespace mywifi
{
    // Nome do Hostname
    const char *hostname = "estufa-webserver";

    // SSID e senha da rede Wi-Fi
    const char *ssid = "LaptopDe";
    const char *password = "21021992";

    // SSID e senha do Wi-Fi a ser criado
    const char *ssidAP = "ControladorEstufa";
    const char *passwordAP = "EstufaAuto";

    bool APMode = false;
    wifi_event_id_t idEvento = 0;

    String getSSID()
    {
        if(APMode)
        {
            return String(ssidAP);
        }
        else
        {
            return String(ssid);
        }
    }
    
    String getIP()
    {
        if(APMode)
        {
            return WiFi.softAPIP().toString();
        }
        else
        {
            return WiFi.localIP().toString();
        }
    }
    void WiFiReconnect(WiFiEvent_t event, WiFiEventInfo_t info)
    {
        PRINTLN("WiFi desconectado, tentando reconectar...");
        if (APMode)
            return;
        setupWifi();
    }

    void WiFiCheck()
    {
        if (APMode)
        {
            int numRedes = WiFi.scanNetworks();
            for (int i = 0; i < numRedes; i++)
            {
                // DEBUG(WiFi.SSID(i));
                if (WiFi.SSID(i) == ssid)
                {
                    DEBUG("Encontrado um Wifi");
                    setupWifi();
                }
            }
        }
    }

    void setupWifi()
    {
        bool conectado = false;
        PRINTLN("Configurando Wifi");
        // Set o hostname para identificação na rede
        WiFi.setHostname(hostname);

        // Tenta conectar na rede fornecida
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);

        // Verifica se conseguiu entrar
        DEBUG(String("Tentando conectar em ") + ssid);
        for (int i = 0; i < 10; i++)
        {
            // Se conectado termina a função
            if (WiFi.status() == WL_CONNECTED)
            {
                conectado = true;
                DEBUG("Conectado no wifi");
                PRINT("IP address: ");
                PRINTLN(WiFi.localIP());
                idEvento = WiFi.onEvent(WiFiReconnect, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
                APMode = false;
                break;
            }
            // senão timeout
            delay(500);
            Serial.print(".");
        }

        // se não conectou, tenta criar uma AP
        if (conectado == false)
        {
            PRINTLN("Wifi não conectado, trocando para modo AP");
            WiFi.mode(WIFI_AP);
            WiFi.removeEvent(idEvento);
            WiFi.softAP(ssidAP, passwordAP);
            Serial.println("IP address:");
            Serial.println(WiFi.softAPIP());
            APMode = true;
        }

        // Configura o nome de host mDNS
        if (!MDNS.begin(hostname))
        {
            Serial.println("Erro ao configurar o mDNS");
            return;
        }
        else
        {
            Serial.println("mDNS configurado");
        }
    }
}
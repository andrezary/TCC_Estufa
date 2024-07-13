#include <Arduino.h>
#include <string.h>

#include <SPIFFS.h>
#include <time.h>

#include "configs.h"
#include "common.h"

// SSID e senha da rede Wi-Fi
#define SSID_PADRAO "LaptopDe"
#define PWD_PADRAO "21021992"

// SSID e senha do Wi-Fi a ser criado
#define MYSSID_PADRAO "ControladorEstufa"
#define MYPWD_PADRAO "EstufaAuto"

// Nome de colheita e controlador a ser criado
#define COLHEITA_PADRAO "Colheita"
#define CONTROLADOR_PADRAO "Controlador ESP32"

namespace configs
{
    Config config;

    Config::Config()
    {
        strcpy(this->data.nomeColheita, COLHEITA_PADRAO);
        strcpy(this->data.nomeControlador, CONTROLADOR_PADRAO);
        strcpy(this->data.SSID, SSID_PADRAO);
        strcpy(this->data.pwd, PWD_PADRAO);
        strcpy(this->data.mySSID, MYSSID_PADRAO);
        strcpy(this->data.myPWD, MYPWD_PADRAO);
    }

    Config::~Config()
    {
    }

    String Config::getNomeControlador()
    {
        return String(data.nomeControlador);
    }

    String Config::getColheita()
    {
        return String(data.nomeColheita);
    }

    String Config::getSSID()
    {
        return String(data.SSID);
    }

    String Config::getPWD()
    {
        String temp(data.pwd);
        String result;
        for (char c : temp)
        {
            result += '*';
        }
        return result;
    }

    void Config::setSSID(String ssid)
    {
        strcpy(data.SSID, ssid.c_str());
    }

    void Config::setPWD(String pwd)
    {
        strcpy(data.pwd, pwd.c_str());
    }

    void resetESP32()
    {
        PRINTLN("Reiniciando o ESP32 via solicitação WEB!");
        ESP.restart();
        delay(1000);
    }

    void setupConfigs()
    {
        // Iniciar o sistema de arquivos SPIFFS
        DEBUG("SetupConfigs");
        if (!SPIFFS.begin(true))
        {
            PRINTLN("Falha ao montar o sistema de arquivos, usando valores padrão");
        }
        else
        {
            loadConfig();
        }
    }

    void saveConfig()
    {
        File file = SPIFFS.open("/config.txt", "w");
        if (!file)
        {
            PRINTLN("Falha ao salvar as configs.");
            return;
        }
        file.write((uint8_t *)&config, sizeof(Config));
        file.close();
    }

    void loadConfig()
    {
        File file = SPIFFS.open("/config.txt", "r");
        DEBUG("loadConfig");
        if (!file)
        {
            PRINTLN("Falha ao abrir o arquivo, usando configs padrões");
            return;
        }
        if (file.available())
        {
            DEBUG("Arquivo disponivel!");
        }
        else
        {
            DEBUG("Arquivo indisponivel, carregado dados padrões...");
        }
        while (file.available())
        {
            file.read((uint8_t *)&config, sizeof(Config));
        }
        if (file.available())
        {
            DEBUG("Arquivo encerrado!");
        }

        file.close();
    }
}
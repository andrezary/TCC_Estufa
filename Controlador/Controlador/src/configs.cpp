#include <Arduino.h>
#include <string.h>

#include <SPIFFS.h>
#include <time.h>

#include "configs.h"
#include "common.h"



/**
 * Espaço para implementação de dados e funções referentes as
 * configurações da estufa.
 */
namespace configs
{
    /**
     * configs::config é a variavel global encapsulada no espaço configs
     * que armazena todas as configurações necessárias para interação 
     * com o usuario e também para trabalho.
     * 
     * 
     */
    Config config;

    Config::Config()
    {
        strcpy(this->data.nomeColheita, COLHEITA_PADRAO);
        strcpy(this->data.nomeControlador, CONTROLADOR_PADRAO);
        strcpy(this->data.SSID, SSID_PADRAO);
        strcpy(this->data.pwd, PWD_PADRAO);
        strcpy(this->data.mySSID, MYSSID_PADRAO);
        strcpy(this->data.myPWD, MYPWD_PADRAO);
        strcpy(this->data.hostname, HOSTNAME);

        this->data.portaIniAtuadores = 0;
        this->data.portaIniSensores = 0;
        this->data.qtdAtuadores = 0;
        this->data.qtdSensores = 0;
        this->data.tempoAmostragem = 0;
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
        return data.pwd;
    }

    String Config::getPWDHided()
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

    String Config::getMyPWD()
    {
        return data.myPWD;
    }

    String Config::getMyPWDHided()
    {
        String temp(data.myPWD);
        String result;
        for (char c : temp)
        {
            result += '*';
        }
        return result;
    }

    String Config::getMySSID()
    {
        DEBUG("--------------------------------------------");
        DEBUG(String("MySSID:")+String(data.mySSID));
        DEBUG("--------------------------------------------");
        return String(data.mySSID);
    }

    void Config::setMySSID(String ssid)
    {
        strcpy(data.mySSID, ssid.c_str());
    }

    void Config::setMyPWD(String pwd)
    {
        strcpy(data.myPWD, pwd.c_str());
    }

    char Config::getQtdSensores()
    {
        return data.qtdSensores;
    }

    void Config::setQtdSensores(char num)
    {
        data.qtdSensores = num;
    }

    char Config::getPortaIniSensores()
    {
        return data.portaIniSensores;
    }

    void Config::setPortaIniSensores(char num)
    {
        data.portaIniSensores = num;
    }

    char Config::getQtdAtuadores()
    {
        return data.qtdAtuadores;
    }

    void Config::setQtdAtuadores(char num)
    {
        data.qtdAtuadores = num;
    }

    char Config::getPortaIniAtuadores()
    {
        return data.portaIniAtuadores;
    }

    void Config::setPortaIniAtuadores(char num)
    {
        data.portaIniAtuadores = num;
    }

    int Config::getTempoAmostragem()
    {
        return data.tempoAmostragem;
    }

    void Config::setTempoAmostragem(int num)
    {
        data.tempoAmostragem = num;
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
            DEBUG("Lendo arquivo de configuração");
        }
        if (file.available())
        {
            DEBUG("Arquivo encerrado!");
        }

        file.close();
    }
}
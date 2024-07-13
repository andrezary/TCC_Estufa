#ifndef CONFIGS_H
#define CONFIGS_H

#include <Arduino.h>

namespace configs
{
    struct Data
    {
        char nomeControlador[33];
        char nomeColheita[64];
        char dia;
        char mes;
        int ano;
        char hora;
        char min;
        char servidor[256];
        char SSID[33];
        char pwd[65];
        char mySSID[33];
        char myPWD[65];
        char qtdSensores;
        char portaIniSensores;
        char qtdAtuadores;
        char portaIniAtuadores;
        int tempoAmostragem;
    };

    class Config
    {
        Data data;

        
        public:
        String getNomeControlador();
        String getColheita();
        String getSSID();
        String getPWD();
        void setSSID(String ssid);
        void setPWD(String pwd);

        Config();

        ~Config();
    };

    extern Config config;
    
    void resetESP32();

    void setupConfigs();
    
    void loadConfig();
    void saveConfig();
    
}
#endif
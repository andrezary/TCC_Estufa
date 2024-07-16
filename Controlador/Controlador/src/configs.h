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

        char hostname[64];
    };

    class Config
    {
        private:
        Data data;

        
        public:
        String getNomeControlador();
        String getColheita();
        
        String getSSID();
        String getPWD();
        String getPWDHided();
        void setSSID(String ssid);
        void setPWD(String pwd);

        String getMyPWD();
        String getMyPWDHided();
        String getMySSID();
        void setMySSID(String ssid);
        void setMyPWD(String pwd);
        

        char getQtdSensores();
        void setQtdSensores(char num);

        char getPortaIniSensores();
        void setPortaIniSensores(char num);

        char getQtdAtuadores();
        void setQtdAtuadores(char num);

        char getPortaIniAtuadores();
        void setPortaIniAtuadores(char num);

        int getTempoAmostragem();
        void setTempoAmostragem(int num);

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
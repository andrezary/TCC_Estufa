#ifndef CONFIGS_H
#define CONFIGS_H

#include "common.h"

#include <Arduino.h>

namespace configs
{

    struct Data
    {
        char nomeControlador[TAM_ARRAY_NOMES];
        char nomeColheita[TAM_ARRAY_NOMES]; 
        
        char dia;
        char mes;
        int ano;
        char hora;
        char min;
        char servidorNTP[256];

        char SSID[TAM_ARRAY_NOMES];
        char pwd[TAM_ARRAY_NOMES];
        
        char mySSID[TAM_ARRAY_NOMES];
        char myPWD[TAM_ARRAY_NOMES];

        char qtdSensores;
        char portaIniSensores;
        char qtdAtuadores;
        char portaIniAtuadores;
        int tempoAmostragem;

        char hostname[TAM_ARRAY_NOMES]; //TODO: Tem que ver a utilização disso e mudança disso na UI
        char hostMaster[TAM_ARRAY_NOMES];
    };

    class Config
    {
        private:
        Data data;

        
        public:
        String getServidorNTP();
        void setServidorNTP(String value);

        String getHostMaster();
        void setHostMaster(String value);

        String getNomeControlador();
        void setNomeControlador(String value);
        String getColheita();
        void setColheita(String value);

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
    

}
#endif
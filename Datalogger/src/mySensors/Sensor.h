#ifndef SENSOR_H
#define SENSOR_H

#include <mySensors/mySensors.h>
#include <mySensors/sensors.h>
#include <mySensors/calibracao.h>

#include "common.h"

#pragma once
namespace mySensors
{
    class Sensor
    {
    private:
        char nome[TAM_ARRAY_NOMES];
        Tipo tipo;
        int porta;
        calibracao calib;
        bool configured;
        
    public:
        String getName();
        void setName(const char *str);

        Tipo getTipo();
        void setTipo(Tipo type);

        int getPorta();
        void setPorta(int value);

        calibracao getCalibracao();
        void setCalibracao(calibracao value);

        float getVoltageRead();
        float getPhysicalRead();
        short getADCRead();
        void setVoltageValue(float value);
        void setPhysicalValue(float value);
        void setADCValue(short value);
        
        Sensor();
        ~Sensor();
    };
}


#endif
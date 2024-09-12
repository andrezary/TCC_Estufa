#include "Sensor.h"

namespace mySensors
{
    Sensor::Sensor()
    {
    }

    Sensor::~Sensor()
    {
    }

    
    String Sensor::getName()
    {
        return (const char*)this->nome;
    }
    void Sensor::setName(const char *str)
    {
        strncpy(nome, str, TAM_ARRAY_NOMES);
        nome[TAM_ARRAY_NOMES - 1] = '\0';
    }

    Tipo Sensor::getTipo()
    {
        return this->tipo;
    }

    void Sensor::setTipo(Tipo type)
    {
        this->tipo = type;
    }

    int Sensor::getPorta()
    {
        return this->porta;
    }

    void Sensor::setPorta(int value)
    {
        this->porta = value;
    }

    float Sensor::getVoltageRead()
    {
        return 0;
    }

    float Sensor::getPhysicalRead()
    {
        return 0;
    }

    short Sensor::getADCRead()
    {
        return 0;
    }

    void Sensor::setVoltageValue(float value)
    {
        ;
    }

    void Sensor::setPhysicalValue(float value)
    {
        ;
    }

    void Sensor::setADCValue(short value)
    {
        ;
    }

    calibracao Sensor::getCalibracao()
    {
        return this->calib;
    }

    void Sensor::setCalibracao(calibracao value)
    {
        this->calib = value;
    }

    Sensor& Sensor::operator=(const MsgData&)
    {
        return *this;
    }
}

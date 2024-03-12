#include <Arduino.h>
#include "sensors.h"

namespace sensors
{
    Sensor::Sensor(/* args */)
    {
    }

    Sensor::~Sensor()
    {
    }

    RegressaoLinear::RegressaoLinear(/* args */)
    {
    }

    RegressaoLinear::~RegressaoLinear()
    {
    }

    RegressaoTrechos::RegressaoTrechos(/* args */)
    {
    }

    RegressaoTrechos::~RegressaoTrechos()
    {
    }

    RegressaoSimples::RegressaoSimples(/* args */)
    {
    }

    RegressaoSimples::~RegressaoSimples()
    {
    }

    String Sensor::getName()
    {
        return this->name;
    }
    void Sensor::setName(const char *str)
    {
        this->name = str;
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

    int Sensor::getValor()
    {
        return 0;
    }

    int Sensor::getRawValue()
    {
        return 0;
    }
    void Sensor::setRawValue(int value)
    {
        ;
    }
    void Sensor::setValor(int value)
    {
        ;
    }

    bool Sensor::getEstadoAcionador()
    {
        return estadoAcionador;
    }
    int Sensor::getPortaAcionador()
    {
        return this->portaAcionador;
    }
    void Sensor::setPortaAcionador(int value)
    {
        ;
    }

    int Sensor::getValorSubida()
    {
        return this->valorSubida;
    }
    int Sensor::getValorDescida()
    {
        return this->valorDescida;
    }
    void Sensor::setValorSubida(int value)
    {
        ;
    }
    void Sensor::setValorDescida(int value)
    {
        ;
    }
}

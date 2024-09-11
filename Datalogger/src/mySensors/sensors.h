#ifndef SENSORS_H
#define SENSORS_H

namespace mySensors
{
    enum Tipo
    {
        temperatura,
        umidade,
        luz,
        outros
    };

    void setup();

    void setupPins();
}
#endif
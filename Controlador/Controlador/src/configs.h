#ifndef CONFIGS_H
#define CONFIGS_H

#include <Arduino.h>

namespace configs
{
    String getNomeControlador();
    String getColheita();
    void resetESP32();
}
#endif
#include <Arduino.h>
#include "configs.h"
#include "common.h"

namespace configs
{
    String getNomeControlador()
    {
        return "configs::NomeControlador";
    }
    String getColheita()
    {
        return "configs::Colheita";
    }
    void resetESP32()
    {
        PRINTLN("Reiniciando o ESP32 via solicitação WEB!");
        ESP.restart();
        delay(1000);
    }
}
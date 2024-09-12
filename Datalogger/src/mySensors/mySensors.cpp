#include "mySensors/mySensors.h"
#include "mySerial/mySerial.h"
#include "myWifi/myWifi.h"

namespace mySensors
{
    Sensor sensores[MYSENSORS_MAX_SENSORES];
    int error;

    void setup()
    {
        if(!tryReadSensorConfigFile())
        {
            myPrintln("Configuração de sensores não encontrada.", THREAD_MAIN);
            error = MYSENSORS_CONFIG_NOT_FOUND;
            if(!mySerial::haveSensors())
            {
                myPrintln("Configuração de sensores não encontrada no controlador.", THREAD_MAIN);
                
                if(!myWifi::haveSensors())
                {
                    myPrintln("Configuração de sensores não encontrada no mestre.", THREAD_MAIN);
                    threadDelay(MYSENSORS_DELAY_SETUP);
                    return;
                }
                else
                {
                    for(int i = 0; i < MYSENSORS_MAX_SENSORES; i++)
                    {
                        sensores[i] = myWifi::getSensorConfig(i);
                    }
                    myPrintln("Configuração de sensores encontrada no mestre.", THREAD_MAIN);
                }
            }
            else
            {
                for(int i = 0; i < MYSENSORS_MAX_SENSORES; i++)
                {
                    sensores[i] = mySerial::getSensor(i);
                }
                myPrintln("Configuração de sensores encontrada no controlador.", THREAD_MAIN);
            }
        }
        error = MYSENSORS_NO_ERROR;
        threadDelay(MYSENSORS_DELAY_SETUP);
    }
}
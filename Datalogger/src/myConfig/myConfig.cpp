#include <freertos/FreeRTOS.h>

#include "common.h"
#include "myConfig/myConfig.h"

#define MYCONFIG
#include "myConfig/Status.h"
#include "mySerial/mySerial.h"
#include "myWifi/myWifi.h"

#include "Config.h"

//LED 0 (On board): Runing
//LED 1:            Serial
//LED 2:
//LED 3:

namespace myConfig
{
    myConfig::Status status;
    
    void setup()
    {
        xTaskCreate(
            processStatusLEDS,
            "StatusLEDs",
            1024,
            NULL,
            0,
            NULL
        );
        myPrintln("ThreadLEDS Iniciada.", THREAD_MAIN);
        setupConfigs();
    }

    void processStatusLEDS(void* param)
    {
        while(true)
        {
            if(!status.getRun())
            {
                digitalWrite(PIN_ONBOARD_LED, !digitalRead(PIN_ONBOARD_LED));
            }
            else
            {
                digitalWrite(PIN_ONBOARD_LED, HIGH);
            }

            uint8_t errorCode = mySerial::getErrorStatus();

            switch (errorCode)
            {
                case CONNECTING_ERROR:
                    digitalWrite(PIN_STATUS1_LED, !digitalRead(PIN_STATUS1_LED));
                    break;
                case ERROR_INIT_ERROR:
                    digitalWrite(PIN_STATUS1_LED, LOW);
                    break;
                case NO_ERROR:
                    digitalWrite(PIN_STATUS1_LED, HIGH);
                    break;
                default:
                    break;
            }

            if(myWifi::getAPMode())
            {
                digitalWrite(PIN_STATUS2_LED, !digitalRead(PIN_STATUS2_LED));
            }
            else{
                digitalWrite(PIN_STATUS2_LED, HIGH);
            }
        threadDelay(THREAD_LED_TIME_DELAY);
        }        
    }

    void setStatusRun(bool param)
    {
        status.setRun(param);
    }

    bool getStatusRun()
    {
        return status.getRun();
    }
}
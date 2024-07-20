#include "mySerial.h"
#include "common.h"

#include <HardwareSerial.h>
#include<freertos/FreeRTOS.h>


namespace mySerial
{
    HardwareSerial SerialUART(1);

    void setup()
    {
        SerialUART.begin(115200, SERIAL_8N1, 16, 17);
        PRINTLN("Serial UART iniciada!");

        xTaskCreate(
            SerialESPs,
            "SerialESPs",
            2048,
            NULL,
            1,
            NULL
        );
        PRINTLN("Criada a task da Serial");
    }

    void SerialESPs(void* param)
    {

    }

}
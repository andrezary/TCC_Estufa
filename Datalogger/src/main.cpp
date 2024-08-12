#include <Arduino.h>
#include "mySerial.h"

void setup() {
    Serial.begin(115200);
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    mySerial::setup();
}

bool Run = false;
void loop() {
    delay(500);
    if(Serial.available())
    {
        String str = Serial.readStringUntil('\n');

        str.trim();
        str.toLowerCase();

        if(str == "passo")
        {
            Run = true;
        }
    }
    
    uint8_t errorCode = mySerial::getErrorStatus();
    switch (errorCode)
    {
    case CONNECTING_ERROR:
        digitalWrite(33, !digitalRead(33));
        break;
    case ERROR_INIT_ERROR:
        digitalWrite(33, HIGH);
        break;
    case NO_ERROR:
        digitalWrite(33, LOW);
        break;
    default:
        break;
    }

}

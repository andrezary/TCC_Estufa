#include <Arduino.h>
#include "mySerial.h"

void setup() {
    Serial.begin(115200);
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
}

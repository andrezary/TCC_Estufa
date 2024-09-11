#include "common.h"

#include <Arduino.h>
#include <time.h>
//#include "mySensors/pins.h"
//#include "myAtuadores/pins.h"

#define ADC_PIN_FIRST           1
#define ADC_PIN_LAST            12
#define DIGITALPIN_FIRST        13
#define DIGITALPIN_LAST         28
#define DAC_PIN_FIRST           29
#define DAC_PIN_LAST            30

void setupPins()
{
    pinMode(PIN_ONBOARD_LED, OUTPUT);
    pinMode(PIN_STATUS1_LED, OUTPUT);
    pinMode(PIN_STATUS2_LED, OUTPUT);
    pinMode(PIN_STATUS3_LED, OUTPUT);

    pinMode(PIN_START_BUTTON, INPUT);


}

int pinRead(uint8_t pin)
{
    if(pin <= ADC_PIN_LAST && pin >= ADC_PIN_FIRST)
    {
        if()
    }
}

void pinWrite(uint8_t pin, int value)
{

}

String getHora(bool full)
{
    struct tm timeinfo;

    getLocalTime(&timeinfo);

    char strTime[9];
    if(!full)
        strftime(strTime, sizeof(strTime), "%H:%M", &timeinfo);
    else
        strftime(strTime, sizeof(strTime), "%H:%M:%S", &timeinfo);
    
    return String(strTime);
}

String getData(bool formatoHTML)
{
    struct tm timeinfo;

    getLocalTime(&timeinfo);

    char strDate[11];
    if(!formatoHTML)
        strftime(strDate, sizeof(strDate), "%d/%m/%Y", &timeinfo);
    else
        strftime(strDate, sizeof(strDate), "%Y-%m-%d", &timeinfo);
    return String(strDate);
}

String decodeSTR(const String &str) {
    String decoded = "";
    int len = str.length();
    for (int i = 0; i < len; i++) {
        if (str[i] == '%' && i + 2 < len) {
            // Pegue o caractere hexadecimal após o '%'
            String hex = str.substring(i + 1, i + 3);
            char decodedChar = (char) strtol(hex.c_str(), NULL, 16);
            decoded += decodedChar;
            i += 2; // Avance os próximos dois caracteres
        } else {
            decoded += str[i];
        }
    }
    return decoded;
}

short int verboseThreads = (1 << THREAD_SERIAL) | 0;

void myPrintln(const char* str, char thread)
{
    if((verboseThreads & (1 << thread)) == 0)
        return;

    switch (thread)
    {
    case THREAD_SERIAL:
        Serial.println("-- Inicio Thread Serial--");

        break;
    
    default:
        break;
    }

    Serial.println(str);

    switch (thread)
    {
    case THREAD_SERIAL:
        Serial.println("-- Fim Thread Serial--");

        break;
    
    default:
        break;
    }
}


void setVerbosite(char thread, bool state)
{
    if(state)
        verboseThreads |= (1 << thread);
    else
        verboseThreads &= (1 << thread);
}
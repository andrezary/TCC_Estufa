#include <Arduino.h>
#include <String.h>
#include <time.h>

#include "common.h"
#include "mySerial/mySerial.h"
#include "myConfig/myConfig.h"
#include "myConfig/Config.h"

#include "myWifi/myWifi.h"
#include "InterfaceWeb/interfaceWeb.h"


#define DELAY_LOOPS 300

void setupRTC()
{
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.printf("Ano: %04d, Mês: %02d, Dia: %02d, Hora: %02d, Minuto: %02d, Segundo: %02d\n",
                                    timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    myPrintln("RTC interno não está configurado!", THREAD_MAIN);
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 6;
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 12;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    time_t t = mktime(&timeinfo);
    struct timeval now = { .tv_sec = t};
    settimeofday(&now, NULL);

    myPrintln("Configuradas Data/Hora padrões.", THREAD_MAIN);
  }
  else {
    myPrintln("RTC interno configurado e funcionando!", THREAD_MAIN);
    Serial.printf("Ano: %04d, Mês: %02d, Dia: %02d, Hora: %02d, Minuto: %02d, Segundo: %02d\n",
                                    timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
}

void setup() {
    pinMode(PIN_ONBOARD_LED, OUTPUT);
    pinMode(PIN_STATUS1_LED, OUTPUT);
    pinMode(PIN_STATUS2_LED, OUTPUT);
    pinMode(PIN_STATUS3_LED, OUTPUT);

    pinMode(PIN_START_BUTTON, INPUT);

    mySerial::setup();
    myConfig::setup();
    setupRTC();
    myWifi::setupWifi();
    InterfaceWeb::setupServer();
}

bool Run = false;

void loop() {
    threadDelay(10);
    myWifi::WiFiCheck();
    InterfaceWeb::loop();
    
    switch (mySerial::processSerialUSB())
    {
        case SERIALUSB_CMD_PASSO:
            Run = true;
            break;
        case SERIALUSB_CMD_RUN:
            myConfig::setStatusRun(true);
            PRINTLN("Rodando o código!");
            break;
        case SERIALUSB_CMD_STOP:
            myConfig::setStatusRun(false);
            PRINTLN("Parando o código!");
            break;
        case SERIALUSB_CMD_RESET:
            myConfig::resetESP32();
            break;
        case SERIALUSB_CMD_NOTHING:
        default:
            break;
    }
}

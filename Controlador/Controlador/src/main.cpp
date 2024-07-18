#include<Arduino.h>
#include <String.h>
#include <WiFiServer.h>
#include <time.h>

#include "common.h"
#include "interfaceWeb.h"
#include "mywifi.h"
#include "configs.h"
#include "Controller.h"

void setupRTC()
{
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.printf("Ano: %04d, Mês: %02d, Dia: %02d, Hora: %02d, Minuto: %02d, Segundo: %02d\n",
                                    timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    PRINTLN("RTC interno não está configurado!");
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 6;
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 12;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    time_t t = mktime(&timeinfo);
    struct timeval now = { .tv_sec = t};
    settimeofday(&now, NULL);

    PRINTLN("Configuradas Data/Hora padrões.");
  }
  else {
    PRINTLN("RTC interno configurado e funcionando!");
    Serial.printf("Ano: %04d, Mês: %02d, Dia: %02d, Hora: %02d, Minuto: %02d, Segundo: %02d\n",
                                    timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                    timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
}

void setup()
{
  PRINTLN("Inicializando o esp.");
  Serial.begin(115200);
  PRINTLN("Iniciando serviço RTC");
  setupRTC();
  configs::setupConfigs();

  // Configura o Wifi
  mywifi::setupWifi();
  InterfaceWeb::setupServer();
  

}

void loop()
{
  delay(100);
  mywifi::WiFiCheck();
  InterfaceWeb::loop();
  Controller::loop();

}

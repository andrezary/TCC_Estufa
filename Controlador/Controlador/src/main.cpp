#include<Arduino.h>
#include <String.h>
#include <WiFiServer.h>

#include "interfaceWeb.h"
#include "mywifi.h"
#include "configs.h"
#include "Controller.h"


void setup()
{
  Serial.begin(115200);

  // Configura o Wifi
  mywifi::setupWifi();
  InterfaceWeb::setupServer();
  configs::setupConfigs();

}

void loop()
{
  delay(100);
  mywifi::WiFiCheck();
  InterfaceWeb::loop();
  Controller::loop();

}

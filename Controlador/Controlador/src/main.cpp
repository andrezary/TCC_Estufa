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

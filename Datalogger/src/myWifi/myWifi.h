#ifndef MYWIFI_H
#define MYWIFI_H
#define DATALOGER

#pragma once

#ifdef DATALOGER
    #define MYWIFI_SSID_AP "DataloggerEstufa"
#else
    #define MYWIFI_SSID_AP "ControladorEstufa"
#endif

namespace myWifi{
    void setupWifi();
    void WiFiCheck();
    String getIP();
    String getSSID();
    bool getAPMode();
}

#endif
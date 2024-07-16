#ifndef COMMOM_H
#define COMMOM_H

#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG(texto) Serial.println(texto)
#else
#define DEBUG(texto)
#endif

#ifndef SILENT_MODE
#define PRINTLN(texto) Serial.println(texto)
#define PRINT(texto) Serial.print(texto)
#else
#define PRINTLN(texto)
#define PRINT(texto)
#endif

//////////////////////////////////////////////
//Usado em Configs e myWifi

// SSID e senha da rede Wi-Fi
#define SSID_PADRAO "LaptopDe"
#define PWD_PADRAO "21021992"

// SSID e senha do Wi-Fi a ser criado
#define MYSSID_PADRAO "ControladorEstufa"
#define MYPWD_PADRAO "EstufaAuto"

// Nome de colheita e controlador a ser criado
#define COLHEITA_PADRAO "Colheita"
#define CONTROLADOR_PADRAO "Controlador ESP32"

// Nome do Hostname
#define HOSTNAME "estufa-webserver"


/////////////////////////////////////////////////
#endif /*COMMOM_H*/
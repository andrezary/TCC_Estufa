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

#endif /*COMMOM_H*/
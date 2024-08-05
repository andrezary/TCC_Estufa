#ifndef MSGDATA_H
#define MSGDATA_H

#pragma once

#include <stdint.h>

#include "common.h"

//ID_Msg
#define BLANK_MSG               0
#define INIT_DATA               1
#define END_DATA                2
#define MSG_OK                  3
#define INIT_SYSTEM             4
#define RESTART_SYSTEM          5
#define I_AM_ALIVE              6
#define MASTER_ALIVE            7
#define HORARIO_SYNC            8
#define SENSOR_SIGNAL           9
#define ATUADOR_CHANGED         10
#define CONFIG_MSG              11
#define MSG_ERROR               12

//MsgType
#define I_AM_DATALOGGER         2
#define I_AM_CONTROLLER         1

class MsgData
{
    public:
    float value;
    char strValue[TAM_ARRAY_NOMES];
    uint8_t ID_Msg;
    uint8_t MsgType;
    
    

    MsgData();
    MsgData(uint8_t id, uint8_t type, float value, const char* str);

    bool operator==(const MsgData& data) const;
    void clear();
    String c_str();
};

#endif
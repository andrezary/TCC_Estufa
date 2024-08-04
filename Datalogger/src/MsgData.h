#ifndef MSGDATA_H
#define MSGDATA_H

#pragma once

#include <stdint.h>

#include "common.h"

class MsgData
{
    public:
    float value;
    char strValue[TAM_ARRAY_NOMES];
    uint8_t ID_Msg;
    uint8_t MsgType;
    
    

    MsgData();
    MsgData(uint8_t id, uint8_t type, float value, const char* str);

    void clear();
};

#endif
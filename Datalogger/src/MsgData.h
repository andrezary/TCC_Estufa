#ifndef MSGDATA_H
#define MSGDATA_H

#pragma once

#include <stdint.h>

#include "common.h"

class MsgData
{
    public:
    uint8_t ID_Msg;
    uint8_t MsgType;
    float value;
    char strValue[TAM_ARRAY_NOMES];

    MsgData();
    MsgData(uint8_t id, uint8_t type, float value, const char* str);

    void clear();
};

#endif
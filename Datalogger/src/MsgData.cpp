#include <Arduino.h>

#include "common.h"

#include "MsgData.h"

MsgData::MsgData()
: ID_Msg(0), MsgType(0), value(0)
{
    memset(strValue, 0, sizeof(strValue));
}

MsgData::MsgData(uint8_t id, uint8_t type, float value, const char* str)
: ID_Msg(id), MsgType(type), value(value)
{
    memset(strValue, 0, sizeof(strValue));
    strncpy(strValue, str, TAM_ARRAY_NOMES - 1);
}

void MsgData::clear()
{
    memset(strValue, 0, sizeof(strValue));
    ID_Msg = 0;
    MsgType = 0;
    value = 0;
}
#include <Arduino.h>

#include "common.h"
#include "MsgData.h"

uint8_t MsgData::idCount = 0;

MsgData::MsgData()
: ID_Msg(idCount++), MsgType(0), value(0)
{
    memset(strValue, 0, sizeof(strValue));
}


MsgData::MsgData(uint8_t type, float value, const char* str)
: MsgData(idCount++, type, value, str)
{

}

MsgData::MsgData(uint8_t id, uint8_t type, float value, const char* str)
: ID_Msg(id), MsgType(type), value(value)
{
    memset(strValue, 0, sizeof(strValue));
    strncpy(strValue, str, TAM_ARRAY_NOMES - 1);
}

bool MsgData::operator== (const MsgData& data) const
{
    bool ret = false;
    
    if(this->value == data.value)
    {
        ret = true;
    }

    if(this->ID_Msg == data.ID_Msg)
    {
        ret = true;
    }

    if(this->MsgType == data.MsgType)
    {
        ret = true;
    }

    if(strcmp(this->strValue, data.strValue) == 0)
    {
        ret = true;
    }

    return ret;
}

void MsgData::clear()
{
    memset(strValue, 0, sizeof(strValue));
    ID_Msg = 0;
    MsgType = 0;
    value = 0;
}

String MsgData::c_str()
{
    String ret("Mensagem: ");

    switch (this->MsgType)
    {
    case INIT_DATA:
        ret += "Init_Data\n";
        break;
    case END_DATA:
        ret += "End_Data\n";
        break;
    case BLANK_MSG:
        ret += "Blank_Msg\n";
        break;
    case MSG_OK:
        ret += "Msg ok\n";
        break;
    case INIT_SYSTEM:
        ret += "Init_system\n";
        break;
    case RESTART_SYSTEM:
        ret += "Restart_System\n";
        break;
    case I_AM_ALIVE:
        ret += "I am alive\n";
        break;
    case MASTER_ALIVE:
        ret += "Master alive\n";
        break;
    case HORARIO_SYNC:
        ret += "Sincronia de relógio\n";
        break;
    case SENSOR_SIGNAL:
        ret += "Sinal de sensor\n";
        break;
    case ATUADOR_CHANGED:
        ret += "Sinal de atuador\n";
        break;
    case CONFIG_MSG:
        ret += "Config msg\n";
        break;
    case MSG_ERROR:
        ret += "Erro na msg\n";
        break;
    default:
        ret += "MsgType Desconhecido";
        break;
    }
    ret += "ID: ";
    ret += this->ID_Msg;
    ret += "\n";
    ret += "Value: ";
    ret += this->value;
    ret += "\n";
    ret += "String: ";
    ret += this->strValue;
    ret += "\n";
    return ret;    
}
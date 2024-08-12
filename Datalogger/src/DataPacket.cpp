#include <stdint.h>
#include <stddef.h>

#include "DataPacket.h"


void DataPacket::calcChecksum()
{
    uint8_t* p = (uint8_t*) (&this->msg);

    checksum = 0;

    for(size_t i = 0; i < sizeof(MsgData); i++)
    {
        this->checksum += p[i];
    }
}

bool DataPacket::isChecksumOK()
{
    uint32_t sum = 0;

    uint8_t* p = (uint8_t*) (&this->msg);

    for(size_t i = 0; i < sizeof(MsgData); i++)
    {
        sum += p[i];
    }
    
    if(this->checksum == sum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

DataPacket::DataPacket()
: checksum(0)
{

}
DataPacket::DataPacket(uint8_t id, uint8_t msgType, float value, const char* str)
: msg(id, msgType, value, str), checksum(0)
{
    this->calcChecksum();
}

DataPacket::DataPacket(uint8_t msgType, float value, const char* str)
: msg(msgType, value, str), checksum(0)
{
    this->calcChecksum();
}

DataPacket::DataPacket(uint8_t msgType, const char* str)
: DataPacket(msgType, 0, str)
{

}

DataPacket::DataPacket(uint8_t msgType)
: DataPacket(msgType, 0, "\0")
{

}

DataPacket::DataPacket(uint8_t msgType, float value)
: DataPacket(msgType, value, "\0")
{
    
}
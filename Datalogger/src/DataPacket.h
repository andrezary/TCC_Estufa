#ifndef DATAPACKET_H
#define DATAPACKET_H

#pragma once

#include <stdint.h>

#include "common.h"
#include "MsgData.h"

class DataPacket
{
    public:
    MsgData msg;
    uint32_t checksum;

    DataPacket();
    DataPacket(uint8_t id, uint8_t msgType, float value, const char* str);

    void calcChecksum();
    bool isChecksumOK();
};

#endif
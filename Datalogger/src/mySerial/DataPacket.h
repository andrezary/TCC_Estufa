#ifndef DATAPACKET_H
#define DATAPACKET_H

#pragma once

#include <stdint.h>

#include "common.h"
#include "MsgData.h"

namespace mySerial
{
    class DataPacket
    {
        public:
        MsgData msg;
        uint32_t checksum;

        DataPacket();
        DataPacket(uint8_t id, uint8_t msgType, float value, const char* str);
        DataPacket(uint8_t msgType, float value, const char* str);
        DataPacket(uint8_t msgType, const char* str);
        DataPacket(uint8_t msgType);
        DataPacket(uint8_t msgType, float value);

        void calcChecksum();
        bool isChecksumOK();
    };
}

#endif
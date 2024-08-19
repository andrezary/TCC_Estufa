#ifndef STATUS_H
#define STATUS_H

#pragma once

#include "common.h"
#include "mySerial/MsgData.h"

#define POOL_SIZE               50

namespace mySerial
{
    class Status
    {
        private:
        mySerial::MsgData msgsSended[POOL_SIZE];
        int8_t isReceived[POOL_SIZE];
        int8_t poolSended;

        mySerial::MsgData msgsReceived[POOL_SIZE];
        int8_t poolReceived;
        
        void incrementPoolSended();
        void incrementPoolReceived();

        public:
        int8_t initiated;
        bool correlated;

        uint8_t error;
        
        unsigned long lastTimeAlive;
        bool sendedAlive;
        
        Status();

        void reset();

        void pushMsgSended(mySerial::MsgData msg);
        void pushMsgReceived(mySerial::MsgData msg);
        
        bool verifyMsgSended(mySerial::MsgData msg);
        bool verifyMsgReceived(mySerial::MsgData msg);

        void initInterpreted(mySerial::MsgData msg);

        bool hasInitiated();
    };
}


#endif
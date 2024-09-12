#ifndef STATUS_H
#define STATUS_H

    #ifndef MYSERIAL
        #error "Status do myConfig está sendo puxado em outro arquivo."
    #endif
    
    #pragma once

#include "common.h"
#include "MsgData.h"

#define POOL_SIZE               50

namespace mySerial
{
    class Status
    {
        private:
        MsgData msgsSended[POOL_SIZE];
        int8_t isReceived[POOL_SIZE];
        int8_t poolSended;

        MsgData msgsReceived[POOL_SIZE];
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

        void pushMsgSended(MsgData msg);
        void pushMsgReceived(MsgData msg);
        
        bool verifyMsgSended(MsgData msg);
        bool verifyMsgReceived(MsgData msg);

        void initInterpreted(MsgData msg);

        bool hasInitiated();
    };
}


#endif
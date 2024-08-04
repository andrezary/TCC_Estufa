#ifndef STATUS_H
#define STATUS_H

#pragma once

#include "common.h"
#include "MsgData.h"

#define POOL_SIZE               50

class Status
{
    public: //
    MsgData pool[POOL_SIZE];
    int8_t isReceived[POOL_SIZE];

    MsgData msgsReceived[POOL_SIZE];

    bool initiated;
    bool correlated;

    uint8_t error;
    
    Status();

    void reset();

    void pushMsgSended(MsgData msg);
    void pushMsgReceived(MsgData msg);
    
    bool verifyMsgSended(MsgData msg);
    bool verifyMsgReceived(MsgData msg);

};

#endif
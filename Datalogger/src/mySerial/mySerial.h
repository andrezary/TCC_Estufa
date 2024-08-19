#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <stdint.h>

#include "common.h"
#include "mySerial/DataPacket.h"

#define TIME_TO_INIT            10000
#define WAIT_TIME               100
#define TIMEOUT_SERIAL          10000
#define TIME_TO_RETRY           250
#define TIME_I_AM_ALIVE         5000



#define SERIALUSB_CMD_NOTHING   0
#define SERIALUSB_CMD_PASSO     1
#define SERIALUSB_CMD_RUN       2
#define SERIALUSB_CMD_STOP      3


namespace mySerial
{
    void setup();

    void SerialESPs(void* param);

    void sendData(mySerial::DataPacket);
    void sendMsgOk(int msgItem_ID);

    bool msgsAvailable();

    mySerial::DataPacket receiveMsg();

    uint8_t getErrorStatus();

    uint8_t processSerialUSB();
}

#endif
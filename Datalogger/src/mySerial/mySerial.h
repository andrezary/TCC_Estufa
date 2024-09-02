#ifndef MYSERIAL_H
#define MYSERIAL_H

#pragma once

#include <stdint.h>

#include "common.h"
#include "mySerial/DataPacket.h"
#include "mySensors/Sensor.h"

#define MYSERIAL_TIME_TO_INIT               10000
#define MYSERIAL_TIMEOUT_SERIAL             10000
#define MYSERIAL_TIME_TO_RETRY_CONNECTION   300000 //5min para recomeçar!
#define MYSERIAL_TIME_LOOPS                 300
#define TIME_I_AM_ALIVE                     5000

#define SERIALUSB_CMD_NOTHING               0
#define SERIALUSB_CMD_PASSO                 1
#define SERIALUSB_CMD_RUN                   2
#define SERIALUSB_CMD_STOP                  3
#define SERIALUSB_CMD_RESET                 4

namespace mySerial
{
    void setup();

    void SerialESPs(void *param);

    void sendData(mySerial::DataPacket);
    void sendMsgOk(int msgItem_ID);

    bool msgsAvailable();

    mySerial::DataPacket receiveMsg();

    uint8_t getErrorStatus();

    uint8_t processSerialUSB();
    
    bool haveSensors();
    mySensors::Sensor getSensor(int n);
}

#endif
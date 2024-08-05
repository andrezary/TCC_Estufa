#ifndef MYSERIALT_H
#define MYSERIAL_H
#include <stdint.h>

#include "common.h"
#include "DataPacket.h"

#define TIME_TO_INIT            5000
#define WAIT_TIME               100
#define TIMEOUT_SERIAL          10000
#define TIME_TO_RETRY           250


#define threadDelay(timeInMS)   vTaskDelay(pdMS_TO_TICKS(timeInMS))

namespace mySerial
{
    void setup();
    void SerialESPs(void* param);
    void sendData(DataPacket);
    void sendMsgOk(int msgItem_ID);

    bool msgsAvailabe();

    DataPacket receiveMsg();
}
#endif
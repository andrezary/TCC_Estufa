#ifndef MYSERIALT_H
#define MYSERIAL_H
#include <stdint.h>

#include "common.h"
#include "DataPacket.h"

//ID_Msg
#define INIT_DATA               0
#define END_DATA                1
#define MSG_OK                  2
#define INIT_SYSTEM             3
#define RESTART_SYSTEM          4
#define I_AM_ALIVE              5
#define MASTER_ALIVE            6
#define HORARIO_SYNC            7
#define SENSOR_SIGNAL           8
#define ATUADOR_CHANGED         9
#define CONFIG_MSG              10
#define MSG_ERROR               11

//MsgType
#define I_AM_DATALOGGER         2
#define I_AM_CONTROLLER         1

#define TIME_TO_INIT            5000
#define WAIT_TIME               100
#define TIMEOUT_SERIAL          10000
#define TIME_TO_RETRY           250


#define threadDelay(timeInMS)   vTaskDelay(pdMS_TO_TICKS(timeInMS))

#define PIN_RX                  16
#define PIN_TX                  17

namespace mySerial
{
    void setup();
    void SerialESPs(void* param);
    void sendData(DataPacket);
    void sendMsgOk(int msgItem_ID);
}
#endif
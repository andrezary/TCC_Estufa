#include <HardwareSerial.h>
#include <Arduino.h>

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

#define POOL_SIZE               50

#define TIME_TO_INIT            5000
#define WAIT_TIME               100
#define TIMEOUT_SERIAL          10000
#define TIME_TO_RETRY           250

//Error codes
#define NO_ERROR                0
#define ERROR_NOT_CONTROLLER    1

#define TAM_ARRAY_NOMES 33




class MsgData
{
  public:
  float value;
  char strValue[TAM_ARRAY_NOMES];
  uint8_t ID_Msg;
  uint8_t MsgType;

  MsgData();
  MsgData(uint8_t id, uint8_t type, float value, const char* str);

  void clear();
};

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


MsgData::MsgData()
: ID_Msg(0), MsgType(0), value(0)
{
  memset(strValue, 0, sizeof(strValue));
}

MsgData::MsgData(uint8_t id, uint8_t type, float value, const char* str)
: ID_Msg(id), MsgType(type), value(value)
{
  memset(strValue, 0, sizeof(strValue));
  strncpy(strValue, str, TAM_ARRAY_NOMES - 1);
}

void MsgData::clear()
{
  memset(strValue, 0, sizeof(strValue));
  ID_Msg = 0;
  MsgType = 0;
  value = 0;
}


void DataPacket::calcChecksum()
{
  uint8_t* p = (uint8_t*) (&this->msg);

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
: msg(id, msgType, value, str)
{
  this->calcChecksum();
}


void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  delay(500);
}

DataPacket packet;

void loop() {
  
      if(Serial2.available() >= sizeof(DataPacket))
      {
        Serial2.readBytes((char*)&packet, sizeof(packet));
        Serial.print("sizeof packet:");
        Serial.println(sizeof(packet));
        Serial.print("Código da mensagem: ");
        Serial.println(packet.msg.ID_Msg);
        Serial.print("Tipo da mensagem: ");
        Serial.println(packet.msg.MsgType);
        Serial.print("Valor da mensagem: ");
        Serial.println(packet.msg.value);
        Serial.print("string: ");
        Serial.println(packet.msg.strValue);
        Serial.print("checksum: ");
        Serial.println(packet.checksum);

        
      }
      else{
        Serial.println("aguardando msg ainda");
        
      }
  delay(1000);
}

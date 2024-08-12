#include <HardwareSerial.h>
#include <Arduino.h>

//ID_Msg
#define BLANK_MSG               0
#define INIT_DATA               1
#define END_DATA                2
#define MSG_OK                  3
#define INIT_SYSTEM             4
#define RESTART_SYSTEM          5
#define I_AM_ALIVE              6
#define MASTER_ALIVE            7
#define HORARIO_SYNC            8
#define SENSOR_SIGNAL           9
#define ATUADOR_CHANGED         10
#define CONFIG_MSG              11
#define MSG_ERROR               12

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
: msg(id, msgType, value, str), checksum(0)
{
  this->calcChecksum();
}


void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  delay(500);
}

DataPacket packet;
bool fun = false;
void loop() {
  
  if(Serial2.available() >= sizeof(DataPacket))
  {
    size_t tamanho = Serial2.readBytes((char*)&packet, sizeof(packet));
    
    Serial.print("size da leitura: ");
    Serial.println(tamanho);
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
    Serial.println("----------------------------");
    if(Serial.available())
    {
        String str = Serial.readStringUntil('\n');

        str.trim();
        str.toLowerCase();

        if(str == "passo")
        {
            fun = !fun;
        }
    }
    if(!fun)
    {
      packet = DataPacket(0, BLANK_MSG, 0, "\0");
      delay(1000);
      return;
    }
    if(packet.msg.MsgType == INIT_SYSTEM && packet.msg.value == I_AM_DATALOGGER)
    {
      packet = DataPacket(1, MSG_OK, 0, "\0");
      Serial2.write((uint8_t*)&packet,sizeof(packet));
      Serial.println("Enviado o ok para o pacote recebido");
      packet = DataPacket(0, INIT_SYSTEM, I_AM_CONTROLLER, "teste");
      Serial2.write((uint8_t*)& packet, sizeof(packet));
      Serial.println("Enviado o packet com o meu initSystem");
      
    }
    else if(packet.msg.MsgType == MSG_OK && packet.msg.value == 0){
      Serial.println("Recebi o Ok da mensagem que eu enviei");
    }
    else if(packet.msg.MsgType == CONFIG_MSG)
    {
      Serial.println("Recebida a msg de config");
      String str("teste config");
      if(str.compareTo(packet.msg.strValue)  == 0)
      {
        packet = DataPacket(packet.msg.ID_Msg, MSG_OK, 0, str.c_str());
        Serial2.write((uint8_t*)& packet, sizeof(packet));
        Serial.println("Enviado o MSG OK");

        packet = DataPacket(5, CONFIG_MSG, 0, str.c_str());
        Serial2.write((uint8_t*)& packet, sizeof(packet));
        Serial.println("Enviando a msg config minha!");
      }
    }
    else if(packet.msg.MsgType == I_AM_ALIVE)
    {
      packet = DataPacket(packet.msg.ID_Msg + 1, MSG_OK, packet.msg.ID_Msg, "\0");
      Serial2.write((uint8_t*)& packet, sizeof(packet));
      Serial.println("Enviado a resposta para o I_AM_ALIVE");
    }
  }
  packet = DataPacket(0, BLANK_MSG, 0, "\0");
  delay(1000);
}

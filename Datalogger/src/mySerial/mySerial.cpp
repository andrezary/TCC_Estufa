#include<freertos/FreeRTOS.h>

#include "common.h"
#include "myConfig/Config.h"
#include "myConfig/myConfig.h"
#include "mySerial/mySerial.h"
#include "mySerial/MsgData.h"
#define MYSERIAL
#include "mySerial/Status.h"
#include "mySerial/DataPacket.h"

extern bool Run;

namespace mySerial
{
    //Variaveis para trabalho
    mySerial::Status status;
    bool debug_retry = false;
    
    void setup()
    {
        Serial.begin(115200);
        Serial2.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);
        myPrintln("Serial UART iniciada.", THREAD_MAIN);
        status.error = CONNECTING_ERROR;
        xTaskCreate(
            SerialESPs,
            "SerialESPs",
            2048,
            NULL,
            1,
            NULL
        );
        myPrintln("ThreadSerial lançada.", THREAD_MAIN);
    }

    void sendData(mySerial::DataPacket param)
    {
        Serial2.write((uint8_t*)&param, sizeof(mySerial::DataPacket));               
        status.pushMsgSended(param.msg);
    }

    void sendMsgOk(int msgItem_ID)
    {
        threadDelay(TIME_DEBUG);
    }

    void sendMsgOk(mySerial::DataPacket param)
    {
        mySerial::DataPacket packet(MSG_OK, param.msg.ID_Msg, "\0");

        sendData(packet);
    }

    void loop()
    {
        //DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, configs::config.getColheita().c_str());
        mySerial::DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str());
        
        
        if(!status.hasInitiated())
        {
            myPrintln("Tentando Handshake Serial", THREAD_SERIAL);
            status.reset(); //Reseta todos os pools para caso seja um reinicio

            sendData(mySerial::DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str())); //Envia o Packet

            unsigned long ellapsed = 0;
            unsigned long time = millis();//Começa a contar o tempo de timeout para iniciar a comunicação
            bool retried = false;
            String msg;
            //Aguarda retorno do controlador ou até estourar o tempo de timeout
            while(ellapsed < (MYSERIAL_TIME_TO_INIT))
            {
                /*msg = "Aguardando retorno da contraparte, ellapsed:" + ellapsed;
                myPrintln(msg.c_str(), THREAD_SERIAL);*/
                
                if(status.error != CONNECTING_ERROR)//Se algo deu errado, termina a espera
                {
                    myPrintln("Aconteceu algum erro durante o handshake!", THREAD_SERIAL);
                    break;
                }
                else if(status.hasInitiated())
                {
                    myPrintln("Troca de msgs incial realizada já! breaking loop...", THREAD_SERIAL);
                    break;
                }
                else if(msgsAvailable()) //Aguarda até ter alguma mensagem para ler
                {
                    packet = receiveMsg();
                    
                    if(packet.isChecksumOK()) //Se a msg não conter erros, prossiga 
                    {
                        myPrintln("Checksum ok! Interpretando!", THREAD_SERIAL);
                        if(packet.msg.value == 0  &&
                            packet.msg.MsgType == MSG_OK) //Se for o retorno da mensagem de inicio sistema
                        {
                            status.pushMsgReceived(packet.msg);
                            status.initInterpreted(packet.msg);
                            myPrintln("Recebido um MSG_OK", THREAD_SERIAL);
                        }
                        else if(packet.msg.value == 0  &&
                            packet.msg.MsgType == MSG_ERROR) //Se for um erro, reenvia o Init_System
                        {
                            sendData(mySerial::DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str())); //Envia o Packet
                        }
                        //Se for a mensagem se identificando como controlador
                        else if(packet.msg.ID_Msg == 0 &&
                            packet.msg.MsgType == INIT_SYSTEM &&
                            packet.msg.value == I_AM_CONTROLLER)
                        {
                            status.pushMsgReceived(packet.msg);//marca o recebimento
                            status.initInterpreted(packet.msg);
                            myPrintln("Recebido um initsystem de um controlador", THREAD_SERIAL);
                            sendMsgOk(packet);
                        }

                        //Se não for um controlador
                        else if(packet.msg.ID_Msg == 0 &&
                            packet.msg.MsgType == INIT_SYSTEM &&
                            packet.msg.value != I_AM_CONTROLLER)
                        {
                            status.error = ERROR_NOT_CONTROLLER; //marca o erro
                        }
                    }
                }
                
                ellapsed = millis() - time;
                if(ellapsed >= (MYSERIAL_TIME_TO_INIT/2) && !retried)
                {
                    sendData(mySerial::DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str())); //Envia o Packet
                    retried = true;
                }
                threadDelay(MYSERIAL_TIME_LOOPS);
            }
            if(ellapsed >= MYSERIAL_TIME_TO_INIT)
            {
                myPrintln("timeout da serial....", THREAD_SERIAL);
                status.error = ERROR_INIT_ERROR;
            }
        }
        else if(status.hasInitiated() && !status.correlated)
        {
            myPrintln("Agora tem que verificar a correlação", THREAD_SERIAL);
            sendData(mySerial::DataPacket(CONFIG_MSG, String("teste config").c_str()));

            unsigned long ellapsed = 0;
            unsigned long retry = 0;
            unsigned long time = millis();//Começa a contar o tempo de timeout para iniciar a comunicação
            retry = time;

            while(ellapsed < MYSERIAL_TIMEOUT_SERIAL)
            {
                if(msgsAvailable())
                {
                    packet = receiveMsg();
                    if(packet.isChecksumOK())
                    {
                        myPrintln("Checksum ok!", THREAD_SERIAL);
                        if(packet.msg.MsgType == CONFIG_MSG)
                        {
                            myPrintln("Config_MSG", THREAD_SERIAL);
                        }
                        else{
                            myPrintln("Outra msg", THREAD_SERIAL);
                        }
                        if((packet.msg.MsgType == CONFIG_MSG) && String("teste config").compareTo(packet.msg.strValue) == 0)
                        {
                            status.correlated = true;
                            myPrintln("Comunicação serial iniciada!", THREAD_SERIAL);
                            status.pushMsgReceived(packet.msg);
                            sendMsgOk(packet);
                            status.lastTimeAlive = millis();
                            status.error = NO_ERROR;
                            break;
                        }
                    }
                }
                threadDelay(10);
            }
            if(ellapsed >= MYSERIAL_TIMEOUT_SERIAL)
            {
                myPrintln("timeout da serial....", THREAD_SERIAL);
                status.error = ERROR_INIT_ERROR;
            }
        }
        else if(status.hasInitiated() && status.correlated)
        {
            if(msgsAvailable())
            {
                packet = receiveMsg();
                myPrintln("Processando msg", THREAD_SERIAL);
                if(packet.isChecksumOK())
                {
                    status.lastTimeAlive = millis();
                    status.sendedAlive = false;
                    switch (packet.msg.MsgType)
                    {
                    case I_AM_ALIVE:
                        sendMsgOk(packet);
                        break;
                    case MSG_OK:
                        myPrintln("Recebido ok!", THREAD_SERIAL);
                        break;
                    case ATUADOR_CHANGED:
                        myPrintln("Recebido uma atualização de atuador", THREAD_SERIAL);
                        sendMsgOk(packet);
                        break;
                    case SENSOR_SIGNAL:
                        myPrintln("Recebido uma atualização de sensor", THREAD_SERIAL);
                        sendMsgOk(packet);
                        break;
                    default:
                        sendData(mySerial::DataPacket(MSG_ERROR, packet.msg.ID_Msg));
                        break;
                    }
                }
                
            }
            else
            {
                unsigned long time = millis();
                String str;
                if((time - status.lastTimeAlive) >= MYSERIAL_TIMEOUT_SERIAL)
                {
                    str = "Status.SendendAlive";
                    str += status.sendedAlive;
                    if(!status.sendedAlive)
                    {
                        sendData(mySerial::DataPacket(I_AM_ALIVE));
                        status.sendedAlive = true;
                        status.lastTimeAlive = time;
                    }
                    else
                    {
                        status.initiated = 0;
                        status.correlated = false;
                        status.error = CONNECTING_ERROR;
                        status.sendedAlive = false;
                        myPrintln("Encerrado o serial por timeout", THREAD_SERIAL);
                    }
                }
            }
        }
        threadDelay(MYSERIAL_TIME_LOOPS);
    }


    void SerialESPs(void* param)
    {
        myPrintln("SerialESPs", THREAD_SERIAL);
        threadDelay(MYSERIAL_TIME_TO_INIT); //Aguarda um tempo para inicialização de si próprio e do irmão
        myPrintln("vTaskDelay", THREAD_SERIAL);
        
        while(true)
        {
            threadDelay(MYSERIAL_TIME_LOOPS);
            if(!myConfig::getStatusRun())
            {
                continue;
            }

            mySerial::loop();
            if(status.error == ERROR_INIT_ERROR)
            {
                myPrintln("Erro ao iniciar a serial com o controlador, tentando novamente em 5min", THREAD_SERIAL);
                threadDelay(MYSERIAL_TIME_TO_RETRY_CONNECTION);
                status.error = CONNECTING_ERROR;
            }
        }
    }

    bool msgsAvailable()
    {
        if(Serial2.available() >= sizeof(mySerial::DataPacket))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    mySerial::DataPacket receiveMsg()
    {
        mySerial::DataPacket packet;
        Serial2.readBytes((uint8_t*)&packet, sizeof(mySerial::DataPacket)); 
        myPrintln("Recebendo msg", THREAD_SERIAL);
        return packet;
    }

    uint8_t getErrorStatus()
    {
        return status.error;
    }

    uint8_t processSerialUSB()
    {
        if(Serial.available())
        {
            String str = Serial.readStringUntil('\n');

            str.trim();
            str.toLowerCase();

            if(str == "passo")
            {
                return SERIALUSB_CMD_PASSO;
            }
            else if(str == "run")
            {
                return SERIALUSB_CMD_RUN;
            }
            else if(str == "stop")
            {
                return SERIALUSB_CMD_STOP;
            }
            else if(str == "reset")
            {
                return SERIALUSB_CMD_RESET;
            }
        }
        return SERIALUSB_CMD_NOTHING;
    }
}
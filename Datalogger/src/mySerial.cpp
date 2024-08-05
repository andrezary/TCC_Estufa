#include<freertos/FreeRTOS.h>

#include "common.h"
#include "mySerial.h"
#include "configs.h"
#include "MsgData.h"
#include "Status.h"
#include "DataPacket.h"

extern bool Run;

namespace mySerial
{
    //Variaveis para trabalho
    Status status;
    
    void setup()
    {
        Serial2.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);
        PRINTLN("Serial UART iniciada!");

        xTaskCreate(
            SerialESPs,
            "SerialESPs",
            2048,
            NULL,
            1,
            NULL
        );
        PRINTLN("Criada a task da Serial");
    }

    void sendData(DataPacket param)
    {
        Serial2.write((uint8_t*)&param, sizeof(DataPacket));
        
        Serial.println("----------------------------");
        PRINTLN("Enviando msg");
        PRINTLN(param.msg.c_str());
        PRINT("sizeof: ");
        PRINTLN(sizeof(param));
        Serial.println("----------------------------");
        status.pushMsgSended(param.msg);
    }

    void sendMsgOk(int msgItem_ID)
    {
        threadDelay(TIME_DEBUG);
    }

    bool debug_retry = false;

    void loop()
    {
        if(!Run) 
            return;
        Run = false;

        //DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, configs::config.getColheita().c_str());
        DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str());
        
        PRINTLN("loop da serial");
        
        if(!status.hasInitiated())
        {
            PRINTLN("Serial não iniciada, iniciando serial");
            status.reset(); //Reseta todos os pools para caso seja um reinicio
            sendData(DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str())); //Envia o Packet

            unsigned long ellapsed = 0;
            unsigned long retry = 0;
            unsigned long time = millis();//Começa a contar o tempo de timeout para iniciar a comunicação
            retry = time;

            //Aguarda retorno do controlador ou até estourar o tempo de timeout
            while(ellapsed < (TIME_TO_INIT*100))
            {
                PRINTLN("Aguardando retorno da contraparte");
                
                if(status.error != NO_ERROR)//Se algo deu errado, termina a espera
                {
                    PRINTLN("Aconteceu algum erro durante o handshake!");
                    break;
                }
                else if(status.hasInitiated())
                {
                    PRINTLN("Troca de msgs incial realizada já! breaking loop...");
                    break;
                }
                else if(msgsAvailabe()) //Aguarda até ter alguma mensagem para ler
                {
                    packet = receiveMsg();
                    
                    if(packet.isChecksumOK()) //Se a msg não conter erros, prossiga 
                    {
                        PRINTLN("Checksum ok! Interpretando!");
                        if(packet.msg.value == 0  &&
                            packet.msg.MsgType == MSG_OK &&
                            packet.msg.value == 0) //Se for o retorno da mensagem de inicio sistema
                        {
                            status.pushMsgReceived(packet.msg);
                            status.initInterpreted(packet.msg);
                            Serial.println("Recebido um MSG_OK");
                        }
                        else if(packet.msg.value == 0  &&
                            packet.msg.MsgType == MSG_OK &&
                            packet.msg.value == 0) //Se for o retorno da mensagem de inicio sistema
                        {
                            status.error = ERROR_INIT_ERROR;
                        }
                        //Se for a mensagem se identificando como controlador
                        else if(packet.msg.ID_Msg == 0 &&
                            packet.msg.MsgType == INIT_SYSTEM &&
                            packet.msg.value == I_AM_CONTROLLER)
                        {
                            status.pushMsgReceived(packet.msg);//marca o recebimento
                            status.initInterpreted(packet.msg);
                            Serial.println("Recebido um initsystem de um controlador");
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
                threadDelay(500);
            }
        }
        else if(status.hasInitiated() && !status.correlated)
        {
            PRINTLN("Agora tem que verificar a correlação");
        }
        else if(status.hasInitiated() && status.correlated)
        {
            
            PRINTLN("Iniciado e correlacionado!");
        }
        PRINTLN("Dando um delay");
        threadDelay(500);
    }

    void SerialESPs(void* param)
    {
        PRINTLN("SerialESPs");
        threadDelay(TIME_TO_INIT); //Aguarda um tempo para inicialização de si próprio e do irmão
        PRINTLN("vTaskDelay");
        while(true)
        {
            mySerial::loop();
            threadDelay(500);
        }
    }

    bool msgsAvailabe()
    {
        if(Serial2.available() >= sizeof(DataPacket))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    DataPacket receiveMsg()
    {
        DataPacket packet;
        Serial2.readBytes((uint8_t*)&packet, sizeof(DataPacket));
        PRINTLN("----------------------------");
        PRINTLN("Recebendo msg");
        PRINTLN(packet.msg.c_str());
        PRINTLN("----------------------------");

        return packet;
    }
}
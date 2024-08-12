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
    bool forceClose = false;
    void setup()
    {
        Serial2.begin(9600, SERIAL_8N1, PIN_RX, PIN_TX);
        PRINTLN("Serial UART iniciada!");
        status.error = CONNECTING_ERROR;
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

    void sendMsgOk(DataPacket param)
    {
        DataPacket packet(MSG_OK, param.msg.ID_Msg, "\0");

        sendData(packet);
    }

    bool debug_retry = false;

    void loop()
    {
        /*if(!Run) 
            return;
        Run = false;*/

        //DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, configs::config.getColheita().c_str());
        DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str());
        
        
        if(!status.hasInitiated())
        {
            PRINTLN("Serial não iniciada, iniciando serial");
            status.reset(); //Reseta todos os pools para caso seja um reinicio
            sendData(DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str())); //Envia o Packet

            unsigned long ellapsed = 0;
            unsigned long time = millis();//Começa a contar o tempo de timeout para iniciar a comunicação
            bool retried = false;
            //Aguarda retorno do controlador ou até estourar o tempo de timeout
            while(ellapsed < (TIME_TO_INIT))
            {
                PRINT("Aguardando retorno da contraparte, ellapsed:");
                PRINTLN(ellapsed);
                if(status.error != CONNECTING_ERROR)//Se algo deu errado, termina a espera
                {
                    PRINTLN("Aconteceu algum erro durante o handshake!");
                    break;
                }
                else if(status.hasInitiated())
                {
                    PRINTLN("Troca de msgs incial realizada já! breaking loop...");
                    break;
                }
                else if(msgsAvailable()) //Aguarda até ter alguma mensagem para ler
                {
                    packet = receiveMsg();
                    
                    if(packet.isChecksumOK()) //Se a msg não conter erros, prossiga 
                    {
                        PRINTLN("Checksum ok! Interpretando!");
                        if(packet.msg.value == 0  &&
                            packet.msg.MsgType == MSG_OK) //Se for o retorno da mensagem de inicio sistema
                        {
                            status.pushMsgReceived(packet.msg);
                            status.initInterpreted(packet.msg);
                            Serial.println("Recebido um MSG_OK");
                        }
                        else if(packet.msg.value == 0  &&
                            packet.msg.MsgType == MSG_ERROR) //Se for um erro, reenvia o Init_System
                        {
                            sendData(DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str())); //Envia o Packet
                        }
                        //Se for a mensagem se identificando como controlador
                        else if(packet.msg.ID_Msg == 0 &&
                            packet.msg.MsgType == INIT_SYSTEM &&
                            packet.msg.value == I_AM_CONTROLLER)
                        {
                            status.pushMsgReceived(packet.msg);//marca o recebimento
                            status.initInterpreted(packet.msg);
                            Serial.println("Recebido um initsystem de um controlador");
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
                if(ellapsed >= (TIME_TO_INIT/2) && !retried)
                {
                    sendData(DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str())); //Envia o Packet
                    retried = true;
                }
                threadDelay(500);
            }
            if(ellapsed >= TIME_TO_INIT)
            {
                PRINTLN("timeout da serial....");
                status.error = ERROR_INIT_ERROR;
            }
        }
        else if(status.hasInitiated() && !status.correlated)
        {
            PRINTLN("Agora tem que verificar a correlação");
            sendData(DataPacket(CONFIG_MSG, String("teste config").c_str()));

            unsigned long ellapsed = 0;
            unsigned long retry = 0;
            unsigned long time = millis();//Começa a contar o tempo de timeout para iniciar a comunicação
            retry = time;

            while(ellapsed < TIMEOUT_SERIAL)
            {
                if(msgsAvailable())
                {
                    packet = receiveMsg();
                    if(packet.isChecksumOK())
                    {
                        PRINTLN("Checksum ok!");
                        if(packet.msg.MsgType == CONFIG_MSG)
                        {
                            PRINTLN("Config_MSG");
                        }
                        else{
                            PRINTLN("Outra msg");
                        }
                        if((packet.msg.MsgType == CONFIG_MSG) && String("teste config").compareTo(packet.msg.strValue) == 0)
                        {
                            status.correlated = true;
                            Serial.println("Correlacionados");
                            PRINTLN("Comunicação serial iniciada!");
                            status.pushMsgReceived(packet.msg);
                            sendMsgOk(packet);
                            status.lastTimeAlive = millis();
                            status.error = NO_ERROR;
                            break;
                        }
                    }
                }
            }
            if(ellapsed >= TIMEOUT_SERIAL)
            {
                PRINTLN("timeout da serial....");
                status.error = ERROR_INIT_ERROR;
            }
        }
        else if(status.hasInitiated() && status.correlated)
        {
            PRINT("Last Time alive: ");
            PRINTLN(status.lastTimeAlive);
            PRINT("actual time: ");
            PRINTLN(millis());
            PRINT("delta: ");
            PRINTLN((millis() - status.lastTimeAlive));
            
            if(msgsAvailable())
            {
                packet = receiveMsg();
                PRINTLN("Processando msg");
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
                        PRINTLN("Recebido ok!");
                        break;
                    default:
                        sendData(DataPacket(MSG_ERROR, packet.msg.ID_Msg));
                        break;
                    }
                }
                
            }
            else
            {
                unsigned long time = millis();

                if((time - status.lastTimeAlive) >= TIMEOUT_SERIAL)
                {
                    PRINT("Status.SendendAlive");
                    PRINTLN(status.sendedAlive);
                    if(!status.sendedAlive)
                    {
                        sendData(DataPacket(I_AM_ALIVE));
                        status.sendedAlive = true;
                        status.lastTimeAlive = time;
                    }
                    else
                    {
                        status.initiated = 0;
                        status.correlated = false;
                        status.error = CONNECTING_ERROR;
                        status.sendedAlive = false;
                        Serial.println("Encerrado o serial por timeout");
                    }
                }
            }
        }
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
            if(status.error == ERROR_INIT_ERROR)
            {
                PRINTLN("Erro ao iniciar a serial com o controlador, tentando novamente em 5min");
                threadDelay(300000);
                status.error = CONNECTING_ERROR;
            }
        }
    }

    bool msgsAvailable()
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

    uint8_t getErrorStatus()
    {
        return status.error;
    }
}
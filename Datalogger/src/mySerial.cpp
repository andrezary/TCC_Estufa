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
        PRINTLN("SendData()");
        Serial2.write((uint8_t*)&param, sizeof(DataPacket));
        PRINT("checksum da msg a enviar:");
        PRINTLN(param.checksum);
        threadDelay(500);
    }

    void sendMsgOk(int msgItem_ID)
    {
        PRINTLN("SendMsgOK!");
        threadDelay(TIME_DEBUG);
    }

    bool debug_retry = false;
    void loop()
    {
        if(!Run) return;
        Run = false;
        //DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, configs::config.getColheita().c_str());
        DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str());
        
        PRINTLN("loop da serial");
        //return;
        if(!status.initiated)
        {
            PRINTLN("Serial não iniciada, iniciando serial");
            //Reseta todos os pools para caso seja um reinicio
            status.reset();
            PRINTLN("SendData packet");
            //Envia o Packet
            PRINT("sizeof(datapacket):");
            PRINTLN(sizeof(packet));
            sendData(DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str()));

            unsigned long ellapsed = 0;
            unsigned long retry = 0;
            unsigned long time = millis();//Começa a contar o tempo de timeout para iniciar a comunicação
            
            //Aguarda retorno do controlador ou até estourar o tempo de timeout
            while(ellapsed < (TIME_TO_INIT*100))
            {
                //return;
                PRINTLN("Aguardando retorno da contraparte");
                if(Serial2.available() >= sizeof(DataPacket)) //Aguarda até ter alguma mensagem para ler
                {
                    Serial2.readBytes((uint8_t*)&packet, sizeof(DataPacket));
                    PRINTLN("Lido os bytes");
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
                    if(packet.isChecksumOK()) //Se a msg não conter erros, prossiga 
                    {
                        PRINTLN("Checksum ok! Interpretando!");
                        //Se for o retorno da mensagem de inicio sistema
                        if(packet.msg.ID_Msg == 0  &&
                            packet.msg.MsgType == MSG_OK)
                        {
                            status.isReceived[0] = 1; //marca o recebimento
                            Serial.println("Recebido um MSG_OK");
                        }

                        //Se for a mensagem se identificando como controlador
                        else if(packet.msg.ID_Msg == 0 &&
                            packet.msg.MsgType == INIT_SYSTEM &&
                            packet.msg.value == I_AM_CONTROLLER)
                        {
                            status.msgsReceived[0] = packet.msg;//marca o recebimento
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

                //Se recebeu o OK da mensagem de INIT e recebeu uma identificação do controlador
                if(status.isReceived[0] = 1 && status.msgsReceived[0].MsgType == I_AM_CONTROLLER)
                {
                    status.initiated = true; //Diga que a conexão foi iniciada
                    break;
                }
                //Se algo deu errado, termina a espera
                else if(status.error != NO_ERROR)
                {
                    break;
                }
                
                if((ellapsed - retry) > TIME_TO_RETRY && 
                    debug_retry == false)
                {
                    PRINTLN("TIME OUT");
                    retry = ellapsed;
                    debug_retry = true;
                    //sendData(DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, configs::config.getColheita().c_str()));
                    sendData(DataPacket(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str()));
                }

                ellapsed = millis() - time;
                threadDelay(500);
            }
        }
        else if(status.initiated && !status.correlated)
        {
            //Se a colheita informada pelo controlador e a armazenada no datalogger forem iguais
            //if(configs::config.getColheita().compareTo(status.msgsReceived[0].strValue) == 0)
            if((String("teste").compareTo(status.msgsReceived[0].strValue)) == 0)
            {
                status.correlated = true;
            }
            else
            {
                status.correlated = false;

            }
        }
        else if(status.initiated)
        {
            PRINTLN("Serial iniciada");
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
            //PRINTLN("Entrando no loop Serial!");
            mySerial::loop();
            //PRINTLN("Delay pos loop Serial");
            threadDelay(500);
        }
    }
}
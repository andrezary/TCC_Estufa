#include <HardwareSerial.h>
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
    HardwareSerial SerialUART(1);
    Status status;
    
    void setup()
    {
        SerialUART.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX);
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

    void sendData(DataPacket)
    {
        PRINTLN("SendData()");
        threadDelay(500);
    }

    void sendMsgOk(int msgItem_ID)
    {
        PRINTLN("SendMsgOK!");
        threadDelay(TIME_DEBUG);
    }

    void loop()
    {
        if(!Run) return;
        Run = false;
        //DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, configs::config.getColheita().c_str());
        DataPacket packet(0, INIT_SYSTEM, I_AM_DATALOGGER, String("teste").c_str());
        PRINTLN("loop da serial");
        
        if(!status.initiated)
        {
            PRINTLN("Serial não iniciada, iniciando serial");
            //Reseta todos os pools para caso seja um reinicio
            status.reset();

            //Envia o Packet
            sendData(packet);

            unsigned long ellapsed = 0;
            unsigned long retry = 0;
            unsigned long time = millis();//Começa a contar o tempo de timeout para iniciar a comunicação
            
            //Aguarda retorno do controlador ou até estourar o tempo de timeout
            while(ellapsed < TIME_TO_INIT)
            {
                PRINTLN("Aguardando retorno da contraparte");
                if(SerialUART.available()) //Aguarda até ter alguma mensagem para ler
                {
                    SerialUART.readBytes((uint8_t*)&packet, sizeof(DataPacket));

                    if(packet.isChecksumOK()) //Se a msg não conter erros, prossiga 
                    {
                        //Se for o retorno da mensagem de inicio sistema
                        if(packet.msg.ID_Msg == 0  &&
                            packet.msg.MsgType == MSG_OK)
                        {
                            status.isReceived[0] = 1; //marca o recebimento
                        }

                        //Se for a mensagem se identificando como controlador
                        else if(packet.msg.ID_Msg == 0 &&
                            packet.msg.MsgType == INIT_SYSTEM &&
                            packet.msg.value == I_AM_CONTROLLER)
                        {
                            status.msgsReceived[0] = packet.msg;//marca o recebimento
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
                
                if((ellapsed - retry) > TIME_TO_RETRY)
                {
                    PRINTLN("TIME OUT");
                    retry = ellapsed;
                    
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
            mySerial::loop();
            threadDelay(500);
        }
    }
}
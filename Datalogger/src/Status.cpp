#include "common.h"
#include "Status.h"

Status::Status()
:poolSended(0), poolReceived(0)
{
    memset(this->isReceived, 0, sizeof(this->isReceived));
    this->correlated = false;
    this->initiated = false;
    this->error = NO_ERROR;
}

void Status::reset()
{
    for(int i = 0; i < POOL_SIZE ;i++)
    {
        this->isReceived[i] = 0;
        this->msgsSended[i].clear();
        this->msgsReceived[i].clear();
    }

    this->correlated = false;
    this->initiated = false;
    this->error = NO_ERROR;
    this->poolReceived = 0;
    this->poolSended = 0;
}

void Status::incrementPoolSended()
{
    this->poolSended++;
    if(this->poolSended > (POOL_SIZE - 1))
    {
        this->poolSended = 0;
    }
}

void Status::incrementPoolReceived()
{
    this->poolReceived++;
    if(this->poolReceived > (POOL_SIZE - 1))
    {
        this->poolReceived = 0;
    }
}

void Status::pushMsgSended(MsgData msg)
{
    this->msgsSended[poolSended] = msg;
    this->isReceived[poolSended] = 0;
    incrementPoolSended();
}

void Status::pushMsgReceived(MsgData msg)
{
    if(msg.MsgType == MSG_OK)
    {
        for(int i = 0; i < POOL_SIZE ;i++)
        {
            if(this->msgsSended[i] == msg)
            {
                isReceived[i] = MSG_OK;
            }
        }
    }
    else 
    {
        this->msgsReceived[poolReceived] = msg;
        incrementPoolReceived();
    }
}

void Status::initInterpreted(MsgData msg)
{
    if(msg.MsgType == MSG_OK)
    {
        this->initiated |= MY_INIT_OK;
    }
    else if(msg.MsgType == INIT_SYSTEM)
    {
        this->initiated |= FRIEND_INIT_OK;
    }
}

bool Status::hasInitiated()
{
    if(this->initiated == ALL_INIT_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Status::verifyMsgSended(MsgData msg)
{

}

bool Status::verifyMsgReceived(MsgData msg)
{

}
#include "common.h"
#include "Status.h"

Status::Status()
{
    memset(this->isReceived, 0, sizeof(this->isReceived));
    correlated = false;
    initiated = false;
    error = NO_ERROR;
}

void Status::reset()
{
    for(int i = 0; i < POOL_SIZE ;i++)
    {
        isReceived[i] = 0;
        pool[i].clear();
        msgsReceived[i].clear();
    }
    correlated = false;
    initiated = false;
    error = NO_ERROR;
}
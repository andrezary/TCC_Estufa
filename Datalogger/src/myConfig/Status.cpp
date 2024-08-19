#include "Status.h"

namespace myConfig
{
    Status::Status()
    : run(false)
    {

    }

    bool Status::getRun()
    {
        return this->run;
    }
    void Status::setRun(bool param)
    {
        this->run = param;
    }
}
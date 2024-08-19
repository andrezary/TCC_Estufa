#ifndef STATUS_H
#define STATUS_H

#pragma once

namespace myConfig
{
    class Status
    {
    public:
        Status();

        bool getRun();
        void setRun(bool param);

    private:
        bool run;

    };

}

#endif
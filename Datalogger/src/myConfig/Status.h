#ifndef STATUS_H
#define STATUS_H

    #ifndef MYCONFIG
        #error "Status do myConfig está sendo puxado em outro arquivo."
    #endif
    
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
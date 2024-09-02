#ifndef MYCONFIG_H
#define MYCONFIG_H

#pragma once

#define THREAD_LED_TIME_DELAY   300

namespace myConfig
{
    void processStatusLEDS(void*);
    void setup();
    void setStatusRun(bool);
    bool getStatusRun();

}
#endif
#include "common.h"

#include <Arduino.h>
#include <time.h>

String getHora(bool full)
{
    struct tm timeinfo;

    getLocalTime(&timeinfo);

    char strTime[9];
    if(!full)
        strftime(strTime, sizeof(strTime), "%H:%M", &timeinfo);
    else
        strftime(strTime, sizeof(strTime), "%H:%M:%S", &timeinfo);
    
    return String(strTime);
}

String getData(bool formatoHTML)
{
    struct tm timeinfo;

    getLocalTime(&timeinfo);

    char strDate[11];
    if(!formatoHTML)
        strftime(strDate, sizeof(strDate), "%d/%m/%Y", &timeinfo);
    else
        strftime(strDate, sizeof(strDate), "%Y-%m-%d", &timeinfo);
    return String(strDate);
}
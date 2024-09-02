#ifndef MYSENSORS_H
#define MYSENSORS_H

#include <mySensors/Sensor.h>

#pragma once

#define MYSENSORS_MAX_SENSORES      12

#define MYSENSORS_DELAY_SETUP  300
#define MYSENSORS_NO_ERROR          0
#define MYSENSORS_CONFIG_NOT_FOUND  1

namespace mySensors
{

    bool tryReadSensorConfigFile();

}

#endif
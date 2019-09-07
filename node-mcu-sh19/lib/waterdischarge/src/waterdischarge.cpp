#include <Arduino.h>
#include "waterdischarge.h"

Waterdischarge::Waterdischarge()
{
}

void Waterdischarge::init(byte sensID, uint32_t loadS, byte dp)
{
    Waterdischarge::pin = dp;
    pinMode(pin, INPUT_PULLDOWN_16);
    Waterdischarge::load_s = loadS;
    Waterdischarge::sensorID = sensID;
}

uint32_t Waterdischarge::discharge()
{    
    return Waterdischarge::load_s;
}

uint8_t Waterdischarge::get_sensID()
{
    return this->sensorID;
}

uint8_t Waterdischarge::get_classID()
{
    return this->classID;
}
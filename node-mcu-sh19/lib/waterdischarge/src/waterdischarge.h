#ifndef WATERDISCHARGE_h
#define WATERDISCHARGE_h

#include "Arduino.h"

class Waterdischarge
{
private:
    byte pin;
    uint32_t load_s;
    byte classID = 1;
    byte sensorID;

public:
    Waterdischarge();
    void init(byte sensID, uint32_t loadS, byte dp);
    uint32_t discharge();
    uint8_t get_sensID();
    uint8_t get_classID();
};

#endif
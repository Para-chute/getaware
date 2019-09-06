#ifndef WATERDISCHARGE_h
#define WATERDISCHARGE_h

#include "Arduino.h"

class Waterdischarge
{
private:
    byte pin;
    int data;

public:
    Waterdischarge();
    void init(byte dp);
    int discharge();
};

#endif
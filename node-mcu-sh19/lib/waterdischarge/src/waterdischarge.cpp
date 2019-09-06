#include <Arduino.h>
#include "waterdischarge.h"

Waterdischarge::Waterdischarge()
{
}

void Waterdischarge::init(byte dp)
{
    Waterdischarge::pin = dp;
    pinMode(pin, INPUT);
    Waterdischarge::data = 0;
}

int Waterdischarge::discharge()
{
    if (digitalRead(pin))
    {
        Waterdischarge::data = 20;
    }
    else
    {
        Waterdischarge::data = 0;
    }
    return Waterdischarge::data;
}

#ifndef WATERFLOW_h
#define WATERFLOW_h

#include "Arduino.h"

class Waterflow
{
private:
    byte pin;
    int data;

public:
    Waterflow();
    void init(byte anp);
    void update_data();
    int return_data();
};

#endif
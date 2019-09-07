#ifndef WATERFLOW_h
#define WATERFLOW_h

#include "Arduino.h"

class Waterflow
{
private:
    byte pin;
    byte classID = 0;
    byte sensorID;
    int data;

public:
    Waterflow();
    void init(byte sensID, byte anp);
    void update_data();
    int return_data();
    uint8_t get_sensID();
    uint8_t get_classID();
};

#endif
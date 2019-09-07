#include <Arduino.h>
#include "waterflow.h"

Waterflow::Waterflow()
{
}

void Waterflow::init(byte sensID, byte anp)
{
    Waterflow::sensorID = sensID;
    Waterflow::pin = anp;

    pinMode(pin, INPUT);

    Waterflow::data = 0;
}

//this function must read the information from the adc or input state
void Waterflow::update_data()
{
    Waterflow::data = analogRead(pin);
}
//return the data readed from the input Waterflow
int Waterflow::return_data()
{
    Waterflow::update_data();
    return Waterflow::data;
}

uint8_t Waterflow::get_sensID()
{
    return this->sensorID;
}

uint8_t Waterflow::get_classID()
{
    return this->classID;
}

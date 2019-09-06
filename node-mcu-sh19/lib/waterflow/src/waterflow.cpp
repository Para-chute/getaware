#include <Arduino.h>
#include "waterflow.h"

Waterflow::Waterflow()
{
}

void Waterflow::init(byte anp)
{

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

#ifndef PUBLISHERSH_H
#define PUBLISHERSH_H

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const size_t capacity = JSON_OBJECT_SIZE(3+70);

class PublisherSH
{

public:
    void begin(char *uuid, char *endpoint);
    void sendData(char *uuid, uint32_t measurement);
    void setupSensorClass(char *uuid, uint8_t sensorClass);
private:
    StaticJsonDocument<capacity> doc;
    char *endpoint;
};

#endif



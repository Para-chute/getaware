#ifndef PUBLISHERSH_H
#define PUBLISHERSH_H

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const size_t capacity = JSON_OBJECT_SIZE(2);

class PublisherSH
{

public:
    void begin(char *uuid, char *endpoint);
    void sendData(uint8_t measurement);
private:
    StaticJsonDocument<capacity> doc;
    char *endpoint;
};

#endif



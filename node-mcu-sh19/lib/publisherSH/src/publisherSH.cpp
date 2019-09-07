#include "publisherSH.h"


void PublisherSH::begin(char *uuid, char *endpoint)
{
    PublisherSH::doc["sensor"] = uuid; // Specify sensor uuid in json document
    Serial.printf("\nPublisher configured with uuid %s", uuid);
    PublisherSH::endpoint = endpoint;  // Specify endpoint
    Serial.printf("\nWill publish on %s", endpoint);
}

void PublisherSH::sendData(uint8_t sensorId, uint32_t measurement)
{
    PublisherSH::doc["sensorId"] = sensorId;
    PublisherSH::doc["value"] = measurement;

    char output[128];
    serializeJson(PublisherSH::doc, output);

    HTTPClient http;                                    // Send the request
    http.begin(endpoint);                               // Specify request destination
    http.addHeader("Content-Type", "application/json"); // Specify content-type header
    int httpCode = http.POST(output);                   // POST message
    http.end();                                         // Close connection

    // Print return code and published message
    Serial.printf("\nCode %d : %s", httpCode, output);
}

void PublisherSH::setupSensorClass(uint8_t sensorId, uint8_t sensorClass)
{
    PublisherSH::doc["sensorId"] = sensorId;
    PublisherSH::doc["sensorClass"] = sensorClass;

    char output[128];
    serializeJson(PublisherSH::doc, output);

    HTTPClient http;                                    // Send the request
    http.begin(endpoint);                               // Specify request destination
    http.addHeader("Content-Type", "application/json"); // Specify content-type header
    int httpCode = http.POST(output);                   // POST message
    http.end();                                         // Close connection

    Serial.printf("\nCode %d : %s", httpCode, output);
}

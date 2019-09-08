#include "publisherSH.h"


void PublisherSH::begin(char *uuid, char *endpoint)
{
    PublisherSH::doc["sensor"] = uuid; // Specify sensor uuid in json document
    Serial.printf("\nPublisher configured with uuid %s", uuid);
    PublisherSH::endpoint = endpoint;  // Specify endpoint
    Serial.printf("\nWill publish on %s", endpoint);
}

void PublisherSH::sendData(char *uuid, uint32_t measurement, char *eventalias)
{
    PublisherSH::doc["uuid"] = uuid;
    PublisherSH::doc["value"] = measurement;
    PublisherSH::doc["eventalias"] = eventalias;

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

void PublisherSH::setupSensorClass(char *uuid, uint8_t sensorClass, char *eventalias)
{
    PublisherSH::doc["uuid"] = uuid;
    PublisherSH::doc["sensorClass"] = sensorClass;
    PublisherSH::doc["eventalias"] = eventalias;

    char output[128];
    serializeJson(PublisherSH::doc, output);
    PublisherSH::doc.remove('sensorClass');
    HTTPClient http;                                    // Send the request
    http.begin(endpoint);                               // Specify request destination
    http.addHeader("Content-Type", "application/json"); // Specify content-type header
    int httpCode = http.POST(output);                   // POST message
    http.end();                                         // Close connection

    Serial.printf("\nCode %d : %s", httpCode, output);
}

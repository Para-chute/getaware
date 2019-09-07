#include <Arduino.h>

#include "waterflow.h"
#include "waterdischarge.h"
#include "wifiSH.h"
#include "publisherSH.h"
#include <ESP8266WiFi.h>

/** TEST Class ID:
 * - 1 bit for class ID;
 * - Read pin and identify class;
 * - Do measurements
 * - G E T B I T C H E S
**/

#define CLASS_PIN_0 D1
#define WATERFLOWADC_PIN A0
#define WATERDISCHARGEDIG_PIN D3

uint8_t classID = 0;
Waterflow publisher = Waterflow();
Waterdischarge publisherD = Waterdischarge();

uint8_t total = 0;
uint8_t measure;

#ifndef PLATFORM_HOST
    #define PLATFORM_HOST "192.168.63.237"
    #define PLATFORM_PORT 5001
#endif

char *uuid = "12trouweygfluisd";
char endpoint[80];

PublisherSH waterFlowPublisher = PublisherSH();

void setup() {
    Serial.begin(115200);

    pinMode(CLASS_PIN_0, INPUT);

    classID = digitalRead(CLASS_PIN_0);

    switch (classID)
    {
    case 0:             // waterflow
        publisher.init(WATERFLOWADC_PIN);
        Serial.println("(oJo) my name is  W A T E R F L O W");
        break;
    
    case 1:             // waterdischarge
        publisherD.init(WATERDISCHARGEDIG_PIN);
        Serial.println("(UwU) my name is  W A T E R D I S C H A R G E");
        break;

    default:
        break;
    }

    connect2Wifi();
    sprintf(endpoint, "http://%s:%i/", PLATFORM_HOST, PLATFORM_PORT);
    waterFlowPublisher.begin(uuid,endpoint);
}

void loop() {
    delay(1000);

    switch (classID)
    {
    case 0:             // waterflow
        measure = publisher.return_data();        
        total += measure;
        Serial.printf("(oJo) y o u s p e n t %i w o u r d e r \n    a n d d i s m a n y w o u r d e r %i\n",measure,total);
        waterFlowPublisher.sendData(total);
        break;
    
    case 1:             // waterdischarge
        measure = publisherD.discharge();
        total += measure;
        Serial.printf("(UwU) y o u s p e n t %i w o u r d e r \n    a n d d i s m a n y w o u r d e r %i\n",measure,total);
        waterFlowPublisher.sendData(total);
        break;

    default:
        break;
    }



}
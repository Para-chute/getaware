#include <Arduino.h>

#include "waterflow.h"
#include "waterdischarge.h"
#include "wifiSH.h"
#include "publisherSH.h"

#include <ESP8266WiFi.h>

#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#include "OLED.h"

/** publisher demo:
 * Demo of publisher with class ID and corresponding measurement
**/

// ANALOG INITS
#define WATERFLOWADC_PIN A0 // ADC read pin
#define WATERDISCHARGEDIG_PIN D8
#define CLASSID_PIN D3

Waterflow waterflowS = Waterflow();
Waterdischarge waterdischargeS = Waterdischarge();

uint8 sensID = 42; // Unit ID

uint32_t total = 0; // Total water spent in utilization
uint32_t measure;   // Instantaneous measurement

// NETWORK INITS
#ifndef PLATFORM_HOST
#define PLATFORM_HOST "192.168.63.237"
#define PLATFORM_PORT 5001
#endif

char *uuid = "heck";
char endpoint[80];

PublisherSH flowPub = PublisherSH();

// OTHER INITS
uint32_t timePerUtilization = 60000;
unsigned long previousT, currentT;

#define DIS_LOAD_S 10

void setup()
{
    Serial.begin(115200);

    connect2Wifi();
    sprintf(endpoint, "http://%s:%i/", PLATFORM_HOST, 5001);
    flowPub.begin(uuid, endpoint);

    pinMode(CLASSID_PIN, INPUT);

    switch (digitalRead(CLASSID_PIN))
    {
    case 0:         //Waterflow
        waterflowS.init(sensID, WATERFLOWADC_PIN);
        flowPub.setupSensorClass(waterflowS.get_sensID(), waterflowS.get_classID()); // Initial send: warn that unit $(sensorID) is measuring $(classID)
        Serial.println("Waterflow");
        break;
    
    case 1:         //Waterdischarge
        waterdischargeS.init(sensID, DIS_LOAD_S, WATERDISCHARGEDIG_PIN);
        flowPub.setupSensorClass(waterdischargeS.get_sensID(), waterdischargeS.get_classID()); // Initial send: warn that unit $(sensorID) is measuring $(classID)
        Serial.println("Waterdischarge");
        break;

    default:
        break;
    }

    initDisplay();

    Serial.println("\nWaterflow sensor is online");
}
void loop() {

    switch (digitalRead(CLASSID_PIN))
    {
    case 0:         //Waterflow
        while (1)
        {
            delay(500);
            measure = waterflowS.return_data();

            if (measure > 50)
            {
                total = 0;
                previousT = millis();

                while (millis() < (previousT + timePerUtilization) && measure > 50)
                {
                total += measure;
                SendDisplay(total);
                measure = waterflowS.return_data();
                Serial.println(total);
                delay(200);
                }

                ClearDisplay();
                flowPub.sendData(waterflowS.get_sensID(), total);                
            }
        }
        break;
    
    case 1:         //Waterdischarge
        while (1)
        {
            if(digitalRead(WATERDISCHARGEDIG_PIN)) {
                total = waterdischargeS.discharge();
                SendDisplay(total);
                ClearDisplay();
                flowPub.sendData(waterdischargeS.get_sensID(), total);
                delay(1000);
            }
            total = 0;
        }
        
        break;

    default:
        break;
    }

  
}
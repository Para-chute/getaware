#include <Arduino.h>

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
#define WATERDISCHARGEDIG_PIN D8

Waterdischarge waterdischargeS = Waterdischarge();

uint8 sensID = 42; // Unit ID

uint32_t total = 0; // Total water spent in utilization

// NETWORK INITS
#ifndef PLATFORM_HOST
#define PLATFORM_HOST "192.168.63.237"
#define PLATFORM_PORT 5001
#endif

char *uuid = "heck";
char endpoint[80];

PublisherSH flowPub = PublisherSH();

// OTHER INITS
#define DIS_LOAD_S 4000

void setup()
{
    Serial.begin(115200);

    connect2Wifi();
    sprintf(endpoint, "http://%s:%i/", PLATFORM_HOST, 5001);
    flowPub.begin(uuid, endpoint);

    waterdischargeS.init(sensID, DIS_LOAD_S, WATERDISCHARGEDIG_PIN);
    flowPub.setupSensorClass(waterdischargeS.get_sensID(), waterdischargeS.get_classID()); // Initial send: warn that unit $(sensorID) is measuring $(classID)

    initDisplay();

    Serial.println("\nWaterdischarge sensor is online");
}
void loop() {

    if(digitalRead(WATERDISCHARGEDIG_PIN)) {
        total = waterdischargeS.discharge();
        SendDisplay(total);        
        flowPub.sendData(waterdischargeS.get_sensID(), total);
        delay(1000);
        total = 0;
        ClearDisplay();
    }
    
}
#include <Arduino.h>

#include "waterflow.h"
#include "wifiSH.h"
#include "publisherSH.h"

#include <ESP8266WiFi.h>

#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#include "OLED.h"

/** Water flow demo:
 * Basic demo for interaction with NODE <-> DB
**/

// ANALOG INITS
#define WATERFLOWADC_PIN A0 // ADC read pin

Waterflow sensorUnit = Waterflow();
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

PublisherSH waterFlowPublisher = PublisherSH();

// OTHER INITS
uint32_t timePerUtilization = 60000;
unsigned long previousT, currentT;

void setup()
{
  Serial.begin(115200);
  sensorUnit.init(sensID, WATERFLOWADC_PIN);

  connect2Wifi();
  sprintf(endpoint, "http://%s:%i/", PLATFORM_HOST, 5001);
  waterFlowPublisher.begin(uuid, endpoint);

  waterFlowPublisher.setupSensorClass(sensorUnit.get_sensID(), sensorUnit.get_classID()); // Initial send: warn that unit $(sensorID) is measuring $(classID)

  initDisplay();

  Serial.println("\nWaterflow sensor is online");
}

void loop()
{

  delay(500);
  measure = sensorUnit.return_data();

  if (measure > 50)
  {
    total = 0;
    previousT = millis();

    while (millis() < (previousT + timePerUtilization) && measure > 50)
    {
      total += measure;
      SendDisplay(total);
      measure = sensorUnit.return_data();
      delay(200);
    }

    ClearDisplay();
    waterFlowPublisher.sendData(sensorUnit.get_sensID(), total);
  }
}

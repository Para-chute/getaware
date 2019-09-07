#include <Arduino.h>

#include "waterflow.h"
#include "wifiSH.h"
#include "publisherSH.h"

#include <ESP8266WiFi.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/** Water flow demo:
 * Basic demo for interaction with NODE <-> DB
**/

// ANALOG INITS
#define WATERFLOWADC_PIN A0                         // ADC read pin

Waterflow sensorUnit = Waterflow();                 
uint8 sensID = 42;                                  // Unit ID

uint32_t total = 0;                                 // Total water spent in utilization
uint32_t measure;                                   // Instantaneous measurement

// NETWORK INITS
#ifndef PLATFORM_HOST
    #define PLATFORM_HOST "192.168.63.237"
    #define PLATFORM_PORT 5001
#endif

char *uuid = "heck";
char endpoint[80];

PublisherSH waterFlowPublisher = PublisherSH();

// ANIMATIONS INIT
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_RESET);

// OTHER INITS
uint32_t timePerUtilization = 60000;
unsigned long previousT, currentT;

void SelectCupSize(uint32_t pos,uint32_t restante)
{
    static const uint8_t PROGMEM copo_vazio [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x10,0x02
,0x10,0x02
,0x18,0x02
,0x08,0x06
,0x08,0x04
,0x08,0x04
,0x08,0x0c
,0x04,0x08
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

static const uint8_t PROGMEM copo_cheio [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x17,0xfe
,0x17,0xfe
,0x1f,0xfe
,0x0b,0xfe
,0x0b,0xf4
,0x0b,0xf4
,0x0b,0xfc
,0x05,0xe8
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

static const uint8_t PROGMEM copo_1_3 [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x10,0x02
,0x10,0x02
,0x18,0x02
,0x08,0x06
,0x08,0x04
,0x09,0xf4
,0x0b,0xfc
,0x05,0xe8
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

static const uint8_t PROGMEM copo_2_3 [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x10,0x02
,0x10,0x02
,0x18,0x02
,0x0b,0xfe
,0x0b,0xf4
,0x0b,0xf4
,0x0b,0xfc
,0x05,0xe8
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

  if(restante<=66)
  {
    display.drawBitmap(pos,0,copo_1_3,16,16,WHITE);
  }
  if(restante >66 && restante <=132)
  {
    display.drawBitmap(pos,0,copo_2_3,16,16,WHITE);
  }
  if(restante> 132 && restante <=200)
  {
    display.drawBitmap(pos,0,copo_cheio,16,16,WHITE);
  }
}

void SendDisplay(uint32_t water)
{
  static const uint8_t PROGMEM copo_vazio [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x10,0x02
,0x10,0x02
,0x18,0x02
,0x08,0x06
,0x08,0x04
,0x08,0x04
,0x08,0x0c
,0x04,0x08
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

static const uint8_t PROGMEM copo_cheio [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x17,0xfe
,0x17,0xfe
,0x1f,0xfe
,0x0b,0xfe
,0x0b,0xf4
,0x0b,0xf4
,0x0b,0xfc
,0x05,0xe8
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

static const uint8_t PROGMEM copo_1_3 [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x10,0x02
,0x10,0x02
,0x18,0x02
,0x08,0x06
,0x08,0x04
,0x09,0xf4
,0x0b,0xfc
,0x05,0xe8
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

static const uint8_t PROGMEM copo_2_3 [] = {
0x00,0x00
,0x00,0x00
,0x00,0x00
,0x00,0x00
,0x10,0x02
,0x10,0x02
,0x18,0x02
,0x0b,0xfe
,0x0b,0xf4
,0x0b,0xf4
,0x0b,0xfc
,0x05,0xe8
,0x04,0x08
,0x07,0xf8
,0x00,0x00
,0x00,0x00
};

    uint32_t pos=0;
    uint32_t  cupfull = 0;
    uint32_t restante = 0;
    uint32_t totalcopos = water/200;
    
    display.clearDisplay();  
    if(water <=1600)
    {
      cupfull= water/200; //copos cheios visiveis
      restante = water%200; //ml de copo não cheio
      if(cupfull!=0)
      {
        for(uint32_t i =0; i<cupfull; i++)
        {
          pos=i*16;
          display.drawBitmap(pos,0,copo_cheio,16,16,WHITE);
        }
        if(restante!=0)
        {
          SelectCupSize(pos+16,restante);
        }
        else
        {
          display.drawBitmap(pos+16,0,copo_vazio,16,16,WHITE);
        }
      }
      else
      {
        if(restante!=0)
        {
          SelectCupSize(0,restante);
        }
        else
        {
          display.drawBitmap(0,0,copo_vazio,16,16,WHITE);
        }
      }
    } 
    else
    {
      uint32_t ml = water%1600; //ml visiveis
      cupfull= ml/200; //copos cheios visiveis
      restante = ml%200; //ml de copo não cheio

      for(uint32_t i =0; i<cupfull; i++)
      {
        pos=i*16;
        display.drawBitmap(pos,0,copo_cheio,16,16,WHITE);
      }
      if(restante!=0)
      {
        SelectCupSize(pos+16,restante);
      }
      else
      {
         display.drawBitmap(pos+16,0,copo_vazio,16,16,WHITE);
      }
    }
    uint32_t litros = water/1000;
    uint32_t  deci= water%1000;
    deci = deci/100;
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,16);
    display.print(litros);
    display.setCursor(19,16);
    display.print(".");
    display.setCursor(32,16);
    display.print(deci);
    display.setCursor(55,16);
    display.print("Litros");
    display.setCursor(0,25);
    display.print(totalcopos);
    display.setCursor(55,25);
    display.print("Copos");
    display.display();
}



void setup() {
    Serial.begin(115200);
    sensorUnit.init(sensID, WATERFLOWADC_PIN);

    connect2Wifi();
    sprintf(endpoint, "http://%s:%i/", PLATFORM_HOST, 5001);
    waterFlowPublisher.begin(uuid,endpoint);

    waterFlowPublisher.setupSensorClass(sensorUnit.get_sensID(), sensorUnit.get_classID());          // Initial send: warn that unit $(sensorID) is measuring $(classID)

    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    Serial.println("\nWaterflow sensor is online");
}

void loop() {

    delay(500);
    measure = sensorUnit.return_data();    

    if(measure > 50) {
        total = 0;
        previousT = millis();

        while (millis() < (previousT + timePerUtilization) && measure > 50)
        {
            total += measure;
            SendDisplay(total);
            measure = sensorUnit.return_data();            
            delay(200);
        }                

        display.clearDisplay();
        waterFlowPublisher.sendData(sensorUnit.get_sensID(), total);
    }
}


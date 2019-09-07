#ifndef OLED_H
#define OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 13

//Select the cup level
void SelectCupSize(uint32_t pos, uint32_t restante);

//Send to the display (Cups animation, Liters and total cups)
void SendDisplay(uint32_t water);
void ClearDisplay();
void initDisplay();

//Adafruit_SSD1306 display(OLED_RESET);

#endif
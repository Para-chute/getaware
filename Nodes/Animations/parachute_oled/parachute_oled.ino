#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Para Chute");

  display.display();
}

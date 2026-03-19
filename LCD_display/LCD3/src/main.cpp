#include "secretscreen.h"
#include "welcomepage.h"
#include "welcomepageanim.h"

void setup() {
  Serial.begin(115200);

  gfx->begin();
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  touchSetup();
  drawSplash();
  Serial.println("Splash shown");

  // TODO: drawDashboard() goes here next
  Serial.println("Ready for dashboard");
}

void loop() {
  touchHandleSwitch(); 
  
  static uint32_t last = 0;
    if (millis() - last >= 1000) {
    last = millis();
    Serial.printf("Uptime: %lu s\n", millis() / 1000);
  }
}
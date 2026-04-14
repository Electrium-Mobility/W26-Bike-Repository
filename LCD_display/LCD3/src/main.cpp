#include <cstring>
#include "welcomepage.h"
#include "welcomepageanim.h"
#include "secretscreen.h"
#include "stats.h"

void setup() {
  Serial.begin(115200);

  gfx->begin();
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  appStartMs = millis();

  touchSetup();
  drawSplash();
  Serial.println("Splash shown");

  Serial.println("Welcome running");
}

void loop() {
  uint32_t now = millis();

  if (appScreen == APP_WELCOME) {
    touchHandleSwitch();

    if (now - appStartMs >= WELCOME_DURATION_MS) {
      appScreen = APP_STATS;
      statsInit();
      Serial.println("-> Stats dashboard");
    }
    return;
  }

  statsUpdateAndRender();
  // TODO: drawDashboard() goes here next
  Serial.println("Ready for dashboard");
}
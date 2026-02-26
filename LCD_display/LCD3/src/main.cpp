#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define LCD_BL 1  // try 2 or 38 if screen stays dark

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
  45, 47, 21, 48, 40, 39
);

Arduino_GFX *panel = new Arduino_NV3041A(
  bus, GFX_NOT_DEFINED, 1, true
);

Arduino_Canvas *gfx = new Arduino_Canvas(272, 480, panel);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");

  gfx->begin();
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  Serial.println("Display init done");

  gfx->fillScreen(0x0000);           // black
  gfx->setTextColor(0xFFFF);         // white
  gfx->setTextSize(3);
  gfx->setCursor(40, 200);
  gfx->print("Hello!");
  gfx->setCursor(20, 240);
  gfx->setTextSize(2);
  gfx->print("Screen is working");
  gfx->flush();

  Serial.println("Screen drawn");
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last >= 1000) {
    last = millis();
    Serial.printf("Uptime: %lu s\n", millis() / 1000);
  }
}
#include <SPI.h>
#include "welcomepage.h"
#include "secretscreen.h"

// ── Touch SPI ──
static SPIClass touchSPI(HSPI);
static uint32_t lastTouchTime = 0;

// ── Screen state ──
enum Screen { SCREEN_SPLASH, SCREEN_SECRET };
static Screen currentScreen = SCREEN_SPLASH;

// ── Raw XPT2046 read ──
static uint16_t xpt2046Read(uint8_t cmd) {
  digitalWrite(TOUCH_CS, LOW);
  touchSPI.transfer(cmd);
  uint16_t hi = touchSPI.transfer(0x00);
  uint16_t lo = touchSPI.transfer(0x00);
  digitalWrite(TOUCH_CS, HIGH);
  return ((hi << 8 | lo) >> 3) & 0xFFF;
}

static bool isTouched() {
  if (digitalRead(TOUCH_IRQ) == HIGH) return false;
  return xpt2046Read(0xB0) > 200;
}

// ── Call once from setup() ──
void touchSetup() {
  pinMode(TOUCH_CS,  OUTPUT);
  pinMode(TOUCH_IRQ, INPUT);
  digitalWrite(TOUCH_CS, HIGH);
  touchSPI.begin(TOUCH_SCK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
  touchSPI.setFrequency(2000000);
  Serial.println("Touch ready");
}

// ── Call every loop() ──
void touchHandleSwitch() {
  if (!isTouched()) return;

  uint32_t now = millis();
  if (now - lastTouchTime < TOUCH_DEBOUNCE_MS) return;
  lastTouchTime = now;

  if (currentScreen == SCREEN_SPLASH) {
    currentScreen = SCREEN_SECRET;
    drawSecretScreen();
    Serial.println("-> Secret Screen");
  } else {
    currentScreen = SCREEN_SPLASH;
    drawSplash();
    Serial.println("-> Splash");
  }
}

// ── Screen drawing ──
static void drawScreenBorder2(uint16_t color, int thickness = 2) {
  for (int i = 0; i < thickness; i++) {
    gfx->drawRect(i, i, SCREEN_WIDTH - (i * 2), SCREEN_HEIGHT - (i * 2), color);
  }
}

void drawSecretScreen() {
  gfx->fillScreen(C_BLACK);
  drawScreenBorder2(C_GREEN);

  const char *title = "Screen 2";
  gfx->setTextColor(C_WHITE);
  gfx->setTextSize(4);
  gfx->setCursor((SCREEN_WIDTH - textWidth(title, 4)) / 2, SCREEN_HEIGHT / 2 - 20);
  gfx->print(title);

  const char *sub = "Your dashboard here";
  gfx->setTextColor(C_GREEN);
  gfx->setTextSize(1);
  gfx->setCursor((SCREEN_WIDTH - textWidth(sub, 1)) / 2, SCREEN_HEIGHT / 2 + 30);
  gfx->print(sub);

  const char *hint = "Touch to go back";
  gfx->setTextColor(C_GRAY);
  gfx->setTextSize(1);
  gfx->setCursor((SCREEN_WIDTH - textWidth(hint, 1)) / 2, SCREEN_HEIGHT - 14);
  gfx->print(hint);

  gfx->flush();
}
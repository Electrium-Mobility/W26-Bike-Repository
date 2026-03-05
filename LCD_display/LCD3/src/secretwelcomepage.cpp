#include <SPI.h>
#include "welcomepage.h"
#include "secretscreen.h"

//screen will have 2 phase - one is if the bike has



// ── Touch SPI ──
static SPIClass touchSPI(HSPI);
static uint32_t lastTouchTime = 0;
static uint32_t lastSecretAnimTime = 0;
static uint8_t secretAnimFrame = 0;
static const uint16_t SECRET_ANIM_INTERVAL_MS = 120;

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
  uint32_t now = millis();

  if (currentScreen == SCREEN_SECRET && (now - lastSecretAnimTime >= SECRET_ANIM_INTERVAL_MS)) {
    lastSecretAnimTime = now;
    secretAnimFrame++;
    drawSecretScreen();
  }

  if (!isTouched()) return;
  if (now - lastTouchTime < TOUCH_DEBOUNCE_MS) return;
  lastTouchTime = now;

  if (currentScreen == SCREEN_SPLASH) {
    currentScreen = SCREEN_SECRET;
    secretAnimFrame = 0;
    lastSecretAnimTime = now;
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

static void drawTagPill(int x, int y, const char *text, uint16_t bg, uint16_t fg) {
  const int w = textWidth(text, 1) + 8;
  gfx->fillRoundRect(x, y, w, 12, 4, bg);
  gfx->setTextColor(fg);
  gfx->setTextSize(1);
  gfx->setCursor(x + 4, y + 2);
  gfx->print(text);
}

static void drawBrainrotOverlay(uint8_t frame) {
  const uint16_t C_BLUE = 0x001F;
  const uint16_t C_CYAN = 0x07FF;
  const uint16_t C_PINK = 0xFBB7;

  const char *leftTags[] = {"NO CAP", "FR FR", "W RIZZ", "AURA++"};
  const char *rightTags[] = {"SIGMA", "YEET", "VIBE", "MAIN CHAR"};
  const char *ticker[] = {"SKIBIDI MODE", "CHAT IS SPAMMING W", "LOWKEY GOATED", "VIBE CHECK PASSED"};

  const int tagIdx = (frame / 6) % 4;
  const int tickerIdx = (frame / 8) % 4;

  drawTagPill(12, 14, leftTags[tagIdx], C_RED, C_WHITE);
  drawTagPill(SCREEN_WIDTH - 12 - (textWidth(rightTags[tagIdx], 1) + 8), 14, rightTags[tagIdx], C_BLUE, C_WHITE);

  const char *level = "BRAINROT LEVEL: MAX";
  gfx->setTextColor(C_PINK);
  gfx->setTextSize(1);
  gfx->setCursor((SCREEN_WIDTH - textWidth(level, 1)) / 2, 38);
  gfx->print(level);

  gfx->setTextColor(C_CYAN);
  gfx->setTextSize(1);
  gfx->setCursor((SCREEN_WIDTH - textWidth(ticker[tickerIdx], 1)) / 2, SCREEN_HEIGHT / 2 + 46);
  gfx->print(ticker[tickerIdx]);
}

static void drawNyanCatMeme(int x, int y, uint8_t frame) {
  const uint16_t C_ORANGE = 0xFD20;
  const uint16_t C_BLUE = 0x001F;
  const uint16_t C_PINK = 0xFBB7;
  const uint16_t C_COOKIE = 0xFD95;
  const uint16_t C_CRUST = 0x9A46;
  const uint16_t C_CAT = 0xBDF7;
  const uint16_t C_DARK = 0x52AA;

  const int bob = (frame % 4 == 1) ? 1 : ((frame % 4 == 3) ? -1 : 0);
  const bool stepA = (frame % 2 == 0);
  const int rainbowShift = frame % 6;

  const uint16_t rainbow[6] = {C_RED, C_ORANGE, C_YELLOW, C_GREEN, C_BLUE, C_PINK};
  for (int i = 0; i < 6; i++) {
    int trailLen = 72 - ((frame + i) % 3) * 4;
    gfx->drawFastHLine(x - trailLen, y + bob + (i * 4), trailLen, rainbow[(i + rainbowShift) % 6]);
  }

  gfx->fillRoundRect(x + 28, y + bob + 6, 44, 28, 4, C_COOKIE);
  gfx->drawRoundRect(x + 28, y + bob + 6, 44, 28, 4, C_CRUST);
  gfx->fillRect(x + 33, y + bob + 10, 34, 20, C_PINK);
  gfx->drawRect(x + 33, y + bob + 10, 34, 20, C_CRUST);

  gfx->drawPixel(x + 39, y + bob + 14, C_GREEN);
  gfx->drawPixel(x + 46, y + bob + 16, C_BLUE);
  gfx->drawPixel(x + 52, y + bob + 12, C_YELLOW);
  gfx->drawPixel(x + 58, y + bob + 18, C_RED);
  gfx->drawPixel(x + 43, y + bob + 24, C_BLUE);
  gfx->drawPixel(x + 55, y + bob + 26, C_GREEN);

  gfx->fillRect(x + 5, y + bob + 7, 27, 25, C_CAT);
  gfx->fillTriangle(x + 7, y + bob + 7, x + 11, y + bob + 2, x + 15, y + bob + 7, C_CAT);
  gfx->fillTriangle(x + 22, y + bob + 7, x + 26, y + bob + 2, x + 30, y + bob + 7, C_CAT);
  gfx->drawRect(x + 5, y + bob + 7, 27, 25, C_DARK);

  gfx->fillCircle(x + 14, y + bob + 17, 1, C_DARK);
  gfx->fillCircle(x + 24, y + bob + 17, 1, C_DARK);
  gfx->drawFastHLine(x + 15, y + bob + 23, 8, C_DARK);

  if (stepA) {
    gfx->fillRect(x + 36, y + bob + 34, 6, 4, C_DARK);
    gfx->fillRect(x + 57, y + bob + 34, 6, 4, C_DARK);
  } else {
    gfx->fillRect(x + 33, y + bob + 34, 6, 4, C_DARK);
    gfx->fillRect(x + 60, y + bob + 34, 6, 4, C_DARK);
  }

  if (stepA) {
    gfx->drawLine(x + 2, y + bob + 20, x - 4, y + bob + 16, C_DARK);
    gfx->drawLine(x + 2, y + bob + 23, x - 5, y + bob + 22, C_DARK);
  } else {
    gfx->drawLine(x + 2, y + bob + 20, x - 6, y + bob + 21, C_DARK);
    gfx->drawLine(x + 2, y + bob + 23, x - 4, y + bob + 27, C_DARK);
  }

  if (frame % 2 == 0) {
    gfx->drawPixel(x - 15, y + bob + 4, C_WHITE);
    gfx->drawPixel(x - 11, y + bob + 8, C_WHITE);
    gfx->drawPixel(x + 84, y + bob + 12, C_WHITE);
  } else {
    gfx->drawPixel(x - 18, y + bob + 11, C_WHITE);
    gfx->drawPixel(x - 8, y + bob + 3, C_WHITE);
    gfx->drawPixel(x + 80, y + bob + 6, C_WHITE);
  }
}

bool touchEarlyExit() {
  if (digitalRead(TOUCH_IRQ) == HIGH) return false;
  uint32_t now = millis();
  if (now - lastTouchTime < TOUCH_DEBOUNCE_MS) return false;
  lastTouchTime = now;
  currentScreen = SCREEN_SECRET;
  return true;
}


void drawSecretScreen() {
  gfx->fillScreen(C_BLACK);
  drawScreenBorder2(C_GREEN);
  drawBrainrotOverlay(secretAnimFrame);

  const char *titleTop = "SECRET";
  const char *titleBottom = "CODE";
  gfx->setTextColor(C_RED);
  gfx->setTextSize(4);
  gfx->setCursor((SCREEN_WIDTH - textWidth(titleTop, 4)) / 2, SCREEN_HEIGHT / 2 - 44);
  gfx->print(titleTop);
  gfx->setCursor((SCREEN_WIDTH - textWidth(titleBottom, 4)) / 2, SCREEN_HEIGHT / 2 - 10);
  gfx->print(titleBottom);

  const char *sub = "Restricted to OG W26 Members";
  gfx->setTextColor(C_GREEN);
  gfx->setTextSize(1);
  gfx->setCursor((SCREEN_WIDTH - textWidth(sub, 1)) / 2, SCREEN_HEIGHT / 2 + 30);
  gfx->print(sub);

  const char *meme = "Nyan mode unlocked";
  gfx->setTextColor(C_YELLOW);
  gfx->setTextSize(2);
  gfx->setCursor((SCREEN_WIDTH - textWidth(meme, 2)) / 2, SCREEN_HEIGHT / 2 + 72);
  gfx->print(meme);

  drawNyanCatMeme((SCREEN_WIDTH / 2) - 32, SCREEN_HEIGHT / 2 + 105, secretAnimFrame);

  const char *hint = "Touch to go back";
  gfx->setTextColor(C_GRAY);
  gfx->setTextSize(1);
  gfx->setCursor((SCREEN_WIDTH - textWidth(hint, 1)) / 2, SCREEN_HEIGHT - 14);
  gfx->print(hint);

  gfx->flush();
}
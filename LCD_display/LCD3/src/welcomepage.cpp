#include <cstring>
#include "welcomepage.h"
#include "welcomepageanim.h"
#include "secretscreen.h"

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
  45, 47, 21, 48, 40, 39
);

Arduino_GFX *panel = new Arduino_NV3041A(
  bus, GFX_NOT_DEFINED, 1, true
);

Arduino_Canvas *gfx = new Arduino_Canvas(SCREEN_WIDTH, SCREEN_HEIGHT, panel);

// ── Text & Layout Functions ──

int textWidth(const char *text, int textSize) {
  return static_cast<int>(strlen(text)) * 6 * textSize;
}

static void drawSectionBorder(int topY, int bottomY, uint16_t color, int horizontalMargin = 20) {
  const int borderWidth = SCREEN_WIDTH - (horizontalMargin * 2);
  gfx->drawFastHLine(horizontalMargin, topY, borderWidth, color);
  gfx->drawFastHLine(horizontalMargin, bottomY, borderWidth, color);
}

static void drawScreenBorder(uint16_t color, int thickness = 2) {
  for (int i = 0; i < thickness; i++) {
    gfx->drawRect(i, i, SCREEN_WIDTH - (i * 2), SCREEN_HEIGHT - (i * 2), color);
  }
}

void drawSplash() {
  const int halfWidth = SCREEN_WIDTH / 2;

  gfx->fillScreen(C_BLACK);
  drawScreenBorder(C_GREEN);

  // ── WELCOME ──
  const char *welcomeText = "WELCOME";
  gfx->setTextColor(C_WHITE);
  gfx->setTextSize(4);
  gfx->setCursor((SCREEN_WIDTH - textWidth(welcomeText, 4)) / 2, 40);
  gfx->print(welcomeText);

  // ── Electrium Mobility ──
  const char *brandText = "Electrium Mobility";
  gfx->setTextColor(C_GREEN);
  gfx->setTextSize(2);
  gfx->setCursor((SCREEN_WIDTH - textWidth(brandText, 2)) / 2, 100);
  gfx->print(brandText);

  // ── w26 bike ──
  const char *modelText = "- W26 Bike -";
  const int modelTextY = 130;
  gfx->setTextColor(C_YELLOW);
  gfx->setTextSize(2);
  gfx->setCursor((SCREEN_WIDTH - textWidth(modelText, 2)) / 2, modelTextY);
  gfx->print(modelText);

  // ── Contributors ──
  const char *contributorsText = "Contributors";
  const int contributorsSize = 2;
  const int contributorsY = 175;
  const int contributorsTextHeight = 8 * contributorsSize;
  const int borderTopY = contributorsY - 10;
  const int namesStartY = 200;
  const int namesEndY = namesStartY + (4 * 30) + (8 * 2);  // 5 rows, last row + text height
  const int borderBottomY = namesEndY + 10;

  drawSectionBorder(borderTopY, borderBottomY, C_GRAY);

  gfx->setTextColor(C_WHITE);
  gfx->setTextSize(contributorsSize);
  gfx->setCursor((SCREEN_WIDTH - textWidth(contributorsText, contributorsSize)) / 2, contributorsY);
  gfx->print(contributorsText);

  const char* names[] = {
    "Nathan",  "Jumaana",
    "Jisara",  "Muhammad",
    "Sophie",  "Eric",
    "Franky",  "Calvin",
    "Colleen",  "Galit"
  };

  gfx->setTextColor(C_WHITE);
  gfx->setTextSize(2);

  for (int i = 0; i < 10; i++) {
    int col = i % 2;          // 0 = left, 1 = right
    int row = i / 2;          // 0–4
    int nameWidth = textWidth(names[i], 2);
    int x = (col * halfWidth) + ((halfWidth - nameWidth) / 2);
    int y   = 200 + row * 30;
    gfx->setCursor(x, y);
    gfx->print(names[i]);
  }

  gfx->flush();
  if (!animateBikeAcrossBottom()) {
  animateBikeToW26FromRight(modelText, modelTextY, 2200, 25);
}
}

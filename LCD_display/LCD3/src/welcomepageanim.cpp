#include "welcomepage.h"
#include "welcomepageanim.h"
#include "secretscreen.h"

// ── Bike Drawing & Animation ──

void drawBike(int x, int y, uint16_t color, int scale) {
  const int wheelRadius = 6 * scale;
  const int rearWheelX = x;
  const int frontWheelX = x + (30 * scale);
  const int wheelY = y + (10 * scale);
  const int seatX = x + (10 * scale);
  const int seatY = y;
  const int handleX = x + (24 * scale);
  const int handleY = y - (2 * scale);

  gfx->drawCircle(rearWheelX, wheelY, wheelRadius, color);
  gfx->drawCircle(frontWheelX, wheelY, wheelRadius, color);

  gfx->drawLine(rearWheelX, wheelY, seatX, seatY, color);
  gfx->drawLine(seatX, seatY, frontWheelX, wheelY, color);
  gfx->drawLine(seatX, seatY, handleX, handleY, color);
  gfx->drawLine(handleX, handleY, frontWheelX, wheelY, color);

  gfx->drawFastHLine(seatX - (4 * scale), seatY - scale, 8 * scale, color);
  gfx->drawLine(handleX, handleY, handleX + (4 * scale), handleY - (3 * scale), color);
}

void animateBikeAcrossBottom(uint32_t durationMs, uint16_t frameDelayMs) {
  const int bikeScale = 2;
  const int bikeWidth = 30 * bikeScale;
  const int bikeY = SCREEN_HEIGHT - 82;
  const int laneTop = bikeY - 16;
  const int laneHeight = 52;
  const int bikeStartX = -bikeWidth - 24;
  const int bikeEndX = SCREEN_WIDTH + 24;

  // insert best quote of all times 
  const char *taglineText = "Electrium Mobility's Greatest Bike";
  const int taglineSize = 1;
  const int taglineWidth = textWidth(taglineText, taglineSize);
  const int taglineY = bikeY - 30;

  const uint32_t startTime = millis();

  while (true) {
    uint32_t elapsed = millis() - startTime;
    if (elapsed > durationMs) {
      break;
    }

    int bikeX = bikeStartX + static_cast<int>((static_cast<int64_t>(bikeEndX - bikeStartX) * elapsed) / durationMs);

    gfx->fillRect(2, laneTop, SCREEN_WIDTH - 4, laneHeight, C_BLACK);
    
    gfx->setTextColor(C_YELLOW);
    gfx->setTextSize(taglineSize);
    gfx->setCursor((SCREEN_WIDTH - taglineWidth) / 2, taglineY);
    gfx->print(taglineText);
    
    drawBike(bikeX, bikeY, C_GREEN, bikeScale);

    gfx->flush();
    delay(frameDelayMs);
  }

  gfx->fillRect(2, laneTop, SCREEN_WIDTH - 4, laneHeight, C_BLACK);
  gfx->flush();
}

void animateBikeToW26FromRight(const char *modelText, int modelTextY, uint32_t durationMs, uint16_t frameDelayMs) {
  const int bikeScale = 1;
  const int textSize = 2;
  const int bikeWidth = 30 * bikeScale;
  const int textWidth_val = textWidth(modelText, textSize);
  const int spacing = 10;
  const int totalWidth = textWidth_val + spacing + bikeWidth;
  
  const int bikeY = 130;
  const int laneTop = 125;
  const int laneHeight = 35;
  
  const int textStartX = (SCREEN_WIDTH - textWidth_val) / 2;
  const int textFinalX = (SCREEN_WIDTH - totalWidth) / 2;
  const int bikeFinalX = textFinalX + textWidth_val + spacing;
  const int bikeStartX = SCREEN_WIDTH + 40;

  const uint32_t startTime = millis();

  while (true) {
    uint32_t elapsed = millis() - startTime;
    if (elapsed > durationMs) {
      break;
    }

    int textX = textStartX - static_cast<int>((static_cast<int64_t>(textStartX - textFinalX) * elapsed) / durationMs);
    int bikeX = bikeStartX - static_cast<int>((static_cast<int64_t>(bikeStartX - bikeFinalX) * elapsed) / durationMs);

    gfx->fillRect(2, laneTop, SCREEN_WIDTH - 4, laneHeight, C_BLACK);

    gfx->setTextColor(C_YELLOW);
    gfx->setTextSize(textSize);
    gfx->setCursor(textX, modelTextY);
    gfx->print(modelText);

    drawBike(bikeX, bikeY, C_GREEN, bikeScale);

    gfx->flush();
    delay(frameDelayMs);
  }

  gfx->fillRect(2, laneTop, SCREEN_WIDTH - 4, laneHeight, C_BLACK);
  gfx->setTextColor(C_YELLOW);
  gfx->setTextSize(textSize);
  gfx->setCursor(textFinalX, modelTextY);
  gfx->print(modelText);
  drawBike(bikeFinalX, bikeY, C_GREEN, bikeScale);
  gfx->flush();
}

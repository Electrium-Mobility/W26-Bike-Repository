#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

// ── Screen Configuration ──
#define LCD_BL 1
#define SCREEN_WIDTH 272
#define SCREEN_HEIGHT 480

// ── Colors ──
#define C_BLACK   0x0000
#define C_WHITE   0xFFFF
#define C_GREEN   0x07E0
#define C_GRAY    0x8410
#define C_YELLOW  0xFFE0
#define C_RED     0xF800

// ── Global Graphics Objects (defined in main.cpp) ──
extern Arduino_DataBus  *bus;
extern Arduino_GFX      *panel;
extern Arduino_Canvas   *gfx;

// ── Utility Functions ──
int textWidth(const char *text, int textSize);

// ── Main Functions ──
void drawSplash();

static const uint32_t WELCOME_DURATION_MS = 10000;
enum AppScreen { APP_WELCOME, APP_STATS };
static AppScreen appScreen = APP_WELCOME;
static uint32_t appStartMs = 0;

#endif
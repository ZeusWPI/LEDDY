// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>

#include "led_control/led_control.hpp"

extern int amountOfScreens;

extern LedControl lcs[4];
extern int totalLedSize;

enum mode_t {
  SCROLLING_TEXT,
  STATIC,
  AUDIO
};

extern unsigned long updateDelayMs;
extern unsigned long autoResetMs;

void setup();
void loop();
void reset();
void contentChanged();
void receiveSerial();
void processCommand();

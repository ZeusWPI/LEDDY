// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>

#include "functionality/text/font.hpp"

// GLOBAL TEXT VARS
extern char text[50];
extern size_t textBufferSize;
extern byte textBuffer[460];

extern int trailingWhitespace;
extern int spaceWidth;
extern int currentTextIndex;
extern int scrollDirection;
// GLOBAL TEXT VARS

/**
  * Initialize the `text` buffer
  *   padBuffer: pad the text buffer with spaces to fille the whole screen
  */
void initText(const char* text, bool padBuffer = false);

/**
  * Move the Text one over
  */
void scrollText();

/**
  * Draw the `text` buffer
  */
void renderText();

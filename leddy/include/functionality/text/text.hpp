// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "functionality/text/font.hpp"

// Global options
extern int16_t trailingWhitespace;
extern int16_t spaceWidth;
extern int16_t scrollDirection;

/**
 * Render `text` to the internal pixel buffer.
 * Params:
 *     text = The asciiz string to render. Length must be < 64 bytes.
 *     padBuffer = Pad the text buffer with spaces to fill the whole screen.
 */
void initText(const char* text, bool padBuffer = false);

/**
 * Shifts the internal pixel buffer by `scrolldirection`.
 */
void scrollText();

/**
 * Send the internal pixel buffer to the led matrices.
 */
void renderText();

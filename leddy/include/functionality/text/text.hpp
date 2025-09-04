// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "functionality/text/font.hpp"

// Global options
extern int16_t g_trailingWhitespace;
extern int16_t g_spaceWidth;
extern int16_t g_scrollDirection;

/**
 * Params:
 *     newText = An asciiz string. A copy is made. Truncated if too long.
 * Note: Has no effect until `prepareText` is called.
 */
void setText(const char *newText);

/**
 * Render the text set by `setText` to the internal pixel buffer.
 * Params:
 *     padBuffer = Pad the text buffer with spaces to fill the whole screen.
 */
void prepareText(bool padBuffer = false);

/**
 * Shifts the internal pixel buffer by `scrolldirection`.
 */
void scrollText();

/**
 * Send the internal pixel buffer to the led matrices.
 */
void renderText();

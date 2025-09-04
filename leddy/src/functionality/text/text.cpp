// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "functionality/text/text.hpp"

#include "leddy.hpp"

// Global options
int16_t g_spaceWidth = 6;
int16_t g_trailingWhitespace = 12 * 8;
int16_t g_scrollDirection = 1;

/**
 * Helper function to get a bit from a byte.
 */
static inline bool getBit(uint8_t input, uint8_t bit)
{
    return (input & (0x80 >> bit)) > 0;
}

/**
 * Helper function to change a bit in a byte.
 */
static inline uint8_t setBit(uint8_t input, uint8_t bit, bool value)
{
    const byte mask = value << (7 - bit); // example 00010000
    return (input & (~mask)) | mask;
}

/**
 * Characters are stored as a byte per row, we want to store them as a byte per
 * column.
 */
static void renderCharacterColumn(char c, uint8_t col[8])
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            const uint8_t row = pgm_read_byte(&(font8x8_basic[(uint8_t)c][7 - j]));
            const bool pixel = getBit(row, 7 - i);
            col[i] = setBit(col[i], j, pixel);
        }
}

static char text[64] = {};
static size_t textLength = 0;
static uint8_t textPixels[sizeof(text) * 9] = {};
static size_t textPixelsLength = 0;
static int16_t textScrollIndex = 0;

void setText(const char *newText)
{
    strncpy(text, newText, sizeof(text));
    textLength = strlen(text);
}

void prepareText(bool padBuffer)
{
    textScrollIndex = 0;
    memset(textPixels, 0, textPixelsLength);
    textPixelsLength = textLength * 8;

    // Render the text to `textPixels`.
    size_t col = 0;
    for (size_t textIndex = 0; textIndex < textLength; ++textIndex)
    {
        const char c = text[textIndex];
        if (c == ' ')
        {
            // Spaces have configurable width.
            // Pixels are zero-initialized so we can just skip spaces.
            col += g_spaceWidth;
        }
        else
        {
            uint8_t columnBasedCharacter[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            renderCharacterColumn(text[textIndex], columnBasedCharacter);
            for (size_t j = 0; j < 8; ++j)
                if (columnBasedCharacter[j] != 0) // Skip whitespace
                    textPixels[col++] = columnBasedCharacter[j];
            col++; // Add a single spacing column
        }
    }

    textPixelsLength += g_trailingWhitespace;
    if (padBuffer)
        textPixelsLength += 12 * 8; // The full width in whitespace.
    textPixelsLength = constrain(textPixelsLength, 0, sizeof(textPixels));
}

void scrollText()
{
    textScrollIndex = (textPixelsLength + textScrollIndex + g_scrollDirection) % textPixelsLength;
}

void renderText()
{
    size_t i = textScrollIndex;
    for (size_t chain = 0; chain < ct_ledMatrixChainCount; chain++)
    {
        for (size_t row = 0; row < 8; row++)
            g_lmcs[chain].setRowsFromPixArr(row, textPixels, textPixelsLength, i + row);
        i += ct_ledMatricesPerChain * 8;
    }
}

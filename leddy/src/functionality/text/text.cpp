// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "functionality/text/text.hpp"

#include "leddy.hpp"

// Global options
int16_t trailingWhitespace = 6 * 8; // Four modules between loops
int16_t spaceWidth = 6;
int16_t scrollDirection = 1;

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
 * Squash spaces in the `pixels` buffer. The buffer is filled up to `length`.
 * Returns the new length (<= original length).
 */
static int squashSpaces(uint8_t *pixels, int length)
{
    int newLength = length;
    int i = 0;
    while (i < newLength)
    {
        if (!pixels[i]) // A zero row
        {
            int spaces = 0;
            // while not at the end and there is a space
            while (i + spaces < newLength && !pixels[i + spaces])
                spaces++;
            if (i + spaces == newLength)
            {
                // At the end of the buffer, trim spaces and return new length
                return newLength - spaces;
            }
            // In the input, a space character will be 8 space columns.
            // Either squash to spaceWidth spaces or 1 space
            const int toShift = spaces >= 8 ? spaces - spaceWidth : spaces - 1;
            if (toShift > 0)
            {
                for (int j = i + spaces; j < newLength; j++)
                    pixels[j - toShift] = pixels[j];
                newLength = newLength - toShift;
            }
            i = i + spaces - toShift;
        }
        i++;
    }
    return newLength;
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

static uint8_t textPixels[460] = {}; // If text is 64 chars, then big enough
static size_t textPixelsLength = 0;
static int16_t textScrollIndex = 0;

void initText(const char *newText, bool padBuffer)
{
    textScrollIndex = 0;

    // clear buffer, just to be sure
    memset(textPixels, 0, textPixelsLength);

    const size_t newTextLength = strlen(newText);
    textPixelsLength = newTextLength * 8;

    // setup textPixels
    for (size_t i = 0; i < newTextLength; i++)
    {
        uint8_t columnBasedCharacter[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        renderCharacterColumn(newText[i], columnBasedCharacter);
        for (int j = 0; j < 8; j++)
            textPixels[i * 8 + j] = columnBasedCharacter[j];
    }

    // Squash space between letters
    const size_t squashedSize = squashSpaces(textPixels, textPixelsLength);
    textPixelsLength = textPixelsLength + trailingWhitespace;
    if (padBuffer)
        textPixelsLength = 12 * 8;
    for (size_t i = squashedSize; i < textPixelsLength; i++)
        textPixels[i] = 0;
}

void scrollText()
{
    textScrollIndex = (textScrollIndex + scrollDirection + textPixelsLength) % textPixelsLength;
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

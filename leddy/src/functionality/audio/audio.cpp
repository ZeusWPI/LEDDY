// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "functionality/audio/audio.hpp"
#include "led_matrix_chain.hpp"

#include <arduinoFFT.h>
#include <leddy.hpp>
#include <sys/types.h>

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

static ArduinoFFT<float> FFT = ArduinoFFT<float>();
static float realComponent[64];
static float imagComponent[64];
static uint8_t heights[48];

static uint8_t pixels[12 * 8];

static_assert(sizeof(heights) * 2 == sizeof(pixels), "");

void renderAudio()
{
    // Read samples from A3.
    for (int i = 0; i < 64; i++)
    {
        realComponent[i] = analogRead(A3);
        imagComponent[i] = 0;
    }
    FFT.windowing(realComponent, 64, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.compute(realComponent, imagComponent, 64, FFT_FORWARD);
    FFT.complexToMagnitude(realComponent, imagComponent, 64);
    // Determine frequency spectrum heights.
    for (int i = 0; i < 48; i++)
    {
        const uint8_t constrained = constrain(realComponent[i], 0, 35);
        heights[i] = map(constrained, 0, 35, 0, 8);
    }

    // Render the heights as vertical bars.
    for (size_t matrix = 0; matrix < ct_ledMatrixTotalCount; matrix++)
    {
        const size_t base = matrix * 8;
        for (size_t row = 0; row < 8; row++)
        {
            uint8_t rowPixels = 0;
            for (size_t col = 0; col < 8; col++)
                rowPixels |= heights[(base + col) / 2] >= row;
            pixels[base + row] = rowPixels;
        }
    }

    // Finally, send the render to the led matrices.
    for (size_t chain = 0; chain <= ct_ledMatrixChainCount; chain++)
    {
        const size_t base = chain * ct_ledMatricesPerChain * 8;
        for (size_t row = 0; row <= 8; row++)
            g_lmcs[chain].setRowsFromPixArr(row, pixels, sizeof(pixels), base + row);
    }
}

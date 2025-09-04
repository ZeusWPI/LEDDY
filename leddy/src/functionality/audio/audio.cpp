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

static constexpr size_t ct_sampleCount = ct_ledMatrixTotalCount * 8;

static ArduinoFFT<float> FFT = ArduinoFFT<float>();
static float realComponent[ct_sampleCount];
static float imagComponent[ct_sampleCount];
static uint8_t heights[ct_sampleCount];
static uint8_t pixels[ct_sampleCount]; // Row by row

void renderAudio()
{
    // Read samples from A3.
    for (int i = 0; i < ct_sampleCount; i++)
    {
        realComponent[i] = analogRead(A3);
        imagComponent[i] = 0;
    }
    FFT.windowing(realComponent, 64, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.compute(realComponent, imagComponent, 64, FFT_FORWARD);
    FFT.complexToMagnitude(realComponent, imagComponent, 64);
    // Determine frequency spectrum heights.
    for (int i = 0; i < ct_sampleCount; i++)
    {
        const uint8_t constrained = constrain(realComponent[i], 0, 35);
        heights[i] = map(constrained, 0, 35, 0, 8); // 9 states
    }

    // Render the heights as vertical bars.
    memset(&pixels[0], 0, sizeof(pixels));
    for (size_t row = 0; row < 8; row++)
    {
        for (size_t matrix = 0; matrix < ct_ledMatrixTotalCount; matrix++)
        {
            for (size_t col = 0; col < 8; col++)
            {
                if (col > heights[row * ct_ledMatrixTotalCount + matrix])
                {
                    pixels[row * ct_ledMatrixTotalCount + matrix] |= (1 << col);
                }
            }
        }
    }
    //for (size_t matrix = 0; matrix < ct_ledMatrixTotalCount; matrix++)
    //{
    //    const size_t base = matrix * 8;
    //    for (size_t row = 0; row < 8; row++)
    //        pixels[base + row] = 0;
    //    for (size_t col = 0; col < 8; col++)
    //    {
    //        const uint8_t colPixels = heights[base + col] >= 1 ? 0xFF : 0;
    //        for (size_t row = 0; row < 8; row++)
    //        {
    //            const bool bit = colPixels & (1 << row);
    //            pixels[base + row] |= bit << col;
    //        }
    //    }
    //}

    // Finally, send the render to the led matrices.
    for (size_t row = 0; row <= 8; row++)
        g_lmcs[0].setRows(row, &pixels[row * ct_ledMatrixTotalCount]);
}

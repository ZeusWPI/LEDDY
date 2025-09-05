// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "functionality/audio/audio.hpp"

#include <arduinoFFT.h>

#include "led_matrix_chain.hpp"
#include "leddy.hpp"

static constexpr size_t ct_sampleCount = ct_ledMatrixCount * 8;

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
        for (size_t matrix = 0; matrix < ct_ledMatrixCount; matrix++)
            for (size_t col = 0; col < 8; col++)
                if (col > heights[row * ct_ledMatrixCount + matrix])
                    pixels[row * ct_ledMatrixCount + matrix] |= (1 << col);

    // Finally, send the render to the led matrices.
    for (size_t row = 0; row <= 8; row++)
        g_lmc.setRows(row, &pixels[row * ct_ledMatrixCount]);
}

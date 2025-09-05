// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace font8x8_ascii_zeus
{
    static constexpr uint8_t ct_firstChar = 0x20; // space
    static constexpr uint8_t ct_lastChar = 0x8F;
    static constexpr uint8_t ct_len = 1 + ct_lastChar - ct_firstChar;
    extern const uint8_t arr[ct_len][8];
}

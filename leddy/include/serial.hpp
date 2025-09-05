// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>
#include <stdint.h>

enum struct CommandType : uint8_t
{
    NONE = 0x00,

    CHANGE_MODE_CLEAR          = 0x10,
    CHANGE_MODE_FILL           = 0x11,
    CHANGE_MODE_TEXT           = 0x12,
    CHANGE_MODE_SCROLLING_TEXT = 0x13,
    CHANGE_MODE_AUDIO          = 0x14,

    SET_OPT_targetFrameTimeMs       = 0x20,
    SET_OPT_autoResetMs             = 0x21,
    SET_OPT_text_spaceWidth         = 0x22,
    SET_OPT_text_trailingWhitespace = 0x23,
    SET_OPT_text_scrollDirection    = 0x24,

    SET_TEXT = 0x30,
};

struct Command
{
    CommandType type;
    union
    {
        struct
        {
            int32_t setOptValue;
        };
        struct
        {
            const char* setTextValue;
        };
    };
};

void initSerial();
Command receiveCommand();

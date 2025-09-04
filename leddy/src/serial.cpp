// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "serial.hpp"

#define str_try_cut_prefix(s, literal_prefix) (strncmp(s, literal_prefix, sizeof(literal_prefix) - 1) == 0 ? s += sizeof(literal_prefix) - 1 : nullptr)

static const char *receiveSerial()
{
    static char receiveBuffer[64] = {};
    static size_t receiveIndex = 0;
    static constexpr size_t ct_maxNumChars = sizeof(receiveBuffer) - 1;

    while (Serial.available() > 0)
    {
        const char c = Serial.read();
        Serial.write(c); // Echo the input
        if (c == '\n' || receiveIndex >= ct_maxNumChars)
        {
            receiveBuffer[receiveIndex] = 0;
            receiveIndex = 0;
            return &receiveBuffer[0];
        }
        receiveBuffer[receiveIndex++] = c;
    }
    return NULL;
}

Command receiveCommand()
{
    const char *str = receiveSerial();

    Command cmd = {};

    if (str == NULL)
        return cmd;

    const char c0 = *(str++);

    switch (c0)
    {
    case 'M':
    {
        const char c1 = *(str++);
        switch (c1)
        {
        case 'C': cmd.type = CommandType::CHANGE_MODE_CLEAR;          return cmd;
        case 'F': cmd.type = CommandType::CHANGE_MODE_FILL;           return cmd;
        case 'T': cmd.type = CommandType::CHANGE_MODE_TEXT;           return cmd;
        case 'S': cmd.type = CommandType::CHANGE_MODE_SCROLLING_TEXT; return cmd;
        case 'A': cmd.type = CommandType::CHANGE_MODE_AUDIO;          return cmd;
        }
        Serial.println("Unknown mode");
        return {};
    }
    case 'O':
        if (str_try_cut_prefix(str, "targetFrameTimeMs "))
        {
            cmd.type = CommandType::SET_OPT_targetFrameTimeMs;
            cmd.setOptValue = atoi(str);
        }
        else if (str_try_cut_prefix(str, "autoResetMs "))
        {
            cmd.type = CommandType::SET_OPT_autoResetMs;
            cmd.setOptValue = atoi(str);
        }
        else if (str_try_cut_prefix(str, "text_spaceWidth "))
        {
            cmd.type = CommandType::SET_OPT_text_spaceWidth;
            cmd.setOptValue = atoi(str);
        }
        else if (str_try_cut_prefix(str, "text_trailingWhitespace "))
        {
            cmd.type = CommandType::SET_OPT_text_trailingWhitespace;
            cmd.setOptValue = atoi(str);
        }
        else if (str_try_cut_prefix(str, "text_scrollDirection "))
        {
            cmd.type = CommandType::SET_OPT_text_scrollDirection;
            cmd.setOptValue = atoi(str);
        }
        else
        {
            Serial.println("Unknown option");
        }
        return cmd;
    case 'T':
        cmd.type = CommandType::SET_TEXT;
        cmd.setTextValue = str;
        return cmd;
    default:
        Serial.println("Unknown command");
        return cmd;
    }
}

#undef str_try_cut_prefix

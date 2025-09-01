// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "functionality/options.hpp"

#include <limits.h>

#include "functionality/text/text.hpp"
#include "leddy.hpp"

#define str_try_cut_prefix(s, literal_prefix) (strncmp(s, literal_prefix, sizeof(literal_prefix) - 1) == 0 ? s += sizeof(literal_prefix) - 1 : nullptr)

static int32_t charsToInt(char *str)
{
    uint32_t result = 0;
    uint32_t sign = 1;
    while (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = sign * -1;
        str++;
    }
    while ('0' <= *str && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result * sign;
}

// Glory to global vars!
void processOption(char *option)
{
    if (str_try_cut_prefix(option, "text_spaceWidth "))
    {
        const int32_t val = charsToInt(option);
        if (val < 0)
            return;
        spaceWidth = val;
        initText(nullptr); // NULL => Use same text
        renderText();
    }
    else if (str_try_cut_prefix(option, "text_trailingWhitespace "))
    {
        const int32_t val = charsToInt(option);
        if (val < 0)
            return;
        trailingWhitespace = val * 8;
        initText(nullptr); // NULL => Use same text
        renderText();
    }
    else if (str_try_cut_prefix(option, "text_scrollDirection "))
    {
        const int32_t val = charsToInt(option);
        if (val != -1 && val != 1)
            return;
        scrollDirection = val;
    }
    else if (str_try_cut_prefix(option, "updateDelayMs "))
    {
        const int32_t val = charsToInt(option);
        if (val < 0)
            return;
        g_updateDelayMs = val;
    }
    else if (str_try_cut_prefix(option, "autoResetMs "))
    {
        const int32_t val = charsToInt(option);
        if (val < 0)
            return;
        g_autoResetMs = val;
    }
}

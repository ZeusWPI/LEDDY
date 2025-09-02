// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "leddy.hpp"

#include "Arduino.h"
#include "functionality/audio/audio.hpp"
#include "functionality/options.hpp"
#include "functionality/text/text.hpp"

/**
 * Pin 2 is connected to `CLK`.
 * Pin 3 is connected to `LOAD (~CS)`.
 * Pin 4 is connected to the first `DATA IN`.
 */
LedMatrixChain<ct_ledMatricesPerChain> g_lmcs[ct_ledMatrixChainCount] = {
    LedMatrixChain<ct_ledMatricesPerChain>(2, 3, 4),
};
uint32_t g_updateDelayMs = 0;
uint32_t g_autoResetMs = 0;

static constexpr const char *defaultText = (char*) "\201 Welkom in de kelder! \201 \217\220\0";

enum struct Mode
{
    NONE,
    CLEAR,
    FILL,
    TEXT,
    SCROLLING_TEXT,
    AUDIO
};

static bool inDefaultMode = false;
static enum Mode mode = Mode::NONE;
static const int maxNumChars = 50;
static int receiveIndex = 0;
static char receiveBuffer[maxNumChars]; // An array to store the received data
static uint32_t initInterval = 2000;

static uint32_t lastInitTimestamp = 0;
static uint32_t lastContentChangeTimestamp = 0;
static uint32_t lastRedrawTimestamp = 0;

static void contentChanged()
{
    Serial.println("contentChanged");

    lastContentChangeTimestamp = millis();
    lastRedrawTimestamp = 0;
    inDefaultMode = false;
}

static void defaultMode()
{
    Serial.println("defaultMode");
    initText(defaultText);
    mode = Mode::SCROLLING_TEXT;
    contentChanged();
    inDefaultMode = true;
}

static Mode processCommand()
{
    Serial.print("processCommand: ");
    Serial.println(receiveBuffer);

    switch (receiveBuffer[0])
    {
    case 'U':
        if (receiveBuffer[1] == 'C')
            return Mode::CLEAR;
        if (receiveBuffer[1] == 'F')
            return Mode::FILL;
        break;
    case 'T':
        initText(receiveBuffer + 1, true);
        return Mode::TEXT;
    case 'S':
        initText(receiveBuffer + 1);
        return Mode::SCROLLING_TEXT;
    case 'A':
        return Mode::AUDIO;
    case 'O':
        processOption(receiveBuffer + 1);
        return Mode::NONE;
    default:
        break;
    }
    Serial.println("processCommand error: Unknown command");
    return Mode::NONE;
}

static void initAllMatrices()
{
    Serial.println("initAllMatrices");
    lastInitTimestamp = millis();
    for (size_t i = 0; i < ct_ledMatrixChainCount; i++)
        g_lmcs[i].init();
}

void setup()
{
    Serial.begin(9600);
    initAllMatrices();
    defaultMode();
}

// source: https://forum.arduino.cc/t/serial-input-basics-updated/382007
static void receiveSerial()
{
    char r_char = 0;
    while (Serial.available() > 0)
    {
        r_char = Serial.read();
        if (r_char == '\n')
        {
            receiveBuffer[receiveIndex] = '\0'; // Terminate the string
            if (receiveIndex > 0)
            {
                receiveIndex = 0;
                const Mode newMode = processCommand();
                if (newMode != Mode::NONE)
                {
                    contentChanged();
                    mode = newMode;
                }
            }
            return;
        }
        receiveBuffer[receiveIndex] = r_char;
        receiveIndex++;
        if (receiveIndex >= maxNumChars)
            receiveIndex = maxNumChars - 1;
    }
}

void loop()
{
    receiveSerial();

    if (initInterval > 0 && (millis() - lastInitTimestamp) > initInterval)
        initAllMatrices();

    if (!inDefaultMode && g_autoResetMs > 0 && (millis() - lastContentChangeTimestamp) > g_autoResetMs)
        defaultMode();

    if (g_updateDelayMs > 0 && (millis() - lastRedrawTimestamp) < g_updateDelayMs)
        return;

    switch (mode)
    {
    case Mode::CLEAR:
        for (size_t chain = 0; chain < ct_ledMatrixChainCount; chain++)
            g_lmcs[chain].setAllRows(0x00);
        break;
    case Mode::FILL:
        for (size_t chain = 0; chain < ct_ledMatrixChainCount; chain++)
            g_lmcs[chain].setAllRows(0xFF);
        break;
    case Mode::TEXT:
        renderText();
        break;
    case Mode::SCROLLING_TEXT:
        scrollText();
        renderText();
        break;
    case Mode::AUDIO:
        renderAudio();
        break;
    default:
        break;
    }
    lastRedrawTimestamp = millis();
}

//template<typename T, T val> void static_print(){int _;};
//static void static_printer()
//{
//    //static_print<int, sizeof(size_t)>();
//    //static_print<int, sizeof(int)>();
//    //static_print<int, sizeof(long int)>();
//    //static_print<int, sizeof(long long int)>();
//    //static_print<int, sizeof(float)>();
//    //static_print<int, sizeof(double)>();
//    //static_print<int, A0>();
//    //static_print<int, A1>();
//    //static_print<int, sizeof(g_lmcs)>();
//}

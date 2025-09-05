// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "leddy.hpp"

#include "functionality/audio/audio.hpp"
#include "functionality/text/text.hpp"
#include "serial.hpp"

/**
 * Pin 2 is connected to `CLK`.
 * Pin 3 is connected to `LOAD (~CS)`.
 * Pin 4 is connected to the first `DATA IN`.
 */
LedMatrixChain<ct_ledMatricesPerChain> g_lmcs[ct_ledMatrixChainCount] = {
    LedMatrixChain<ct_ledMatricesPerChain>(2, 3, 4),
};
uint32_t g_targetFrameTimeMs = 10;
uint32_t g_autoResetMs = 15000;

static constexpr const char *defaultText = (char*) "\201 Welkom in de kelder! \201 \217\220\0";

enum struct Mode
{
    NONE,
    CLEAR,
    FILL,
    TEXT,
    SCROLLING_TEXT,
    AUDIO,
};

static bool inDefaultMode = false;
static enum Mode mode = Mode::NONE;
static const int maxNumChars = 50;
static uint32_t initInterval = 2000;

static uint32_t lastInitTimestamp = 0;
static uint32_t lastContentChangeTimestamp = 0;
static uint32_t lastRedrawTimestamp = 0;

static void contentChanged()
{
    lastContentChangeTimestamp = millis();
    lastRedrawTimestamp = 0;
    inDefaultMode = false;
}

static void defaultMode()
{
    setText(defaultText);
    prepareText();
    mode = Mode::SCROLLING_TEXT;
    contentChanged();
    inDefaultMode = true;
}

static void initAllMatrices()
{
    lastInitTimestamp = millis();
    for (size_t i = 0; i < ct_ledMatrixChainCount; i++)
        g_lmcs[i].init();
}

void setup()
{
    initSerial();
    initAllMatrices();
    defaultMode();
}

void processCommand(const Command cmd)
{
    switch (cmd.type)
    {
    case CommandType::CHANGE_MODE_CLEAR:
        mode = Mode::CLEAR;
        contentChanged();
        break;
    case CommandType::CHANGE_MODE_FILL:
        mode = Mode::FILL;
        contentChanged();
        break;
    case CommandType::CHANGE_MODE_TEXT:
        mode = Mode::TEXT;
        prepareText(true);
        contentChanged();
        break;
    case CommandType::CHANGE_MODE_SCROLLING_TEXT:
        mode = Mode::SCROLLING_TEXT;
        prepareText();
        contentChanged();
        break;
    case CommandType::CHANGE_MODE_AUDIO:
        mode = Mode::AUDIO;
        contentChanged();
        break;

    case CommandType::SET_OPT_targetFrameTimeMs:
        if (0 <= cmd.setOptValue)
            g_targetFrameTimeMs = cmd.setOptValue;
        break;
    case CommandType::SET_OPT_autoResetMs:
        if (0 <= cmd.setOptValue)
            g_autoResetMs = cmd.setOptValue;
        break;
    case CommandType::SET_OPT_text_trailingWhitespace:
        if (0 <= cmd.setOptValue)
        {
            g_trailingWhitespace = cmd.setOptValue;
            prepareText(mode == Mode::TEXT);
        }
        break;
    case CommandType::SET_OPT_text_spaceWidth:
        if (0 <= cmd.setOptValue && cmd.setOptValue <= 8)
        {
            g_spaceWidth = cmd.setOptValue;
            prepareText(mode == Mode::TEXT);
        }
        break;
    case CommandType::SET_OPT_text_scrollDirection:
        if (cmd.setOptValue == -1 || cmd.setOptValue == 1)
        {
            g_scrollDirection = cmd.setOptValue;
            prepareText(mode == Mode::TEXT);
        }
        break;

    case CommandType::SET_TEXT:
        contentChanged();
        setText(cmd.setTextValue);
        prepareText(mode == Mode::TEXT);
        break;
    default:
        break;
    }
}

void loop()
{
    processCommand(receiveCommand());

    if (initInterval > 0 && (millis() - lastInitTimestamp) > initInterval)
        initAllMatrices();

    if (!inDefaultMode && g_autoResetMs > 0 && (millis() - lastContentChangeTimestamp) > g_autoResetMs)
        defaultMode();

    if (g_targetFrameTimeMs > 0 && (millis() - lastRedrawTimestamp) < g_targetFrameTimeMs)
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

// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "led_matrix_chain.hpp"

constexpr size_t ct_ledMatrixCount = 12;

extern LedMatrixChain<ct_ledMatrixCount> g_lmc;
extern uint32_t g_targetFrameTimeMs;
extern uint32_t g_autoResetMs;

void setup();
void loop();

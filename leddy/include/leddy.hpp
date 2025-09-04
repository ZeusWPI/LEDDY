// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "led_matrix_chain.hpp"

constexpr size_t ct_ledMatrixChainCount = 1;
constexpr size_t ct_ledMatricesPerChain = 12;
constexpr size_t ct_ledMatrixTotalCount = ct_ledMatrixChainCount * ct_ledMatricesPerChain;

extern LedMatrixChain<ct_ledMatricesPerChain> g_lmcs[ct_ledMatrixChainCount];
extern uint32_t g_targetFrameTimeMs;
extern uint32_t g_autoResetMs;

void setup();
void loop();

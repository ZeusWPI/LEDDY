// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#pragma once

#include <Arduino.h>

/**
  * Wrapper around LedControl.setRow that takes the correct screen array
  * Not used
  */
void setRow(int screen, int row, byte data);

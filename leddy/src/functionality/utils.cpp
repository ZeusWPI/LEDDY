// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "functionality/utils.hpp"

#include "led_control/utils.hpp"
#include "leddy.hpp"

void processUtilCommand(char *command) {
  fillDisplay(strcmp(command, "ClearDisplay") != 0);
}

void fillDisplay(int filled) {
  for (int screen = 0; screen < totalLedSize; screen++) {
    for (int row = 0; row < 8; row++) {
      setRow(screen, row, filled ? 0xFF: 0x00);
    }
  }
}

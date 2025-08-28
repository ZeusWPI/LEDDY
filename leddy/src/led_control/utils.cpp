// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "led_control/utils.hpp"

#include "led_control/led_control.hpp"
#include "leddy.hpp"

void setRow(int screen, int row, byte data) {
  int screenIter = 0;
  int screenNum = screen;
  while (screenNum >= lcs[screenIter].getDeviceCount()) {
    screenNum -= lcs[screenIter].getDeviceCount();
    screenIter++;
  }
  lcs[screenIter].setRow(screenNum, row, data);
}

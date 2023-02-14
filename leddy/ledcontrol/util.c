#include "LedControl.h"

/**
  * Wrapper around LedControl.setRow that takes the correct screen array
  * Not used
  */
void setRow(int screen, int row, byte data) {
  int screenIndex = 0;
  while (screen >= lcs[screenIndex].getDeviceCount()) {
    screen -= lcs[screenIndex].getDeviceCount();
    screenIndex++;
  }
  lcs[screenIndex].setRow(screen, row, data);
}

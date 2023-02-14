#include "LedControl.h"

/**
  * Wrapper around LedControl.setRow that takes the correct screen array
  * Not used
  */
void setRow(int screen, int row, byte data) {
  int screenIter = 0;
  int screenNum = screen;
  while (screenNum >= lcs[screenIter].getDeviceCount()) {
    screenNum -= lcs[screenIter].getDeviceCount();
    screenIter++;
  }
  lcs[screenIter].setRow(screenNum, row, data);
}

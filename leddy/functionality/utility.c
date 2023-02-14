#include "utility.h"

void processUtilCommand(char *command) {
  if (!strcmp(command, "ClearDisplay")) {
    clearDisplay();
  } else if (!strcmp(command, "FillDisplay")) {
    fillDisplay();
  }
}

void clearDisplay() {
  for (int screen = 0; screen < totalLedSize; screen++) {
    for (int row = 0; row < 8; row++) {
      setRow(screen, row, 0x00);
    }
  }
}

void fillDisplay() {
  for (int screen = 0; screen < totalLedSize; screen++) {
    for (int row = 0; row < 8; row++) {
      setRow(screen, row, 0xFF);
    }
  }
}

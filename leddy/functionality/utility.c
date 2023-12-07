#include "utility.h"

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

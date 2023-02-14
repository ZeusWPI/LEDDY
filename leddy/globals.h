#include "ledcontrol/LedControl.h"

#ifndef GLOBALS_H_
#define GLOBALS_H_

int amountOfScreens = 4;

LedControl lcs[4] = {
/* Create a new LedControl variable.
 * We use pins 12,11 and 10 on the Arduino for the SPI interface
 * Pin 12 is connected to the DATA IN-pin of the first MAX7221
 * Pin 11 is connected to the CLK-pin of the first MAX7221
 * Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
 * Amount of MAX7221 modules attached to the arduino
 */
  LedControl(4, 2, 3, 3),
  LedControl(7, 5, 6, 3),
  LedControl(10, 8, 9, 3),
  LedControl(13, 11, 12, 3)
};
int totalLedSize = 12;

#endif /* GLOBALS_H_ */
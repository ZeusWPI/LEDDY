// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
#include "leddy.hpp"

#include <time.h>

#include "functionality/audio/audio.hpp"
#include "functionality/options.hpp"
#include "functionality/text/text.hpp"
#include "functionality/utils.hpp"
#include "led_control/led_control.hpp"

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

unsigned long updateDelayMs = 0;
unsigned long autoResetMs = 0;


const char *defaultText = (const char *)"\201 Welkom in de kelder! \201 \217\220\0";

bool isReset = false;
enum mode_t mode = SCROLLING_TEXT;

void setup() {
  Serial.begin(9600);

  // clear and set intensity
  for (int i = 0; i < amountOfScreens; i++) {
    for (int j = 0; j < lcs[i].getDeviceCount(); j++) {
      lcs[i].shutdown(j, false);
      lcs[i].setIntensity(j, 15);
      lcs[i].clearDisplay(j);
    }
  }

  reset();
}

// Note: overflows once every month
unsigned long lastUpdate = millis();
unsigned long lastContentChange = millis();

void loop() {
  receiveSerial();

  if (!isReset && autoResetMs > 0 && (millis() - lastContentChange) > autoResetMs)
    reset();

  switch (mode) {
    case SCROLLING_TEXT:
      if (updateDelayMs == 0 || (millis() - lastUpdate) > updateDelayMs) {
        scrollText();
        renderText();
        lastUpdate = millis();
      }
      break;
    case STATIC:
      break;
    case AUDIO:
      renderAudio();
      break;
    default:
      break;
  }
}

void reset() {
  initText(defaultText);
  mode = SCROLLING_TEXT;
  isReset = true;
}

void contentChanged() {
  lastContentChange = millis();
  isReset = false;
}

const int maxNumChars = 50;
int receiveIndex = 0;
char receiveBuffer[maxNumChars];   // an array to store the received data

// source: https://forum.arduino.cc/t/serial-input-basics-updated/382007
void receiveSerial() {
  char r_char;

  while (Serial.available() > 0) {
    r_char = Serial.read();

    if (r_char == '\n') {
      receiveBuffer[receiveIndex] = '\0'; // Terminate the string
      if (receiveIndex > 0) {
        receiveIndex = 0;
        processCommand();
      }
      return;
    }

    receiveBuffer[receiveIndex] = r_char;
    receiveIndex++;
    if (receiveIndex >= maxNumChars) {
      receiveIndex = maxNumChars - 1;
    }
  }
}

void processCommand() {
  // Serial.print("Processing text: ");
  // Serial.println(receiveBuffer);

  switch (receiveBuffer[0]) {
    case 'U':
      processUtilCommand(receiveBuffer+1);
      mode = STATIC;
      contentChanged();
      break;
    case 'T':
      initText(receiveBuffer+1, true);
      renderText();
      mode = STATIC;
      contentChanged();
      break;
    case 'S':
      initText(receiveBuffer+1);
      renderText();
      mode = SCROLLING_TEXT;
      contentChanged();
      break;
    case 'A':
      mode = AUDIO;
      contentChanged();
      break;
    case 'O':
      processOption(receiveBuffer+1);
      break;
    default:
      break;
  }
}

//template<typename T, T val> constexpr void static_print(){int _;};
//static void static_printer()
//{
//    //static_print<int, sizeof(size_t)>();
//    //static_print<int, sizeof(int)>();
//    //static_print<int, sizeof(long int)>();
//    //static_print<int, sizeof(long long int)>();
//    //static_print<int, sizeof(float)>();
//    //static_print<int, sizeof(float)>();
//}

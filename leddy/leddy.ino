#include "globals.h"
#include "time.h"
#include "ledcontrol/LedControl.cpp" // LedControl by Eberhard Fahle <e.fahle@wayoda.org> v1.0.6
#include "functionality/text/text.c"
#include "functionality/audio/audio.c"
#include "functionality/utility.c"
#include "functionality/options.c"

const unsigned char *defaultText = "\201 Welkom in de kelder! \201 \217\220\0";

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

unsigned long lastUpdate = getCurrentTime();
unsigned long lastContentChange = getCurrentTime();

void loop() {
  receiveSerial();

  if (!isReset && autoResetMs > 0 && (getCurrentTime() - lastContentChange) > autoResetMs)
    reset();

  switch (mode) {
    case SCROLLING_TEXT:
      if (updateDelayMs == 0 || (getCurrentTime() - lastUpdate) > updateDelayMs) {
        scrollText();
        renderText();
        lastUpdate = getCurrentTime();
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
  lastContentChange = getCurrentTime();
  isReset = false;
}

const int maxNumChars = 50;
int receiveIndex = 0;
char receiveBuffer[maxNumChars];   // an array to store the received data

// source: https://forum.arduino.cc/t/serial-input-basics-updated/382007
char** receiveSerial() {
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

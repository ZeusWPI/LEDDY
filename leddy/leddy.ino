#include "globals.h"
#include "ledcontrol/LedControl.cpp" // LedControl by Eberhard Fahle <e.fahle@wayoda.org> v1.0.6
#include "functionality/text/text.c"

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

  unsigned char *text = "\201 Welkom in de kelder! \201 \217\220\0";
  initScrollingText(text);
}

const byte maxNumChars = 64;
char receiveBuffer[maxNumChars];   // an array to store the received data

void loop() {
  receiveSerial();
  updateScrollingText();
}

// source: https://forum.arduino.cc/t/serial-input-basics-updated/382007
char** receiveSerial() {
  int index = 0;
  char r_char;

  while (Serial.available() > 0) {
    r_char = Serial.read();

    if (r_char == '\n') { break; }

    receiveBuffer[index] = r_char;
    index++;
    if (index >= maxNumChars) {
      index = maxNumChars - 1;
    }
  }
  receiveBuffer[index] = '\0'; // Terminate the string
  if (index > 0) {
    processCommand();
  }
}

void processCommand() {
  Serial.print("Preprocessing text: ");
  Serial.println(receiveBuffer);
  processString(receiveBuffer);
}

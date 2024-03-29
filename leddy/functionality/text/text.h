#ifndef TEXT_H_
#define TEXT_H_

#include "font.h"

// GLOBAL TEXT VARS
char text[50] = {0};
size_t textBufferSize = 460;
byte textBuffer[460]; // if text is 64 chars, then big enough

int trailingWhitespace = 6 * 8; // Four modules between loops
int spaceWidth = 6;
int currentTextIndex = 0;
int scrollDirection = 1;
// GLOBAL TEXT VARS


/**
  * Initialize the `text` buffer
  *   padBuffer: pad the text buffer with spaces to fille the whole screen
  */
void initText(unsigned char* text, bool padBuffer = false);

/**
  * Move the Text one over
  */
void scrollText();

/**
  * Draw the `text` buffer
  */
void renderText();

#endif /* TEXT_H_ */

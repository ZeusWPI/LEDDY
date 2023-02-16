#ifndef TEXT_H_
#define TEXT_H_

#include "font.h"

// GLOBAL TEXT VARS
size_t textBufferSize = 600;
byte textBuffer[600]; // if text is 64 chars, then big enough

int trailingWhitespace = 6 * 8; // Four modules between loops
int spaceWidth = 6;
int currentTextIndex = 0;
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

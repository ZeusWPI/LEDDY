#ifndef TEXT_H_
#define TEXT_H_

#include "font.h"

// GLOBAL TEXT VARS
size_t textBufferSize = 600;
byte textBuffer[600]; // if text is 64 chars, then big enough

int trailingWhitespace = 4 * 8; // Four modules between loops
int spaceWidth = 6;
int currentTextIndex = 0;
// GLOBAL TEXT VARS


/**
  * Initialize ScrollingText
  */
void initScrollingText(unsigned char* text);

/**
  * Draw ScrollingText and move it one over
  */
void updateScrollingText();

#endif /* TEXT_H_ */

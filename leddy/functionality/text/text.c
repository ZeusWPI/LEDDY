#include "text.h"

#include "../../ledcontrol/LedControl.h"
#include "../../ledcontrol/util.c"

/**
 * Helper function to get a bit from a byte.
 */
bool getBit(byte input, int bit) {
  return (input & (0x80 >> bit)) > 0;
}

/**
 * Helper function to change a bit in a byte.
 */
byte setBit(byte input, int bit, bool value) {
    byte mask = value << (7 - bit);  // example 00010000
    return (input & (~mask)) | mask;
}

/**
 * Characters are stored as a byte per row, we want to store them as a byte per
 * column.
 */
void convertCharacterRowsToColumns(unsigned char character, byte *columnBased) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            columnBased[i] =
                setBit(columnBased[i], j,
                       getBit(pgm_read_byte(&(font8x8_basic[character][7 - j])),
                              7 - i));
        }
    }
}

/**
 *  Squash spaces in buffer. The buffer is filled up to length
 *  Returns the new length (<= original length)
 */
int squashSpaces(byte *buffer, int length) {
    int newLength = length;
    int i = 0;
    while (i < newLength) {
        if (!buffer[i]) {  // A zero row
            int spaces = 0;
            // while not at the end and there is a space
            while (i + spaces < newLength && !buffer[i + spaces]) {
                spaces++;
            }
            if (i + spaces == newLength) {
                // At the end of the buffer, trim spaces and return new length
                return newLength - spaces;
            }
            // In the input, a space character will be 8 space columns.
            // Either squash to spaceWidth spaces or 1 space
            int toShift = spaces >= 8 ? spaces - spaceWidth : spaces - 1;
            if (toShift > 0) {
                for (int j = i + spaces; j < newLength; j++) {
                    buffer[j - toShift] = buffer[j];
                }
                newLength = newLength - toShift;
            }
            i = i + spaces - toShift;
        }
        i++;
    }
    return newLength;
}

/**
 *  Preprocess the text into the textBuffer buffer
 */
void processString(unsigned char *text, bool padBuffer = false) {
    // clear buffer, just to be sure
    for (int i = 0; i < textBufferSize; i++) {
        textBuffer[i] = 0;
    }

    // setup textBuffer
    for (int i = 0; i < strlen(text); i++) {
        byte columnBasedCharacter[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        convertCharacterRowsToColumns(text[i], columnBasedCharacter);
        for (int j = 0; j < 8; j++) {
            textBuffer[i * 8 + j] = columnBasedCharacter[j];
        }
    }

    // Squash space between letters
    int squashedSize = squashSpaces(textBuffer, strlen(text) * 8);
    textBufferSize = squashedSize + trailingWhitespace;
    if (padBuffer) {
        textBufferSize = 12 * 8;
    }
    for (int i = squashedSize; i < textBufferSize; i++) {
        textBuffer[i] = 0;
    }
}

void initText(unsigned char *text, bool padBuffer = false) {
    currentTextIndex = 0;
    processString(text, padBuffer);
}

void scrollText() {
    currentTextIndex = (currentTextIndex + 1) % textBufferSize;
}

void renderText() {
    int bufferIndex = currentTextIndex;
    // for each screen
    for (int screenIndex = 0; screenIndex < amountOfScreens; screenIndex++) {
        // for each row of a matrix contained in that screen
        for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
            // set row i of each matrix of the screen simultaneous
            lcs[screenIndex].setRowParallel(textBuffer, textBufferSize, bufferIndex + rowIndex, rowIndex);
        }
        bufferIndex += 8*lcs[screenIndex].getDeviceCount();
    }
}

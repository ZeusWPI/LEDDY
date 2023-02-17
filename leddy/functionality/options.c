#include "utility.h"
#include "limits.h"

int charsToInt(char *string) {
    int val = atoi(string);
    if ((val == 0 && string[0] != '0') || val >= INT_MAX || val < INT_MIN) {
        return -69;
    }
    return val;
}

// Glory to global vars!
void processOption(char *option) {
  if (!strncmp(option, "text_spaceWidth ", 16)) {
    int val = charsToInt(option + 16);
    if (val < 0) { return; }
    spaceWidth = val;
    initText(nullptr);  // NULL => Use same text
    renderText();
  } else if (!strncmp(option, "text_trailingWhitespace ", 24)) {
    int val = charsToInt(option + 24);
    if (val < 0) { return; }
    trailingWhitespace = val * 8;
    initText(nullptr);  // NULL => Use same text
    renderText();
  } else if (!strncmp(option, "updateDelayMs ", 14)) {
    int val = charsToInt(option + 14);
    if (val < 0) { return; }
    updateDelayMs = val;
  }
}

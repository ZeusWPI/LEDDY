#include "utility.h"
#include "limits.h"

long charsToInt(char *str) {
  long result = 0;
  long sign = 1;
  while (('-' == (*str)) || ((*str) == '+')) {
      if (*str == '-')
        sign = sign * -1;
      str++;
  }
  while ((*str >= '0') && (*str <= '9')) {
      result = (result * 10) + ((*str) - '0');
      str++;
  }
  return result * sign;
}

// Glory to global vars!
void processOption(char *option) {
  if (!strncmp(option, "text_spaceWidth ", 16)) {
    long val = charsToInt(option + 16);
    if (val < 0) { return; }
    spaceWidth = val;
    initText(nullptr);  // NULL => Use same text
    renderText();
  } else if (!strncmp(option, "text_trailingWhitespace ", 24)) {
    long val = charsToInt(option + 24);
    if (val < 0) { return; }
    trailingWhitespace = val * 8;
    initText(nullptr);  // NULL => Use same text
    renderText();
  } else if (!strncmp(option, "text_scrollDirection ", 21)) {
    long val = charsToInt(option + 21);
    if (val == -1) { scrollDirection = -1; }
    else if (val == 1) { scrollDirection = 1; }
  } else if (!strncmp(option, "updateDelayMs ", 14)) {
    long val = charsToInt(option + 14);
    if (val < 0) { return; }
    updateDelayMs = val;
  } else if (!strncmp(option, "autoResetMs ", 12)) {
    long val = charsToInt(option + 12);
    if (val < 0) { return; }
    autoResetMs = val;
  }
}

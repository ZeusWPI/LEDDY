## LED DisplaY

### Control LEDDY

There is a **web interface on http://leddy** to control everything.

There is also an API: send an HTTP POST request to http://leddy with the following plaintext body:
```
COMMAND [command opts ...]
```

Example:
```bash
$ curl leddy -X POST --data 'ScrollingText Welkom in de kelder!'
```

Available commands are as follows:
```
Commands:
    Text <text>
    ScrollingText <text>

    ClearDisplay
    FillDisplay

    Audio

    Option targetFrameTimeMs <milliseconds>
    Option autoResetMs <milliseconds>
    Option text_spaceWidth <columns>
    Option text_trailingWhitespace <columns>
    Option text_scrollDirection 1
    Option text_scrollDirection -1
```

### Hardware

- ESP8266 aka Servy: handles outside communication
- Arduino Uno: controlling the LEDS
- 12x MAX7219 Led display (8x8 leds)
- Wired like this _(source: the internet)_:
<img src="https://user-images.githubusercontent.com/47608311/211937380-e48b0876-e36b-4873-9c20-ebee0466bb67.png" height="500px" />


### Leddy

#### Notes on LED control implementation

- All displays are chained together into one big shift register, each display holding 16 bits.
  The protocol is described [here](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX7219-MAX7221.pdf), and is implemented by the `LedMatrixChain` class, of which `g_lmc` is an instance.
- To set or unset leds on the display, typically you would use:
    - `g_lmc.setRows(uint8_t row, uint8_t pixels[12])`: Sets all `96` pixels of a single row of the ledstrip.
      The MSB of the first byte of the first row is the top-left pixel.
- As for text, each character is stored as 8 bytes resulting in an 8 by 8 grid of bits
- We do some weird 'transposing' of the text string, so that the list of bytes to display are columns when looking at the led displays.
  To display what we call a sliding window, we just have to send the correct columns to the displays.
- The font is originally from Marcel Sondaar, public domain, made available [here](https://github.com/dhepper/font8x8).

#### Development

Requires the `platformio` cli tool, follow [these instructions](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html) to install it.

Afterwards, build and flash using the following:
```
. ~/.platformio/penv/bin/activate # Might depend on platform
pio run -t clean
pio run
pio run -t upload -t monitor
```

You can run `platformio project init --ide [editor]` to generate editor-specific files.

#### Servy

- Hostame: `leddy`(`.kelder.local`).
- Running [MicroPython](https://docs.micropython.org/en/latest/esp8266/tutorial/intro.html#getting-the-firmware)
- Write files to microcontroller. `boot.py` and `main.py` are executed on boot.
    1. Connected through serial with `ampy`:
        ```bash
        $ ampy -p /dev/ttyUSB0 -b 115200 put main.py main.py
        # Or directly run file while developing
        $ ampy -p /dev/ttyUSB0 -b 115200 run main.py
        ```
    2. Use the WebREPL in `servy/webrepl/webrepl.html` (submodule) to update or execute code on Servy.
        Connect with `ws://leddy:8266`, password = `zeus`.

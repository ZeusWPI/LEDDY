## LED DisplaY

#### Control LEDDY

Currently only through HTTP POST requests.
```
Usage: HTTP Post with the following plaintext body:

COMMAND [command opts ...]

Commands:
    ScrollingText <text>
    ClearDisplay
```
Example:
```bash
$ echo "ScrollingText Welkom in de kelder!" | http POST 10.1.0.181
```

#### Hardware

- ESP8266 aka Servy: handles outside communication
- Arduino Uno: controlling the LEDS
- 12x MAX7219 Led display (8x8 leds)
- Wired like this _(source: the internet)_:
<img src="https://user-images.githubusercontent.com/47608311/211937380-e48b0876-e36b-4873-9c20-ebee0466bb67.png" height="500px" />


#### Notes on LED control implementation

- The LedControl library can only handle 8 displays chained. We have 12 displays. 4 chains of each 3 displays. Displays are chained on the same bus, more displays on the same bus causes slow text scrolling. This is the reason they are split into 4 different busses with each only 3 displays.
- To set or unset leds on the display:
    - `setLed(..., bool state)` sets one led on or off
    - `setRow(..., byte value)` sets a row of 8 leds, each led _i_ on or off depending on the bit _i_ of the `value`-byte
    - `setColumn`, analogous to the above
- Each character is stored as 8 bytes resulting in an 8 by 8 grid of bits
- We do some weird 'transposing' of the text string, so that the list of bytes to display are columns when looking at the led displays. To display what we call a sliding window, we just have to send the correct columns to the displays.
- The font is originally from Marcel Sondaar, public domain, made available [here](https://github.com/dhepper/font8x8)

#### Servy

- IP: 10.1.0.181
- Running [MicroPython](https://docs.micropython.org/en/latest/esp8266/tutorial/intro.html#getting-the-firmware)
- Write files to microcontroller. `boot.py` and `main.py` are executed on boot.
    1. Connected through serial with `ampy`:
        ```bash
        $ ampy -p /dev/ttyUSB0 -b 115200 put main.py main.py
        # Or directly run file while developing
        $ ampy -p /dev/ttyUSB0 -b 115200 run main.py
        ```
    2. Use the WebREPL in `servy/webrepl/webrepl.html` (submodule) to update or execute code on Servy.
        Connect with `ws://10.1.0.181:8266`. password = `led=demax`

#### Development

- Arduino IDE
- Dependencies:
    - Modified version of [LedControl](http://wayoda.github.io/LedControl/) by Eberhard Fahle <e.fahle@wayoda.org> v1.0.6. The files are included in this repository.

#### Setup

Simply clone this repository inside `~/Arduino` folder. Open `leddy/leddy.ino` inside the Arduino IDE and select your arduino board at the top of your editor.


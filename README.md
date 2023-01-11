## LED DisplaY

#### Hardware

- Arduino Uno
- 11x MAX7219 Led display (8x8 leds)
- Wired like this _(source: the internet)_:
<img src="https://user-images.githubusercontent.com/47608311/211937380-e48b0876-e36b-4873-9c20-ebee0466bb67.png" height="500px" />


#### Notes on implementation

- The LedControl library can only handle 8 displays chained. We have 11 displays so we have two chains of 8 and 3 displays.
- To set or unset leds on the display:
    - `setLed(..., bool state)` sets one led on or off
    - `setRow(..., byte value)` sets a row of 8 leds, each led _i_ on or off depending on the bit _i_ of the `value`-byte
    - `setColumn`, analogous to the above
- Each character is stored as 8 bytes resulting in an 8 by 8 grid of bits
- We do some weird 'transposing' of the text string, so that the list of bytes to display are columns when looking at the led displays. To display what we call a sliding window, we just have to send the correct columns to the displays.

#### Development

- Arduino IDE
- Dependencies:
    - [LedControl](http://wayoda.github.io/LedControl/) by Eberhard Fahle <e.fahle@wayoda.org> v1.0.6


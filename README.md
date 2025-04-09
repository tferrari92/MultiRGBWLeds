# MultiRGBWLeds

An Arduino library to control up to four RGBW LED modules independently, with built-in support for:

- Flash patterns (with different brightness/duration options)
- Crossfades between colors
- Side-to-side animations (on X or Y axis)
- Spinning effects (clockwise/anticlockwise)
- Resetting specific or all lamp positions

## Installation

1. [Download the library](https://github.com/tferrari92/MultiRGBWLeds/archive/refs/tags/v1.0.0.zip)
2. Open Arduino IDE
3. Go to Sketch > Include Library > Add .ZIP Library...
4. Select the `.zip` you just downloaded

## Basic Usage

```cpp
#include <MultiRGBWLeds.h>

void setup() {
    int backLeft[4] = {12, 13, 14, 15}; // {R, G, B, W}
    int frontLeft[4] = {8, 9, 10, 11};  // {R, G, B, W}
    int frontRight[4] = {4, 5, 6, 7};   // {R, G, B, W}
    int backRight[4] = {0, 1, 2, 3};    // {R, G, B, W}
    MultiRGBWLeds::begin(backLeft, frontLeft, frontRight, backRight);
    MultiRGBWLeds::set(FRONT_LEFT, RED, 255);
}

void loop() {
    MultiRGBWLeds::flash(FRONT_LEFT, BLUE, 200, 500); // Blink blue every 500ms
}
```

## Supported Effects

- `set()`
- `flash()`
- `crossFade()`
- `sideToSide()`
- `spin()`
- `resetPosition()`
- `resetAllPositions()`

See the [examples](examples/) folder for more.

## License

MIT

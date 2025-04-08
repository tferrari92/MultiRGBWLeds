# MultiRGBWLeds

An Arduino library to control up to four RGBW LED modules independently, with built-in support for:

- Flash patterns (with different brightness/duration options)
- Crossfades between colors
- Side-to-side animations (on X or Y axis)
- Spinning effects (clockwise/anticlockwise)
- Resetting specific or all lamp positions

## Installation

1. Download the library from the [GitHub Releases Page](https://github.com/tferrari92/MultiRGBWLeds/releases)
2. Open Arduino IDE
3. Go to Sketch > Include Library > Add .ZIP Library...
4. Select the `.zip` you just downloaded

## Basic Usage

```cpp
#include <MultiRGBWLeds.h>

int backLeft[] = {3, 5, 6, 9};     // R, G, B, W PWM pins
int frontLeft[] = {10, 11, 12, 13};
int frontRight[] = {A0, A1, A2, A3};
int backRight[] = {A4, A5, 2, 4};

void setup() {
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

# MultiRGBWLeds

An Arduino library to control up to four **RGBW** lamps independently through a
[PCA9685](https://www.adafruit.com/product/815) 16-channel PWM driver, with built-in
animations:

- `set` — solid colors
- `crossFade` — smooth, sine-eased color/brightness fades
- `flash` — blink on/off
- `sideToSide` — swap two colors across an axis
- `spin` — rotate a color wheel around the four corners
- `resetPosition` / `resetAllPositions` — turn lamps off

## Hardware

- A **PCA9685** PWM driver on I²C (default address `0x40`).
- Up to **four RGBW lamps**. Each lamp uses four PCA9685 channels — one each for
  R, G, B, W. With four lamps that's all 16 channels.

> The channel arrays you pass to `begin()` are **PCA9685 channel numbers (0–15)**,
> not Arduino pins.

## Dependencies

- [Adafruit PWM Servo Driver Library](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)
  (install from the Library Manager; pulls in Adafruit BusIO).

## Installation

**Library Manager:** search for **MultiRGBWLeds** and install (this installs the
Adafruit dependency automatically).

**Manual:** [download the latest release ZIP](https://github.com/tferrari92/MultiRGBWLeds/releases),
then in the Arduino IDE: *Sketch → Include Library → Add .ZIP Library…*

## Usage

```cpp
#include <MultiRGBWLeds.h>

// PCA9685 channel numbers per lamp, in {R, G, B, W} order (0-15).
const uint8_t backLeft[4]   = {12, 13, 14, 15};
const uint8_t frontLeft[4]  = {8, 9, 10, 11};
const uint8_t frontRight[4] = {4, 5, 6, 7};
const uint8_t backRight[4]  = {0, 1, 2, 3};

MultiRGBWLeds leds;

void setup() {
    // Creates and configures its own PCA9685 at 0x40.
    leds.begin(backLeft, frontLeft, frontRight, backRight);

    // Brightness is 0-255.
    leds.set(LampPosition::FrontLeft, LampColor::Red, 255);
}

void loop() {
    // Blink the front-left lamp blue: 200ms on, 300ms off.
    leds.flash(LampPosition::FrontLeft, LampColor::Blue, 255, 200, 300);
}
```

### Brightness

Brightness is **0–255** everywhere (0 = off, 255 = full), matching `analogWrite()`
and Adafruit NeoPixel.

### Colors

`LampColor`: `Off`, `Red`, `Orange`, `Yellow`, `Lime`, `Green`, `Cyan`, `Aqua`,
`Sky`, `Blue`, `Indigo`, `Magenta`, `Pink`, `White`.

### Positions

`LampPosition`: `BackLeft`, `FrontLeft`, `FrontRight`, `BackRight`.

### Custom I²C address / frequency

```cpp
leds.begin(backLeft, frontLeft, frontRight, backRight, 0x41, 1600.0);
```

### Bringing your own driver

To share the I²C bus or drive several boards, pass an existing
`Adafruit_PWMServoDriver` (you configure it yourself):

```cpp
Adafruit_PWMServoDriver myDriver(0x40, Wire);

void setup() {
    Wire.begin();
    myDriver.begin();
    myDriver.setPWMFreq(1600);
    leds.begin(backLeft, frontLeft, frontRight, backRight, myDriver);
}
```

## Examples

See the [`examples/`](examples/) folder: `BasicSet`, `CrossFade`, `FlashSingle`,
`FlashMulti`, `SideToSide`, `Spin`, `ResetDemo`.

## Notes

The animation calls (`crossFade`, `flash`, `sideToSide`, `spin`) are **blocking** —
they run to completion before returning. That suits sequential, choreographed shows.

## License

[MIT](LICENSE) © Tomas Ferrari

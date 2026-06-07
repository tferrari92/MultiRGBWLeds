#include <MultiRGBWLeds.h>

// Each lamp is four PCA9685 channel numbers (0-15) in {R, G, B, W} order.
// These are PCA9685 channels, NOT Arduino pins.
const uint8_t backLeft[4]   = {12, 13, 14, 15};
const uint8_t frontLeft[4]  = {8, 9, 10, 11};
const uint8_t frontRight[4] = {4, 5, 6, 7};
const uint8_t backRight[4]  = {0, 1, 2, 3};

MultiRGBWLeds leds;

void setup()
{
    // This example uses Linear mode: nominal RGB colors, brightness 0-255.
    // (Omit the ColorMode argument for Calibrated mode + 0-10 brightness.)
    leds.begin(backLeft, frontLeft, frontRight, backRight, ColorMode::Linear);
}

void loop()
{
    // Fade all four lamps from off up to full red over 2s...
    leds.crossFade(
        LampPosition::BackLeft, LampColor::Red, 0, LampColor::Red, 255,
        LampPosition::FrontLeft, LampColor::Red, 0, LampColor::Red, 255,
        LampPosition::FrontRight, LampColor::Red, 0, LampColor::Red, 255,
        LampPosition::BackRight, LampColor::Red, 0, LampColor::Red, 255,
        2000);

    // ...then fade red down to off over 2s.
    leds.crossFade(
        LampPosition::BackLeft, LampColor::Red, 255, LampColor::Blue, 0,
        LampPosition::FrontLeft, LampColor::Red, 255, LampColor::Blue, 0,
        LampPosition::FrontRight, LampColor::Red, 255, LampColor::Blue, 0,
        LampPosition::BackRight, LampColor::Red, 255, LampColor::Blue, 0,
        2000);
}

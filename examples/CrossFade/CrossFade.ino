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
    leds.begin(backLeft, frontLeft, frontRight, backRight);
}

void loop()
{
    // Fade all four lamps from red up to full, then down to off, over 2s each.
    leds.crossFade(
        LampPosition::BackLeft, LampColor::Red, 0, LampColor::Red, 255,
        LampPosition::FrontLeft, LampColor::Red, 0, LampColor::Red, 255,
        LampPosition::FrontRight, LampColor::Red, 0, LampColor::Red, 255,
        LampPosition::BackRight, LampColor::Red, 0, LampColor::Red, 255,
        2000);

    leds.crossFade(
        LampPosition::BackLeft, LampColor::Red, 255, LampColor::Blue, 0,
        LampPosition::FrontLeft, LampColor::Red, 255, LampColor::Blue, 0,
        LampPosition::FrontRight, LampColor::Red, 255, LampColor::Blue, 0,
        LampPosition::BackRight, LampColor::Red, 255, LampColor::Blue, 0,
        2000);
}

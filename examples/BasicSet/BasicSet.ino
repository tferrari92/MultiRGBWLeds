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

    // Set each corner to a solid color at full brightness (0-255).
    leds.set(
        LampPosition::BackLeft, LampColor::Red, 255,
        LampPosition::FrontLeft, LampColor::Green, 255,
        LampPosition::FrontRight, LampColor::Blue, 255,
        LampPosition::BackRight, LampColor::White, 255);
}

void loop()
{
    // Static scene; nothing to update.
}

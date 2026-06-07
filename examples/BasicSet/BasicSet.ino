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
    // Default mode is Calibrated -> brightness is 0-10 (full = 10).
    // For nominal RGB on a 0-255 scale, use ColorMode::Linear (see CrossFade).
    leds.begin(backLeft, frontLeft, frontRight, backRight);

    leds.set(
        LampPosition::BackLeft, LampColor::Red, 10,
        LampPosition::FrontLeft, LampColor::Green, 10,
        LampPosition::FrontRight, LampColor::Blue, 10,
        LampPosition::BackRight, LampColor::White, 10);
}

void loop()
{
    // Static scene; nothing to update.
}

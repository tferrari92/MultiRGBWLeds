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
    // Default Calibrated mode -> brightness is 0-10 (full = 10).
    leds.begin(backLeft, frontLeft, frontRight, backRight);
}

void loop()
{
    // Light all four corners white.
    leds.set(
        LampPosition::BackLeft, LampColor::White, 10,
        LampPosition::FrontLeft, LampColor::White, 10,
        LampPosition::FrontRight, LampColor::White, 10,
        LampPosition::BackRight, LampColor::White, 10);
    delay(1000);

    // Turn off just one corner.
    leds.resetPosition(LampPosition::FrontRight);
    delay(1000);

    // Turn everything off.
    leds.resetAllPositions();
    delay(1000);
}

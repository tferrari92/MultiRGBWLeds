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
    // Flash the two front lamps yellow, then the two back lamps yellow.
    leds.flash(
        LampPosition::FrontLeft, LampColor::Yellow, 10,
        LampPosition::FrontRight, LampColor::Yellow, 10,
        150, 100);

    leds.flash(
        LampPosition::BackLeft, LampColor::Yellow, 10,
        LampPosition::BackRight, LampColor::Yellow, 10,
        150, 100);
}

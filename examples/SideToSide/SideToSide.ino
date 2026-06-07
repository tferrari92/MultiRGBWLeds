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
    // Swap green/indigo between the left and right pairs, 4 times,
    // holding each side for 400ms.
    leds.sideToSide(Axis::LeftRight, LampColor::Green, LampColor::Indigo, 400, 4);

    // Now swap front/back instead.
    leds.sideToSide(Axis::FrontBack, LampColor::Red, LampColor::Blue, 400, 4);
}

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
    // One color chasing around the four corners, clockwise, 2 rotations.
    // stepMs is the crossfade time between adjacent corners.
    leds.spin(LampColor::Aqua, Direction::Clockwise, 120, 2);

    // Two alternating colors, the other way.
    leds.spin(LampColor::Red, LampColor::Blue, Direction::Anticlockwise, 120, 2);
}

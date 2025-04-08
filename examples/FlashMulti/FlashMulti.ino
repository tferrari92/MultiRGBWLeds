#include <MultiRGBWLeds.h>

int backLeft[] = {3, 5, 6, 9};     // R, G, B, W
int frontLeft[] = {10, 11, 12, 13};
int frontRight[] = {A0, A1, A2, A3};
int backRight[] = {A4, A5, 2, 4};

void setup() {
    MultiRGBWLeds::begin(backLeft, frontLeft, frontRight, backRight);
}

void loop() {
    // FlashMulti demo
}

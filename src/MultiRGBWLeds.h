#ifndef MultiRGBWLeds_h
#define MultiRGBWLeds_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Lamp positions, named for a four-corner layout.
// `None` is a sentinel for the optional positions of the multi-lamp calls.
enum class LampPosition : uint8_t
{
    BackLeft,
    FrontLeft,
    FrontRight,
    BackRight,
    None
};

// Predefined colors. Each maps to a relative R/G/B/W mix (see the color table
// in the .cpp); brightness is applied on top at call time.
enum class LampColor : uint8_t
{
    Off,
    Red,
    Orange,
    Yellow,
    Lime,
    Green,
    Cyan,
    Aqua,
    Sky,
    Blue,
    Indigo,
    Magenta,
    Pink,
    White
};

// Axis for the sideToSide() animation.
enum class Axis : uint8_t
{
    LeftRight,
    FrontBack
};

// Rotation direction for the spin() animation.
enum class Direction : uint8_t
{
    Clockwise,
    Anticlockwise
};

// Color model, chosen in begin():
//   Calibrated - hues empirically tuned so colors read true on real RGBW
//                lamps; brightness is 0-10.
//   Linear     - nominal RGB color values, scaled linearly; brightness is
//                0-255 (the analogWrite()/NeoPixel convention).
enum class ColorMode : uint8_t
{
    Calibrated,
    Linear
};

// Controls up to four RGBW lamps wired to a PCA9685 16-channel PWM driver.
//
// The color model and brightness range are chosen in begin() via ColorMode
// (default Calibrated -> brightness 0-10; Linear -> brightness 0-255).
//
// Each lamp is described by an array of four PCA9685 channel numbers (0-15) in
// {R, G, B, W} order -- these are PCA9685 channels, NOT Arduino pins.
class MultiRGBWLeds
{
public:
    static constexpr uint8_t kLampCount = 4;
    static constexpr uint8_t kChannelsPerLamp = 4;
    static constexpr uint16_t kPwmMax = 4095; // PCA9685 is 12-bit
    static constexpr uint8_t kDefaultAddress = 0x40;
    static constexpr float kDefaultFrequencyHz = 1600.0f;
    static constexpr uint8_t kCalibratedBrightnessMax = 10;
    static constexpr uint8_t kLinearBrightnessMax = 255;

    MultiRGBWLeds() = default;

    // Initialize with an internally-owned PCA9685 at the given I2C address.
    // Calls Wire.begin() and configures the driver for you.
    // `mode` selects the color model and brightness range (see ColorMode).
    void begin(
        const uint8_t backLeft[kChannelsPerLamp],
        const uint8_t frontLeft[kChannelsPerLamp],
        const uint8_t frontRight[kChannelsPerLamp],
        const uint8_t backRight[kChannelsPerLamp],
        ColorMode mode = ColorMode::Calibrated,
        uint8_t i2cAddress = kDefaultAddress,
        float pwmFrequencyHz = kDefaultFrequencyHz);

    // Initialize using a PCA9685 you own (e.g. to share the bus or drive
    // several boards). You are responsible for calling driver.begin() and
    // driver.setPWMFreq() yourself before using the lamps.
    void begin(
        const uint8_t backLeft[kChannelsPerLamp],
        const uint8_t frontLeft[kChannelsPerLamp],
        const uint8_t frontRight[kChannelsPerLamp],
        const uint8_t backRight[kChannelsPerLamp],
        Adafruit_PWMServoDriver &externalDriver,
        ColorMode mode = ColorMode::Calibrated);

    // Set 1-4 lamps to a solid color/brightness. Color `Off` clears that lamp.
    void set(
        LampPosition pos1, LampColor color1, uint8_t brightness1,
        LampPosition pos2 = LampPosition::None, LampColor color2 = LampColor::Off, uint8_t brightness2 = 0,
        LampPosition pos3 = LampPosition::None, LampColor color3 = LampColor::Off, uint8_t brightness3 = 0,
        LampPosition pos4 = LampPosition::None, LampColor color4 = LampColor::Off, uint8_t brightness4 = 0);

    // Blink 1-4 lamps once: on for onMs, then off for offMs (blocking).
    void flash(LampPosition pos1, LampColor color1, uint8_t brightness1,
               uint16_t onMs, uint16_t offMs);
    void flash(
        LampPosition pos1, LampColor color1, uint8_t brightness1,
        LampPosition pos2, LampColor color2, uint8_t brightness2,
        uint16_t onMs, uint16_t offMs);
    void flash(
        LampPosition pos1, LampColor color1, uint8_t brightness1,
        LampPosition pos2, LampColor color2, uint8_t brightness2,
        LampPosition pos3, LampColor color3, uint8_t brightness3,
        uint16_t onMs, uint16_t offMs);
    void flash(
        LampPosition pos1, LampColor color1, uint8_t brightness1,
        LampPosition pos2, LampColor color2, uint8_t brightness2,
        LampPosition pos3, LampColor color3, uint8_t brightness3,
        LampPosition pos4, LampColor color4, uint8_t brightness4,
        uint16_t onMs, uint16_t offMs);

    // Smoothly fade 1-4 lamps from a start color/brightness to an end
    // color/brightness over durationMs (blocking, sine-eased). All lamps fade
    // together.
    void crossFade(
        LampPosition pos1, LampColor startColor1, uint8_t startBrightness1,
        LampColor endColor1, uint8_t endBrightness1,
        uint16_t durationMs);
    void crossFade(
        LampPosition pos1, LampColor startColor1, uint8_t startBrightness1, LampColor endColor1, uint8_t endBrightness1,
        LampPosition pos2, LampColor startColor2, uint8_t startBrightness2, LampColor endColor2, uint8_t endBrightness2,
        uint16_t durationMs);
    void crossFade(
        LampPosition pos1, LampColor startColor1, uint8_t startBrightness1, LampColor endColor1, uint8_t endBrightness1,
        LampPosition pos2, LampColor startColor2, uint8_t startBrightness2, LampColor endColor2, uint8_t endBrightness2,
        LampPosition pos3, LampColor startColor3, uint8_t startBrightness3, LampColor endColor3, uint8_t endBrightness3,
        uint16_t durationMs);
    void crossFade(
        LampPosition pos1, LampColor startColor1, uint8_t startBrightness1, LampColor endColor1, uint8_t endBrightness1,
        LampPosition pos2, LampColor startColor2, uint8_t startBrightness2, LampColor endColor2, uint8_t endBrightness2,
        LampPosition pos3, LampColor startColor3, uint8_t startBrightness3, LampColor endColor3, uint8_t endBrightness3,
        LampPosition pos4, LampColor startColor4, uint8_t startBrightness4, LampColor endColor4, uint8_t endBrightness4,
        uint16_t durationMs);

    // Swap two colors back and forth across an axis, `cycles` times.
    // Each half-swap is held for halfPeriodMs (blocking).
    void sideToSide(Axis axis, LampColor color1, LampColor color2,
                    uint16_t halfPeriodMs, uint16_t cycles);

    // Rotate a 4-color wheel around the lamps, `rotations` times.
    // Each step crossfades over stepMs (blocking). Convenience overloads pad
    // the unused slots with Off.
    void spin(LampColor color1, LampColor color2, LampColor color3, LampColor color4,
              Direction direction, uint16_t stepMs, uint16_t rotations = 1);
    void spin(LampColor color1, Direction direction, uint16_t stepMs, uint16_t rotations = 1)
    {
        spin(color1, LampColor::Off, LampColor::Off, LampColor::Off, direction, stepMs, rotations);
    }
    void spin(LampColor color1, LampColor color2, Direction direction, uint16_t stepMs, uint16_t rotations = 1)
    {
        spin(color1, LampColor::Off, color2, LampColor::Off, direction, stepMs, rotations);
    }
    void spin(LampColor color1, LampColor color2, LampColor color3, Direction direction, uint16_t stepMs, uint16_t rotations = 1)
    {
        spin(color1, color2, color3, LampColor::Off, direction, stepMs, rotations);
    }

    // Turn a single lamp / all lamps off.
    void resetPosition(LampPosition position);
    void resetAllPositions();

private:
    struct Rgbw
    {
        uint8_t r, g, b, w;
    };

    // Relative R/G/B/W mix per LampColor (0-255), indexed by the enum. In flash.
    static const Rgbw kColorTableCalibrated[14];
    static const Rgbw kColorTableLinear[14];

    uint8_t _backLeft[kChannelsPerLamp] = {0};
    uint8_t _frontLeft[kChannelsPerLamp] = {0};
    uint8_t _frontRight[kChannelsPerLamp] = {0};
    uint8_t _backRight[kChannelsPerLamp] = {0};

    Adafruit_PWMServoDriver _ownedDriver;     // used when begin() owns the driver
    Adafruit_PWMServoDriver *_driver = nullptr; // points at owned or external

    const Rgbw *_colorTable = kColorTableCalibrated; // active color table
    uint8_t _brightnessMax = kCalibratedBrightnessMax; // full-brightness value for the active mode

    void storeChannels(
        const uint8_t backLeft[kChannelsPerLamp],
        const uint8_t frontLeft[kChannelsPerLamp],
        const uint8_t frontRight[kChannelsPerLamp],
        const uint8_t backRight[kChannelsPerLamp]);
    void applyMode(ColorMode mode);

    const uint8_t *channelsFor(LampPosition position) const;
    Rgbw colorFor(LampColor color) const;
    uint16_t channelPwm(uint8_t mix, uint8_t brightness) const;

    void applyOne(LampPosition position, LampColor color, uint8_t brightness);
    void writeLamp(LampPosition position, LampColor color, uint8_t brightness);

    void crossFadeCore(
        const LampPosition positions[kLampCount],
        const LampColor startColors[kLampCount], const uint8_t startBrightness[kLampCount],
        const LampColor endColors[kLampCount], const uint8_t endBrightness[kLampCount],
        uint8_t count, uint16_t durationMs);
};

#endif // MultiRGBWLeds_h

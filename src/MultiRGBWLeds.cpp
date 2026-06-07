#include "MultiRGBWLeds.h"

// Relative R/G/B/W mix for each LampColor (0-255 per channel). Order MUST match
// the LampColor enum. Stored in flash (PROGMEM) to save RAM. Brightness is
// applied on top of these mixes at call time.

// Calibrated: empirically tuned so colors read true on real RGBW lamps (the
// channels' apparent intensities are not equal), not derived from nominal RGB.
// At full brightness (10) they reproduce the original hand-tuned output exactly.
const MultiRGBWLeds::Rgbw MultiRGBWLeds::kColorTableCalibrated[14] PROGMEM = {
    {0, 0, 0, 0},     // Off
    {255, 0, 0, 0},   // Red
    {204, 25, 0, 0},  // Orange
    {153, 76, 0, 0},  // Yellow
    {102, 127, 0, 0}, // Lime
    {0, 255, 0, 0},   // Green
    {0, 204, 25, 0},  // Cyan
    {0, 178, 51, 0},  // Aqua
    {0, 102, 127, 0}, // Sky
    {0, 0, 255, 0},   // Blue
    {51, 0, 178, 0},  // Indigo
    {102, 0, 127, 0}, // Magenta
    {204, 0, 25, 0},  // Pink
    {0, 0, 0, 255},   // White
};

// Linear: nominal RGB color values, scaled linearly. Predictable on any rig,
// but may not look "true" on RGBW lamps without per-rig tuning.
const MultiRGBWLeds::Rgbw MultiRGBWLeds::kColorTableLinear[14] PROGMEM = {
    {0, 0, 0, 0},     // Off
    {255, 0, 0, 0},   // Red
    {255, 128, 0, 0}, // Orange
    {255, 255, 0, 0}, // Yellow
    {191, 255, 0, 0}, // Lime
    {0, 255, 0, 0},   // Green
    {0, 255, 255, 0}, // Cyan
    {0, 255, 191, 0}, // Aqua
    {0, 191, 255, 0}, // Sky
    {0, 0, 255, 0},   // Blue
    {75, 0, 130, 0},  // Indigo
    {255, 0, 255, 0}, // Magenta
    {255, 96, 160, 0},// Pink
    {0, 0, 0, 255},   // White
};

void MultiRGBWLeds::storeChannels(
    const uint8_t backLeft[kChannelsPerLamp],
    const uint8_t frontLeft[kChannelsPerLamp],
    const uint8_t frontRight[kChannelsPerLamp],
    const uint8_t backRight[kChannelsPerLamp])
{
    for (uint8_t i = 0; i < kChannelsPerLamp; i++)
    {
        _backLeft[i] = backLeft[i];
        _frontLeft[i] = frontLeft[i];
        _frontRight[i] = frontRight[i];
        _backRight[i] = backRight[i];
    }
}

void MultiRGBWLeds::applyMode(ColorMode mode)
{
    if (mode == ColorMode::Linear)
    {
        _colorTable = kColorTableLinear;
        _brightnessMax = kLinearBrightnessMax;
    }
    else
    {
        _colorTable = kColorTableCalibrated;
        _brightnessMax = kCalibratedBrightnessMax;
    }
}

void MultiRGBWLeds::begin(
    const uint8_t backLeft[kChannelsPerLamp],
    const uint8_t frontLeft[kChannelsPerLamp],
    const uint8_t frontRight[kChannelsPerLamp],
    const uint8_t backRight[kChannelsPerLamp],
    ColorMode mode,
    uint8_t i2cAddress,
    float pwmFrequencyHz)
{
    storeChannels(backLeft, frontLeft, frontRight, backRight);
    applyMode(mode);

    _ownedDriver = Adafruit_PWMServoDriver(i2cAddress, Wire);
    _driver = &_ownedDriver;

    Wire.begin();
    _driver->begin();
    _driver->setPWMFreq(pwmFrequencyHz);
}

void MultiRGBWLeds::begin(
    const uint8_t backLeft[kChannelsPerLamp],
    const uint8_t frontLeft[kChannelsPerLamp],
    const uint8_t frontRight[kChannelsPerLamp],
    const uint8_t backRight[kChannelsPerLamp],
    Adafruit_PWMServoDriver &externalDriver,
    ColorMode mode)
{
    storeChannels(backLeft, frontLeft, frontRight, backRight);
    applyMode(mode);
    _driver = &externalDriver;
}

const uint8_t *MultiRGBWLeds::channelsFor(LampPosition position) const
{
    switch (position)
    {
    case LampPosition::BackLeft:
        return _backLeft;
    case LampPosition::FrontLeft:
        return _frontLeft;
    case LampPosition::FrontRight:
        return _frontRight;
    case LampPosition::BackRight:
        return _backRight;
    default:
        return nullptr;
    }
}

MultiRGBWLeds::Rgbw MultiRGBWLeds::colorFor(LampColor color) const
{
    Rgbw mix;
    memcpy_P(&mix, &_colorTable[static_cast<uint8_t>(color)], sizeof(Rgbw));
    return mix;
}

uint16_t MultiRGBWLeds::channelPwm(uint8_t mix, uint8_t brightness) const
{
    // mix (0-255) * brightness (0.._brightnessMax) scaled into the 12-bit PWM range.
    uint32_t pwm = (uint32_t)mix * brightness * kPwmMax / (255UL * _brightnessMax);
    if (pwm > kPwmMax)
        pwm = kPwmMax;
    return (uint16_t)pwm;
}

void MultiRGBWLeds::writeLamp(LampPosition position, LampColor color, uint8_t brightness)
{
    const uint8_t *ch = channelsFor(position);
    if (ch == nullptr || _driver == nullptr)
        return;

    Rgbw mix = colorFor(color);
    _driver->setPWM(ch[0], 0, channelPwm(mix.r, brightness));
    _driver->setPWM(ch[1], 0, channelPwm(mix.g, brightness));
    _driver->setPWM(ch[2], 0, channelPwm(mix.b, brightness));
    _driver->setPWM(ch[3], 0, channelPwm(mix.w, brightness));
}

void MultiRGBWLeds::applyOne(LampPosition position, LampColor color, uint8_t brightness)
{
    if (position == LampPosition::None)
        return;
    if (color == LampColor::Off)
        resetPosition(position);
    else
        writeLamp(position, color, brightness);
}

void MultiRGBWLeds::resetPosition(LampPosition position)
{
    const uint8_t *ch = channelsFor(position);
    if (ch == nullptr || _driver == nullptr)
        return;
    for (uint8_t i = 0; i < kChannelsPerLamp; i++)
        _driver->setPWM(ch[i], 0, 0);
}

void MultiRGBWLeds::resetAllPositions()
{
    resetPosition(LampPosition::BackLeft);
    resetPosition(LampPosition::FrontLeft);
    resetPosition(LampPosition::FrontRight);
    resetPosition(LampPosition::BackRight);
}

void MultiRGBWLeds::set(
    LampPosition pos1, LampColor color1, uint8_t brightness1,
    LampPosition pos2, LampColor color2, uint8_t brightness2,
    LampPosition pos3, LampColor color3, uint8_t brightness3,
    LampPosition pos4, LampColor color4, uint8_t brightness4)
{
    applyOne(pos1, color1, brightness1);
    applyOne(pos2, color2, brightness2);
    applyOne(pos3, color3, brightness3);
    applyOne(pos4, color4, brightness4);
}

// ---------------------------------------------------------------------------
// flash
// ---------------------------------------------------------------------------

void MultiRGBWLeds::flash(LampPosition pos1, LampColor color1, uint8_t brightness1,
                          uint16_t onMs, uint16_t offMs)
{
    set(pos1, color1, brightness1);
    delay(onMs);
    resetPosition(pos1);
    delay(offMs);
}

void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, uint8_t brightness1,
    LampPosition pos2, LampColor color2, uint8_t brightness2,
    uint16_t onMs, uint16_t offMs)
{
    set(pos1, color1, brightness1, pos2, color2, brightness2);
    delay(onMs);
    resetPosition(pos1);
    resetPosition(pos2);
    delay(offMs);
}

void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, uint8_t brightness1,
    LampPosition pos2, LampColor color2, uint8_t brightness2,
    LampPosition pos3, LampColor color3, uint8_t brightness3,
    uint16_t onMs, uint16_t offMs)
{
    set(pos1, color1, brightness1, pos2, color2, brightness2, pos3, color3, brightness3);
    delay(onMs);
    resetPosition(pos1);
    resetPosition(pos2);
    resetPosition(pos3);
    delay(offMs);
}

void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, uint8_t brightness1,
    LampPosition pos2, LampColor color2, uint8_t brightness2,
    LampPosition pos3, LampColor color3, uint8_t brightness3,
    LampPosition pos4, LampColor color4, uint8_t brightness4,
    uint16_t onMs, uint16_t offMs)
{
    set(pos1, color1, brightness1, pos2, color2, brightness2,
        pos3, color3, brightness3, pos4, color4, brightness4);
    delay(onMs);
    resetPosition(pos1);
    resetPosition(pos2);
    resetPosition(pos3);
    resetPosition(pos4);
    delay(offMs);
}

// ---------------------------------------------------------------------------
// crossFade
// ---------------------------------------------------------------------------

void MultiRGBWLeds::crossFadeCore(
    const LampPosition positions[kLampCount],
    const LampColor startColors[kLampCount], const uint8_t startBrightness[kLampCount],
    const LampColor endColors[kLampCount], const uint8_t endBrightness[kLampCount],
    uint8_t count, uint16_t durationMs)
{
    if (_driver == nullptr || durationMs == 0)
        return;

    // Precompute the start and end PWM values for every channel of every lamp.
    const uint8_t *channels[kLampCount];
    uint16_t startPwm[kLampCount][kChannelsPerLamp];
    uint16_t endPwm[kLampCount][kChannelsPerLamp];

    for (uint8_t i = 0; i < count; i++)
    {
        channels[i] = channelsFor(positions[i]);
        Rgbw s = colorFor(startColors[i]);
        Rgbw e = colorFor(endColors[i]);
        const uint8_t sMix[kChannelsPerLamp] = {s.r, s.g, s.b, s.w};
        const uint8_t eMix[kChannelsPerLamp] = {e.r, e.g, e.b, e.w};
        for (uint8_t c = 0; c < kChannelsPerLamp; c++)
        {
            startPwm[i][c] = channelPwm(sMix[c], startBrightness[i]);
            endPwm[i][c] = channelPwm(eMix[c], endBrightness[i]);
        }
    }

    unsigned long startTime = millis();
    while (millis() - startTime < durationMs)
    {
        float progress = (float)(millis() - startTime) / durationMs;
        // Sine easing: slow in, slow out.
        progress = (sin((progress - 0.5f) * PI) + 1.0f) / 2.0f;

        for (uint8_t i = 0; i < count; i++)
        {
            if (channels[i] == nullptr)
                continue;
            for (uint8_t c = 0; c < kChannelsPerLamp; c++)
            {
                float value = startPwm[i][c] + ((float)endPwm[i][c] - startPwm[i][c]) * progress;
                long pwm = constrain((long)round(value), 0L, (long)kPwmMax);
                _driver->setPWM(channels[i][c], 0, (uint16_t)pwm);
            }
        }
    }

    // Land exactly on the end values.
    for (uint8_t i = 0; i < count; i++)
    {
        if (channels[i] == nullptr)
            continue;
        for (uint8_t c = 0; c < kChannelsPerLamp; c++)
            _driver->setPWM(channels[i][c], 0, endPwm[i][c]);
    }
}

void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, uint8_t startBrightness1,
    LampColor endColor1, uint8_t endBrightness1,
    uint16_t durationMs)
{
    LampPosition positions[kLampCount] = {pos1, LampPosition::None, LampPosition::None, LampPosition::None};
    LampColor startColors[kLampCount] = {startColor1, LampColor::Off, LampColor::Off, LampColor::Off};
    uint8_t startBr[kLampCount] = {startBrightness1, 0, 0, 0};
    LampColor endColors[kLampCount] = {endColor1, LampColor::Off, LampColor::Off, LampColor::Off};
    uint8_t endBr[kLampCount] = {endBrightness1, 0, 0, 0};
    crossFadeCore(positions, startColors, startBr, endColors, endBr, 1, durationMs);
}

void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, uint8_t startBrightness1, LampColor endColor1, uint8_t endBrightness1,
    LampPosition pos2, LampColor startColor2, uint8_t startBrightness2, LampColor endColor2, uint8_t endBrightness2,
    uint16_t durationMs)
{
    LampPosition positions[kLampCount] = {pos1, pos2, LampPosition::None, LampPosition::None};
    LampColor startColors[kLampCount] = {startColor1, startColor2, LampColor::Off, LampColor::Off};
    uint8_t startBr[kLampCount] = {startBrightness1, startBrightness2, 0, 0};
    LampColor endColors[kLampCount] = {endColor1, endColor2, LampColor::Off, LampColor::Off};
    uint8_t endBr[kLampCount] = {endBrightness1, endBrightness2, 0, 0};
    crossFadeCore(positions, startColors, startBr, endColors, endBr, 2, durationMs);
}

void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, uint8_t startBrightness1, LampColor endColor1, uint8_t endBrightness1,
    LampPosition pos2, LampColor startColor2, uint8_t startBrightness2, LampColor endColor2, uint8_t endBrightness2,
    LampPosition pos3, LampColor startColor3, uint8_t startBrightness3, LampColor endColor3, uint8_t endBrightness3,
    uint16_t durationMs)
{
    LampPosition positions[kLampCount] = {pos1, pos2, pos3, LampPosition::None};
    LampColor startColors[kLampCount] = {startColor1, startColor2, startColor3, LampColor::Off};
    uint8_t startBr[kLampCount] = {startBrightness1, startBrightness2, startBrightness3, 0};
    LampColor endColors[kLampCount] = {endColor1, endColor2, endColor3, LampColor::Off};
    uint8_t endBr[kLampCount] = {endBrightness1, endBrightness2, endBrightness3, 0};
    crossFadeCore(positions, startColors, startBr, endColors, endBr, 3, durationMs);
}

void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, uint8_t startBrightness1, LampColor endColor1, uint8_t endBrightness1,
    LampPosition pos2, LampColor startColor2, uint8_t startBrightness2, LampColor endColor2, uint8_t endBrightness2,
    LampPosition pos3, LampColor startColor3, uint8_t startBrightness3, LampColor endColor3, uint8_t endBrightness3,
    LampPosition pos4, LampColor startColor4, uint8_t startBrightness4, LampColor endColor4, uint8_t endBrightness4,
    uint16_t durationMs)
{
    LampPosition positions[kLampCount] = {pos1, pos2, pos3, pos4};
    LampColor startColors[kLampCount] = {startColor1, startColor2, startColor3, startColor4};
    uint8_t startBr[kLampCount] = {startBrightness1, startBrightness2, startBrightness3, startBrightness4};
    LampColor endColors[kLampCount] = {endColor1, endColor2, endColor3, endColor4};
    uint8_t endBr[kLampCount] = {endBrightness1, endBrightness2, endBrightness3, endBrightness4};
    crossFadeCore(positions, startColors, startBr, endColors, endBr, 4, durationMs);
}

// ---------------------------------------------------------------------------
// sideToSide / spin
// ---------------------------------------------------------------------------

void MultiRGBWLeds::sideToSide(Axis axis, LampColor color1, LampColor color2,
                               uint16_t halfPeriodMs, uint16_t cycles)
{
    // The two lamp groups for each axis.
    LampPosition groupA[2];
    LampPosition groupB[2];
    if (axis == Axis::LeftRight)
    {
        groupA[0] = LampPosition::BackLeft;
        groupA[1] = LampPosition::FrontLeft;
        groupB[0] = LampPosition::FrontRight;
        groupB[1] = LampPosition::BackRight;
    }
    else // FrontBack
    {
        groupA[0] = LampPosition::FrontLeft;
        groupA[1] = LampPosition::FrontRight;
        groupB[0] = LampPosition::BackLeft;
        groupB[1] = LampPosition::BackRight;
    }

    for (uint16_t i = 0; i < cycles; i++)
    {
        set(groupA[0], color1, _brightnessMax, groupA[1], color1, _brightnessMax,
            groupB[0], color2, _brightnessMax, groupB[1], color2, _brightnessMax);
        delay(halfPeriodMs);

        set(groupA[0], color2, _brightnessMax, groupA[1], color2, _brightnessMax,
            groupB[0], color1, _brightnessMax, groupB[1], color1, _brightnessMax);
        delay(halfPeriodMs);
    }
}

void MultiRGBWLeds::spin(LampColor color1, LampColor color2, LampColor color3, LampColor color4,
                         Direction direction, uint16_t stepMs, uint16_t rotations)
{
    LampColor current[kLampCount] = {color1, color2, color3, color4};

    for (uint16_t rotation = 0; rotation < rotations; rotation++)
    {
        for (uint8_t step = 0; step < kLampCount; step++)
        {
            LampColor next[kLampCount];
            if (direction == Direction::Clockwise)
            {
                next[0] = current[3];
                next[1] = current[0];
                next[2] = current[1];
                next[3] = current[2];
            }
            else
            {
                next[0] = current[1];
                next[1] = current[2];
                next[2] = current[3];
                next[3] = current[0];
            }

            crossFade(
                LampPosition::BackLeft, current[0], _brightnessMax, next[0], _brightnessMax,
                LampPosition::FrontLeft, current[1], _brightnessMax, next[1], _brightnessMax,
                LampPosition::FrontRight, current[2], _brightnessMax, next[2], _brightnessMax,
                LampPosition::BackRight, current[3], _brightnessMax, next[3], _brightnessMax,
                stepMs);

            memcpy(current, next, sizeof(current));
        }
    }
}

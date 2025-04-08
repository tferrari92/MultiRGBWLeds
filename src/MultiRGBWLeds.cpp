#include "MultiRGBWLeds.h"
#include "Wire.h"
#include "Adafruit_PWMServoDriver.h"

// External PCA9685 variable from main sketch
extern Adafruit_PWMServoDriver PCA9685;

// Static member initialization
int MultiRGBWLeds::backLeftChannels[4];
int MultiRGBWLeds::frontLeftChannels[4];
int MultiRGBWLeds::frontRightChannels[4];
int MultiRGBWLeds::backRightChannels[4];

void MultiRGBWLeds::initializeChannels(
    const int backLeft[4],
    const int frontLeft[4],
    const int frontRight[4],
    const int backRight[4])
{
    for (int i = 0; i < 4; i++)
    {
        backLeftChannels[i] = backLeft[i];
        frontLeftChannels[i] = frontLeft[i];
        frontRightChannels[i] = frontRight[i];
        backRightChannels[i] = backRight[i];
    }
}

// Function to convert color enum to RGB values
void MultiRGBWLeds::getColorValues(LampColor color, int *redChannel, int *greenChannel, int *blueChannel, int *whiteChannel)
{
    switch (color)
    {
    case OFF:
        *redChannel = 0;
        *greenChannel = 0;
        *blueChannel = 0;
        *whiteChannel = 0;
        break;
    case RED:
        *redChannel = 10;
        *greenChannel = 0;
        *blueChannel = 0;
        *whiteChannel = 0;
        break;
    case ORANGE:
        *redChannel = 10;
        *greenChannel = 2;
        *blueChannel = 0;
        *whiteChannel = 0;
        break;
    case YELLOW:
        *redChannel = 10;
        *greenChannel = 5;
        *blueChannel = 0;
        *whiteChannel = 0;
        break;
    case LIME:
        *redChannel = 8;
        *greenChannel = 10;
        *blueChannel = 0;
        *whiteChannel = 0;
        break;
    case GREEN:
        *redChannel = 0;
        *greenChannel = 10;
        *blueChannel = 0;
        *whiteChannel = 0;
        break;
    case CYAN:
        *redChannel = 0;
        *greenChannel = 10;
        *blueChannel = 2;
        *whiteChannel = 0;
        break;
    case AQUA:
        *redChannel = 0;
        *greenChannel = 10;
        *blueChannel = 4;
        *whiteChannel = 0;
        break;
    case SKY:
        *redChannel = 0;
        *greenChannel = 7;
        *blueChannel = 10;
        *whiteChannel = 0;
        break;
    case BLUE:
        *redChannel = 0;
        *greenChannel = 0;
        *blueChannel = 10;
        *whiteChannel = 0;
        break;
    case INDIGO:
        *redChannel = 4;
        *greenChannel = 0;
        *blueChannel = 10;
        *whiteChannel = 0;
        break;
    case MAGENTA:
        *redChannel = 9;
        *greenChannel = 0;
        *blueChannel = 10;
        *whiteChannel = 0;
        break;
    case PINK:
        *redChannel = 10;
        *greenChannel = 0;
        *blueChannel = 2;
        *whiteChannel = 0;
        break;
    case WHITE:
        *redChannel = 0;
        *greenChannel = 0;
        *blueChannel = 0;
        *whiteChannel = 10;
        break;
    default:
        *redChannel = 0;
        *greenChannel = 0;
        *blueChannel = 0;
        *whiteChannel = 0;
        break;
    }
    // Calculate total intensity
    float totalIntensity = *redChannel + *greenChannel + *blueChannel + *whiteChannel;

    // Normalize to percentage (keeping the sum at 10 for backward compatibility)
    if (totalIntensity > 0)
    {
        *redChannel = (*redChannel / totalIntensity) * 10;
        *greenChannel = (*greenChannel / totalIntensity) * 10;
        *blueChannel = (*blueChannel / totalIntensity) * 10;
        *whiteChannel = (*whiteChannel / totalIntensity) * 10;
    }
}

// New function to calculate color-specific brightness
ColorBrightness MultiRGBWLeds::getColorBrightness(LampColor color, int globalBrightness)
{
    ColorBrightness brightness;
    int redVal = 0, greenVal = 0, blueVal = 0, whiteVal = 0;

    // First, get the color channel values
    getColorValues(color, &redVal, &greenVal, &blueVal, &whiteVal);

    // Calculate brightness for each channel proportionally
    float totalColorIntensity = redVal + greenVal + blueVal + whiteVal;

    if (totalColorIntensity > 0)
    {
        brightness.red = (redVal > 0) ? (redVal / totalColorIntensity * globalBrightness) : 0;
        brightness.green = (greenVal > 0) ? (greenVal / totalColorIntensity * globalBrightness) : 0;
        brightness.blue = (blueVal > 0) ? (blueVal / totalColorIntensity * globalBrightness) : 0;
        brightness.white = (whiteVal > 0) ? (whiteVal / totalColorIntensity * globalBrightness) : 0;
    }
    else
    {
        // For white or off, just use the global brightness
        brightness.red = 0;
        brightness.green = 0;
        brightness.blue = 0;
        brightness.white = globalBrightness;
    }

    return brightness;
}

// Reset function for each position
void MultiRGBWLeds::resetPosition(LampPosition position)
{
    int *channelsToReset;

    switch (position)
    {
    case BACK_LEFT:
        channelsToReset = backLeftChannels;
        break;
    case FRONT_LEFT:
        channelsToReset = frontLeftChannels;
        break;
    case FRONT_RIGHT:
        channelsToReset = frontRightChannels;
        break;
    case BACK_RIGHT:
        channelsToReset = backRightChannels;
        break;
    default:
        return;
    }

    // Turn off all channels for this specific position
    for (int i = 0; i < 4; i++)
    {
        PCA9685.setPWM(channelsToReset[i], 0, 0);
    }
}

// Get channels for a specific position and color
int *MultiRGBWLeds::getChannels(LampPosition position, LampColor color)
{
    static int channels[4];
    int *selectedChannels;
    int redVal = 0, greenVal = 0, blueVal = 0, whiteVal = 0;

    // Select correct channel array based on position
    switch (position)
    {
    case BACK_LEFT:
        selectedChannels = backLeftChannels;
        break;
    case FRONT_LEFT:
        selectedChannels = frontLeftChannels;
        break;
    case FRONT_RIGHT:
        selectedChannels = frontRightChannels;
        break;
    case BACK_RIGHT:
        selectedChannels = backRightChannels;
        break;
    default:
        selectedChannels = frontLeftChannels;
        break;
    }

    // Get color channel values
    getColorValues(color, &redVal, &greenVal, &blueVal, &whiteVal);

    // Map values to channels
    channels[0] = redVal > 0 ? selectedChannels[0] : 0;
    channels[1] = greenVal > 0 ? selectedChannels[1] : 0;
    channels[2] = blueVal > 0 ? selectedChannels[2] : 0;
    channels[3] = whiteVal > 0 ? selectedChannels[3] : 0;

    return channels;
}

// Set function with multiple position and color support
void MultiRGBWLeds::set(
    LampPosition pos1, LampColor color1, int brightness1,
    LampPosition pos2, LampColor color2, int brightness2,
    LampPosition pos3, LampColor color3, int brightness3,
    LampPosition pos4, LampColor color4, int brightness4)
{
    // Helper function to set a single position
    auto setPosition = [](LampPosition pos, LampColor color, int brightness)
    {
        if (pos == NONE)
            return;

        int *selectedChannels;
        // Select correct channel array based on position
        switch (pos)
        {
        case BACK_LEFT:
            selectedChannels = backLeftChannels;
            break;
        case FRONT_LEFT:
            selectedChannels = frontLeftChannels;
            break;
        case FRONT_RIGHT:
            selectedChannels = frontRightChannels;
            break;
        case BACK_RIGHT:
            selectedChannels = backRightChannels;
            break;
        default:
            return;
        }

        // Get color values for the new color
        int redVal = 0, greenVal = 0, blueVal = 0, whiteVal = 0;
        getColorValues(color, &redVal, &greenVal, &blueVal, &whiteVal);

        // Get brightness values
        ColorBrightness colorBrightness = getColorBrightness(color, brightness);

        // Set each channel, turning off unused ones
        PCA9685.setPWM(selectedChannels[0], 0, (redVal > 0) ? colorBrightness.red * 409 : 0);
        delayMicroseconds(50);
        PCA9685.setPWM(selectedChannels[1], 0, (greenVal > 0) ? colorBrightness.green * 409 : 0);
        delayMicroseconds(50);
        PCA9685.setPWM(selectedChannels[2], 0, (blueVal > 0) ? colorBrightness.blue * 409 : 0);
        delayMicroseconds(50);
        PCA9685.setPWM(selectedChannels[3], 0, (whiteVal > 0) ? colorBrightness.white * 409 : 0);
        delayMicroseconds(50);
    };

    // Set each position
    if (color1 == OFF)
    {
        resetPosition(pos1);
    }
    else
    {
        setPosition(pos1, color1, brightness1);
    }

    if (pos2 != NONE)
    {
        if (color2 == OFF)
        {
            resetPosition(pos2);
        }
        else
        {
            setPosition(pos2, color2, brightness2);
        }
    }

    if (pos3 != NONE)
    {
        if (color3 == OFF)
        {
            resetPosition(pos3);
        }
        else
        {
            setPosition(pos3, color3, brightness3);
        }
    }

    if (pos4 != NONE)
    {
        if (color4 == OFF)
        {
            resetPosition(pos4);
        }
        else
        {
            setPosition(pos4, color4, brightness4);
        }
    }
}

void MultiRGBWLeds::resetAllPositions()
{
    resetPosition(BACK_LEFT);
    resetPosition(FRONT_LEFT);
    resetPosition(FRONT_RIGHT);
    resetPosition(BACK_RIGHT);
}

// Single lamp implementation - calls the full version with just one position
void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, int startBrightness1,
    LampColor endColor1, int endBrightness1,
    int durationMs)
{
    crossFade(pos1, startColor1, startBrightness1, endColor1, endBrightness1,
              NONE, OFF, 0, OFF, 0,
              NONE, OFF, 0, OFF, 0,
              NONE, OFF, 0, OFF, 0,
              durationMs);
}

// Two lamp implementation - calls the full version with two positions
void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, int startBrightness1, LampColor endColor1, int endBrightness1,
    LampPosition pos2, LampColor startColor2, int startBrightness2, LampColor endColor2, int endBrightness2,
    int durationMs)
{
    crossFade(pos1, startColor1, startBrightness1, endColor1, endBrightness1,
              pos2, startColor2, startBrightness2, endColor2, endBrightness2,
              NONE, OFF, 0, OFF, 0,
              NONE, OFF, 0, OFF, 0,
              durationMs);
}

// Three lamp implementation - calls the full version with three positions
void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, int startBrightness1, LampColor endColor1, int endBrightness1,
    LampPosition pos2, LampColor startColor2, int startBrightness2, LampColor endColor2, int endBrightness2,
    LampPosition pos3, LampColor startColor3, int startBrightness3, LampColor endColor3, int endBrightness3,
    int durationMs)
{
    crossFade(pos1, startColor1, startBrightness1, endColor1, endBrightness1,
              pos2, startColor2, startBrightness2, endColor2, endBrightness2,
              pos3, startColor3, startBrightness3, endColor3, endBrightness3,
              NONE, OFF, 0, OFF, 0,
              durationMs);
}

void MultiRGBWLeds::crossFade(
    LampPosition pos1, LampColor startColor1, int startBrightness1, LampColor endColor1, int endBrightness1,
    LampPosition pos2, LampColor startColor2, int startBrightness2, LampColor endColor2, int endBrightness2,
    LampPosition pos3, LampColor startColor3, int startBrightness3, LampColor endColor3, int endBrightness3,
    LampPosition pos4, LampColor startColor4, int startBrightness4, LampColor endColor4, int endBrightness4,
    int durationMs)
{
    unsigned long startTime = millis();
    unsigned long duration = durationMs;

    // Structure to hold normalized color values (0-10 scale, proportional)
    struct BaseColorValues
    {
        float red, green, blue, white;
    };

    // Arrays to store the start and end colors for each position
    BaseColorValues startColors[4];
    BaseColorValues endColors[4];

    // Arrays to track positions and their active status
    LampPosition positions[4] = {pos1, pos2, pos3, pos4};
    bool activePositions[4] = {true, pos2 != NONE, pos3 != NONE, pos4 != NONE};

    // Store channel arrays for each active position
    int *channelArrays[4];

    // Initialize color values and channel arrays for each position
    for (int i = 0; i < 4; i++)
    {
        if (activePositions[i])
        {
            // Get the channel array for this position
            switch (positions[i])
            {
            case BACK_LEFT:
                channelArrays[i] = backLeftChannels;
                break;
            case FRONT_LEFT:
                channelArrays[i] = frontLeftChannels;
                break;
            case FRONT_RIGHT:
                channelArrays[i] = frontRightChannels;
                break;
            case BACK_RIGHT:
                channelArrays[i] = backRightChannels;
                break;
            default:
                channelArrays[i] = nullptr;
                break;
            }

            // Get start color values
            LampColor startColor = (i == 0) ? startColor1 : (i == 1) ? startColor2
                                                        : (i == 2)   ? startColor3
                                                                     : startColor4;

            int startRed, startGreen, startBlue, startWhite;
            getColorValues(startColor, &startRed, &startGreen, &startBlue, &startWhite);
            startColors[i].red = startRed;
            startColors[i].green = startGreen;
            startColors[i].blue = startBlue;
            startColors[i].white = startWhite;

            // Get end color values
            LampColor endColor = (i == 0) ? endColor1 : (i == 1) ? endColor2
                                                    : (i == 2)   ? endColor3
                                                                 : endColor4;

            int endRed, endGreen, endBlue, endWhite;
            getColorValues(endColor, &endRed, &endGreen, &endBlue, &endWhite);
            endColors[i].red = endRed;
            endColors[i].green = endGreen;
            endColors[i].blue = endBlue;
            endColors[i].white = endWhite;
        }
    }

    // Run the fade animation
    while (millis() - startTime < duration)
    {
        // Calculate progress (0.0 to 1.0)
        float progress = (float)(millis() - startTime) / duration;

        // Apply sine-based easing function
        progress = (sin((progress - 0.5) * PI) + 1) / 2;

        // Update all active positions
        for (int pos = 0; pos < 4; pos++)
        {
            if (activePositions[pos] && channelArrays[pos] != nullptr)
            {
                // Get start and end brightness for this position
                int startBrightness = (pos == 0) ? startBrightness1 : (pos == 1) ? startBrightness2
                                                                  : (pos == 2)   ? startBrightness3
                                                                                 : startBrightness4;

                int endBrightness = (pos == 0) ? endBrightness1 : (pos == 1) ? endBrightness2
                                                              : (pos == 2)   ? endBrightness3
                                                                             : endBrightness4;

                // Interpolate the brightness
                float currentBrightness = startBrightness + (endBrightness - startBrightness) * progress;

                // Interpolate between color values (already normalized in getColorValues)
                float redValue = startColors[pos].red + (endColors[pos].red - startColors[pos].red) * progress;
                float greenValue = startColors[pos].green + (endColors[pos].green - startColors[pos].green) * progress;
                float blueValue = startColors[pos].blue + (endColors[pos].blue - startColors[pos].blue) * progress;
                float whiteValue = startColors[pos].white + (endColors[pos].white - startColors[pos].white) * progress;

                // Convert to PWM values (0-4095)
                int redPWM = round(redValue * currentBrightness * 40.95); // 40.95 = 409.5/10
                int greenPWM = round(greenValue * currentBrightness * 40.95);
                int bluePWM = round(blueValue * currentBrightness * 40.95);
                int whitePWM = round(whiteValue * currentBrightness * 40.95);

                // Constrain values to valid PWM range
                redPWM = constrain(redPWM, 0, 4095);
                greenPWM = constrain(greenPWM, 0, 4095);
                bluePWM = constrain(bluePWM, 0, 4095);
                whitePWM = constrain(whitePWM, 0, 4095);

                // Set PWM values
                PCA9685.setPWM(channelArrays[pos][0], 0, redPWM);
                PCA9685.setPWM(channelArrays[pos][1], 0, greenPWM);
                PCA9685.setPWM(channelArrays[pos][2], 0, bluePWM);
                PCA9685.setPWM(channelArrays[pos][3], 0, whitePWM);
            }
        }

        // Small delay to prevent overwhelming the I2C bus
        delayMicroseconds(50);
    }

    // Ensure we reach the final values exactly
    for (int pos = 0; pos < 4; pos++)
    {
        if (activePositions[pos] && channelArrays[pos] != nullptr)
        {
            // Get final brightness
            int finalBrightness = (pos == 0) ? endBrightness1 : (pos == 1) ? endBrightness2
                                                            : (pos == 2)   ? endBrightness3
                                                                           : endBrightness4;

            // Calculate final PWM values
            int redPWM = round(endColors[pos].red * finalBrightness * 40.95);
            int greenPWM = round(endColors[pos].green * finalBrightness * 40.95);
            int bluePWM = round(endColors[pos].blue * finalBrightness * 40.95);
            int whitePWM = round(endColors[pos].white * finalBrightness * 40.95);

            // Constrain values to valid PWM range
            redPWM = constrain(redPWM, 0, 4095);
            greenPWM = constrain(greenPWM, 0, 4095);
            bluePWM = constrain(bluePWM, 0, 4095);
            whitePWM = constrain(whitePWM, 0, 4095);

            // Set final PWM values
            PCA9685.setPWM(channelArrays[pos][0], 0, redPWM);
            PCA9685.setPWM(channelArrays[pos][1], 0, greenPWM);
            PCA9685.setPWM(channelArrays[pos][2], 0, bluePWM);
            PCA9685.setPWM(channelArrays[pos][3], 0, whitePWM);
        }
    }
}

// Single position flash with equal on/off times
void MultiRGBWLeds::flash(LampPosition pos1, LampColor color1, int brightness1, int intervalMs)
{
    // Turn on the specified position with the specified color and brightness
    set(pos1, color1, brightness1);

    // Turn the position off
    resetPosition(pos1);

    delay(intervalMs);
}

// Two positions flash with equal on/off times
void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, int brightness1,
    LampPosition pos2, LampColor color2, int brightness2,
    int intervalMs)
{
    // Turn on all specified positions
    set(pos1, color1, brightness1,
        pos2, color2, brightness2);

    // Turn off all specified positions
    resetPosition(pos1);
    resetPosition(pos2);

    delay(intervalMs);
}

// Three positions flash with equal on/off times
void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, int brightness1,
    LampPosition pos2, LampColor color2, int brightness2,
    LampPosition pos3, LampColor color3, int brightness3,
    int intervalMs)
{
    // Turn on all specified positions
    set(pos1, color1, brightness1,
        pos2, color2, brightness2,
        pos3, color3, brightness3);

    // Turn off all specified positions
    resetPosition(pos1);
    resetPosition(pos2);
    resetPosition(pos3);

    delay(intervalMs);
}

// Four positions flash with equal on/off times
void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, int brightness1,
    LampPosition pos2, LampColor color2, int brightness2,
    LampPosition pos3, LampColor color3, int brightness3,
    LampPosition pos4, LampColor color4, int brightness4,
    int intervalMs)
{
    // Turn on all specified positions
    set(pos1, color1, brightness1,
        pos2, color2, brightness2,
        pos3, color3, brightness3,
        pos4, color4, brightness4);

    // Turn off all specified positions
    resetPosition(pos1);
    resetPosition(pos2);
    resetPosition(pos3);
    resetPosition(pos4);

    delay(intervalMs);
}

// Single position flash with specified on duration
void MultiRGBWLeds::flash(LampPosition pos1, LampColor color1, int brightness1, int durationMs, int intervalMs)
{
    // Turn on the specified position
    set(pos1, color1, brightness1);

    delay(durationMs);

    // Turn the position off
    resetPosition(pos1);

    delay(intervalMs);
}

// Two positions flash with specified on duration
void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, int brightness1,
    LampPosition pos2, LampColor color2, int brightness2,
    int durationMs, int intervalMs)
{
    // Turn on all specified positions
    set(pos1, color1, brightness1,
        pos2, color2, brightness2);

    delay(durationMs);

    // Turn off all specified positions
    resetPosition(pos1);
    resetPosition(pos2);

    delay(intervalMs);
}

// Three positions flash with specified on duration
void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, int brightness1,
    LampPosition pos2, LampColor color2, int brightness2,
    LampPosition pos3, LampColor color3, int brightness3,
    int durationMs, int intervalMs)
{
    // Turn on all specified positions
    set(pos1, color1, brightness1,
        pos2, color2, brightness2,
        pos3, color3, brightness3);

    delay(durationMs);

    // Turn off all specified positions
    resetPosition(pos1);
    resetPosition(pos2);
    resetPosition(pos3);

    delay(intervalMs);
}

// Four positions flash with specified on duration
void MultiRGBWLeds::flash(
    LampPosition pos1, LampColor color1, int brightness1,
    LampPosition pos2, LampColor color2, int brightness2,
    LampPosition pos3, LampColor color3, int brightness3,
    LampPosition pos4, LampColor color4, int brightness4,
    int durationMs, int intervalMs)
{
    // Turn on all specified positions
    set(pos1, color1, brightness1,
        pos2, color2, brightness2,
        pos3, color3, brightness3,
        pos4, color4, brightness4);

    delay(durationMs);

    // Turn off all specified positions
    resetPosition(pos1);
    resetPosition(pos2);
    resetPosition(pos3);
    resetPosition(pos4);

    delay(intervalMs);
}

void MultiRGBWLeds::sideToSide(Axis axis, LampColor color1, LampColor color2, unsigned int time, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
    {
        // First pattern: group1 = color1, group2 = color2
        switch (axis)
        {
        case LEFT_RIGHT:
            MultiRGBWLeds::set(
                BACK_LEFT, color1, 10,
                FRONT_LEFT, color1, 10,
                FRONT_RIGHT, color2, 10,
                BACK_RIGHT, color2, 10);
            break;
        case FRONT_BACK:
            MultiRGBWLeds::set(
                FRONT_LEFT, color1, 10,
                FRONT_RIGHT, color1, 10,
                BACK_LEFT, color2, 10,
                BACK_RIGHT, color2, 10);
            break;
        }
        delay(time);

        // Second pattern: swap colors
        switch (axis)
        {
        case LEFT_RIGHT:
            MultiRGBWLeds::set(
                BACK_LEFT, color2, 10,
                FRONT_LEFT, color2, 10,
                FRONT_RIGHT, color1, 10,
                BACK_RIGHT, color1, 10);
            break;
        case FRONT_BACK:
            MultiRGBWLeds::set(
                FRONT_LEFT, color2, 10,
                FRONT_RIGHT, color2, 10,
                BACK_LEFT, color1, 10,
                BACK_RIGHT, color1, 10);
            break;
        }
        delay(time);
    }
}

void MultiRGBWLeds::spin(LampColor color1, LampColor color2, LampColor color3, LampColor color4, Direction direction, int speed, int count)
{
    LampColor currentColors[4] = {color1, color2, color3, color4};

    for (int rotation = 0; rotation < count; rotation++)
    {
        for (int step = 0; step < 4; step++)
        {
            LampColor nextColors[4];
            if (direction == CLOCKWISE)
            {
                nextColors[0] = currentColors[3];
                nextColors[1] = currentColors[0];
                nextColors[2] = currentColors[1];
                nextColors[3] = currentColors[2];
            }
            else
            {
                nextColors[0] = currentColors[1];
                nextColors[1] = currentColors[2];
                nextColors[2] = currentColors[3];
                nextColors[3] = currentColors[0];
            }

            crossFade(
                BACK_LEFT, currentColors[0], 10, nextColors[0], 10,
                FRONT_LEFT, currentColors[1], 10, nextColors[1], 10,
                FRONT_RIGHT, currentColors[2], 10, nextColors[2], 10,
                BACK_RIGHT, currentColors[3], 10, nextColors[3], 10,
                speed);

            memcpy(currentColors, nextColors, sizeof(currentColors));
        }
    }
}
#ifndef MultiRGBWLeds_h
#define MultiRGBWLeds_h

// Enum for lamp positions (unchanged)
enum LampPosition
{
    BACK_LEFT,
    FRONT_LEFT,
    FRONT_RIGHT,
    BACK_RIGHT,
    NONE
};

// Enum for lamp colors (unchanged)
enum LampColor
{
    RED,
    ORANGE,
    YELLOW,
    LIME,
    GREEN,
    CYAN,
    AQUA,
    SKY,
    BLUE,
    INDIGO,
    MAGENTA,
    PINK,
    WHITE,
    OFF
};

// Enum for axis (added for sideToSide function)
enum Axis
{
    LEFT_RIGHT,
    FRONT_BACK
};

enum Direction
{
    CLOCKWISE,
    ANTICLOCKWISE
};

// New struct to handle color-specific brightness
struct ColorBrightness
{
    int red;
    int green;
    int blue;
    int white;
};

class MultiRGBWLeds
{
public:
    static void begin(
        const int backLeft[4],
        const int frontLeft[4],
        const int frontRight[4],
        const int backRight[4])
    {
        initializeChannels(backLeft, frontLeft, frontRight, backRight);
    }

    // Public static methods remain the same
    static void set(
        LampPosition pos1, LampColor color1, int brightness1,
        LampPosition pos2 = NONE, LampColor color2 = OFF, int brightness2 = 0,
        LampPosition pos3 = NONE, LampColor color3 = OFF, int brightness3 = 0,
        LampPosition pos4 = NONE, LampColor color4 = OFF, int brightness4 = 0);

    // Single position flash with equal on/off times
    static void flash(LampPosition pos1, LampColor color1, int brightness1, int intervalMs);

    // Two positions flash with equal on/off times
    static void flash(
        LampPosition pos1, LampColor color1, int brightness1,
        LampPosition pos2, LampColor color2, int brightness2,
        int intervalMs);

    // Three positions flash with equal on/off times
    static void flash(
        LampPosition pos1, LampColor color1, int brightness1,
        LampPosition pos2, LampColor color2, int brightness2,
        LampPosition pos3, LampColor color3, int brightness3,
        int intervalMs);

    // Four positions flash with equal on/off times
    static void flash(
        LampPosition pos1, LampColor color1, int brightness1,
        LampPosition pos2, LampColor color2, int brightness2,
        LampPosition pos3, LampColor color3, int brightness3,
        LampPosition pos4, LampColor color4, int brightness4,
        int intervalMs);

    // Single position flash with specified on duration
    static void flash(LampPosition pos1, LampColor color1, int brightness1, int durationMs, int intervalMs);

    // Two positions flash with specified on duration
    static void flash(
        LampPosition pos1, LampColor color1, int brightness1,
        LampPosition pos2, LampColor color2, int brightness2,
        int durationMs, int intervalMs);

    // Three positions flash with specified on duration
    static void flash(
        LampPosition pos1, LampColor color1, int brightness1,
        LampPosition pos2, LampColor color2, int brightness2,
        LampPosition pos3, LampColor color3, int brightness3,
        int durationMs, int intervalMs);

    // Four positions flash with specified on duration
    static void flash(
        LampPosition pos1, LampColor color1, int brightness1,
        LampPosition pos2, LampColor color2, int brightness2,
        LampPosition pos3, LampColor color3, int brightness3,
        LampPosition pos4, LampColor color4, int brightness4,
        int durationMs, int intervalMs);

    // Existing crossFade methods remain the same...
    static void crossFade(
        LampPosition pos1, LampColor startColor1, int startBrightness1,
        LampColor endColor1, int endBrightness1,
        int durationMs);

    static void crossFade(
        LampPosition pos1, LampColor startColor1, int startBrightness1, LampColor endColor1, int endBrightness1,
        LampPosition pos2, LampColor startColor2, int startBrightness2, LampColor endColor2, int endBrightness2,
        int durationMs);

    static void crossFade(
        LampPosition pos1, LampColor startColor1, int startBrightness1, LampColor endColor1, int endBrightness1,
        LampPosition pos2, LampColor startColor2, int startBrightness2, LampColor endColor2, int endBrightness2,
        LampPosition pos3, LampColor startColor3, int startBrightness3, LampColor endColor3, int endBrightness3,
        int durationMs);

    static void crossFade(
        LampPosition pos1, LampColor startColor1, int startBrightness1, LampColor endColor1, int endBrightness1,
        LampPosition pos2, LampColor startColor2, int startBrightness2, LampColor endColor2, int endBrightness2,
        LampPosition pos3, LampColor startColor3, int startBrightness3, LampColor endColor3, int endBrightness3,
        LampPosition pos4, LampColor startColor4, int startBrightness4, LampColor endColor4, int endBrightness4,
        int durationMs = 1000);

    // New sideToSide function
    static void sideToSide(Axis axis, LampColor color1, LampColor color2, unsigned int time, unsigned int count);

    static void spin(LampColor color1, LampColor color2, LampColor color3, LampColor color4, Direction direction, int speed, int count = 1);
    static void spin(LampColor color1, Direction direction, int speed, int count = 1)
    {
        spin(color1, LampColor::OFF, LampColor::OFF, LampColor::OFF, direction, speed, count);
    }
    static void spin(LampColor color1, LampColor color2, Direction direction,
                     int speed, int count = 1)
    {
        spin(color1, LampColor::OFF, color2, LampColor::OFF, direction, speed, count);
    }
    static void spin(LampColor color1, LampColor color2, LampColor color3,
                     Direction direction, int speed, int count = 1)
    {
        spin(color1, color2, color3, LampColor::OFF, direction, speed, count);
    }

    static void resetPosition(LampPosition position);
    static void resetAllPositions();

private:
    // Channel configurations
    static int backLeftChannels[4];
    static int frontLeftChannels[4];
    static int frontRightChannels[4];
    static int backRightChannels[4];

    static void initializeChannels(
        const int backLeft[4],
        const int frontLeft[4],
        const int frontRight[4],
        const int backRight[4]);

    static void getColorValues(LampColor color, int *redChannel, int *greenChannel, int *blueChannel, int *whiteChannel);
    static ColorBrightness getColorBrightness(LampColor color, int globalBrightness);
    static int *getChannels(LampPosition position, LampColor color);
};

#endif // MultiRGBWLeds_h
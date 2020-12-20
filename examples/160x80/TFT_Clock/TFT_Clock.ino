/*
    An example analogue clock using a TFT LCD screen to show the time
    use of some of the drawing commands with the library.

    For a more accurate clock, it would be better to use the RTClib library.
    But this is just a demo.

    Based on a sketch by Gilchrist 6/2/2014 1.0
*/

#include <TFT_eSPI.h>

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

#define LCD_H TFT_WIDTH
#define LCD_W TFT_HEIGHT

#define LCD_SCALE(v) (LCD_H*v/240)

#define TFT_GREY 0x5AEB

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = (LCD_H/2), osy = (LCD_H/2), omx = (LCD_H/2), omy = (LCD_H/2), ohx = (LCD_H/2), ohy = (LCD_H/2); // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;
uint32_t targetTime = 0;                    // for next 1 second timeout

static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

boolean initial = 1;

void setup(void) {
    tft.begin();
    tft.setRotation(1);

    //tft.fillScreen(TFT_BLACK);
    //tft.fillScreen(TFT_RED);
    //tft.fillScreen(TFT_GREEN);
    //tft.fillScreen(TFT_BLUE);
    //tft.fillScreen(TFT_BLACK);
    tft.fillScreen(TFT_GREY);

    tft.setTextColor(TFT_WHITE, TFT_GREY);  // Adding a background colour erases previous text automatically

    // Draw clock face
    tft.fillCircle((LCD_H/2), (LCD_H/2), LCD_SCALE(118), TFT_GREEN);
    tft.fillCircle((LCD_H/2), (LCD_H/2), LCD_SCALE(110), TFT_BLACK);

    // Draw 12 lines
    for (int i = 0; i < 360; i += 30) {
        sx = cos((i - 90) * 0.0174532925);
        sy = sin((i - 90) * 0.0174532925);
        x0 = sx * LCD_SCALE(114) + (LCD_H/2);
        yy0 = sy * LCD_SCALE(114) + (LCD_H/2);
        x1 = sx * LCD_SCALE(100) + (LCD_H/2);
        yy1 = sy * LCD_SCALE(100) + (LCD_H/2);

        tft.drawLine(x0, yy0, x1, yy1, TFT_GREEN);
    }

    // Draw 60 dots
    for (int i = 0; i < 360; i += 6) {
        sx = cos((i - 90) * 0.0174532925);
        sy = sin((i - 90) * 0.0174532925);
        x0 = sx * LCD_SCALE(102) + (LCD_H/2);
        yy0 = sy * LCD_SCALE(102) + (LCD_H/2);
        // Draw minute markers
        tft.drawPixel(x0, yy0, TFT_WHITE);

        // Draw main quadrant dots
        if (i == 0 || i == 180) {
            tft.fillCircle(x0, yy0, 2, TFT_WHITE);
        }
        if (i == 90 || i == 270) {
            tft.fillCircle(x0, yy0, 2, TFT_WHITE);
        }
    }

    tft.fillCircle((LCD_H/2), (LCD_H/2)+1, 3, TFT_WHITE);

    tft.setCursor((LCD_H/8), LCD_SCALE(260));
    tft.println("Time flies");

    targetTime = millis() + 1000;
}

void loop() {
    if (targetTime < millis()) {
        targetTime += 1000;
        ss++;              // Advance second
        if (ss == 60) {
            ss = 0;
            mm++;            // Advance minute
            if (mm > 59) {
                mm = 0;
                hh++;          // Advance hour
                if (hh > 23) {
                    hh = 0;
                }
            }
        }

        // Pre-compute hand degrees, x & y coords for a fast screen update
        sdeg = ss * 6;                // 0-59 -> 0-354
        mdeg = mm * 6 + sdeg * 0.01666667; // 0-59 -> 0-360 - includes seconds
        hdeg = hh * 30 + mdeg * 0.0833333; // 0-11 -> 0-360 - includes minutes and seconds
        hx = cos((hdeg - 90) * 0.0174532925);
        hy = sin((hdeg - 90) * 0.0174532925);
        mx = cos((mdeg - 90) * 0.0174532925);
        my = sin((mdeg - 90) * 0.0174532925);
        sx = cos((sdeg - 90) * 0.0174532925);
        sy = sin((sdeg - 90) * 0.0174532925);

        if (ss == 0 || initial) {
            initial = 0;
            // Erase hour and minute hand positions every minute
            tft.drawLine(ohx, ohy, (LCD_H/2), (LCD_H/2)+1, TFT_BLACK);
            ohx = hx * LCD_SCALE(62) + (LCD_H/2)+1;
            ohy = hy * LCD_SCALE(62) + (LCD_H/2)+1;
            tft.drawLine(omx, omy, (LCD_H/2), (LCD_H/2)+1, TFT_BLACK);
            omx = mx * LCD_SCALE(84) + (LCD_H/2);
            omy = my * LCD_SCALE(84) + (LCD_H/2)+1;
        }

        // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
        tft.drawLine(osx, osy, (LCD_H/2), (LCD_H/2)+1, TFT_BLACK);
        osx = sx * LCD_SCALE(90) + (LCD_H/2)+1;
        osy = sy * LCD_SCALE(90) + (LCD_H/2)+1;
        tft.drawLine(osx, osy, (LCD_H/2), (LCD_H/2)+1, TFT_RED);
        tft.drawLine(ohx, ohy, (LCD_H/2), (LCD_H/2)+1, TFT_WHITE);
        tft.drawLine(omx, omy, (LCD_H/2), (LCD_H/2)+1, TFT_WHITE);
        tft.drawLine(osx, osy, (LCD_H/2), (LCD_H/2)+1, TFT_RED);

        tft.fillCircle((LCD_H/2), (LCD_H/2)+1, 3, TFT_RED);
    }
}

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9') {
        v = *p - '0';
    }
    return 10 * v + *++p - '0';
}

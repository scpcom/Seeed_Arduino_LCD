/*
    Ellipse drawing example

    This sketch does not use any fonts.
*/

#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define LCD_H TFT_WIDTH
#define LCD_W TFT_HEIGHT

void setup(void) {
    randomSeed(analogRead(A0));

    tft.init();

    tft.setRotation(1);

}

void loop() {

    tft.fillScreen(TFT_BLACK);

    // Draw some random filled elipses
    for (int i = 0; i < 20; i++) {
        int rx = random((LCD_H/4));
        int ry = random((LCD_H/4));
        int x = rx + random(LCD_W - rx - rx);
        int y = ry + random(LCD_H - ry - ry);
        tft.fillEllipse(x, y, rx, ry, random(0xFFFF));
    }

    delay(2000);
    tft.fillScreen(TFT_BLACK);

    // Draw some random outline elipses
    for (int i = 0; i < 20; i++) {
        int rx = random((LCD_H/4));
        int ry = random((LCD_H/4));
        int x = rx + random(LCD_W - rx - rx);
        int y = ry + random(LCD_H - ry - ry);
        tft.drawEllipse(x, y, rx, ry, random(0xFFFF));
    }

    delay(2000);
}




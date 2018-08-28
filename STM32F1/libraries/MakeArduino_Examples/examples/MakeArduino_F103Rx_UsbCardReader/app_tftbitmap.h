#ifndef _APP_TFTBITMAP_H
#define _APP_TFTBITMAP_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

extern Adafruit_TFTLCD tft;
void tft_drawBitmap(const char*fname,int x,int y);
void init_tft();
#endif



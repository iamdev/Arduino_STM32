
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int val=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

  tft.reset();

  uint16_t identifier = tft.readID();

  identifer = 0x9341;

  tft.begin(identifier);

void loop() {
  // put your main code here, to run repeatedly:
    if (Serial.available() > 0) {
    val = Serial.read();
    Serial.print("I received: ");
    //Serial.write(val);
    Serial.print(val, HEX);
    Serial.println(val);
    tft.println(val);
    }
}

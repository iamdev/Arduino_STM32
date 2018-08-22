// This sketch has been Refurbished by BUHOSOFT
// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.
//#define DEBUG
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
//#include <glcdfont_ctt.c>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

#define SensorPin A15
#define OffsetPH 0.90
#define LED 13
#define samplingInterval 20
#define printInterval 5000
#define ArrayLenth  40

int pHArray[ArrayLenth];
int pHArrayIndex=0;

int str1, str2, str3;
static float pHValue,voltage;
int TValue;
double orpValue;

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define BRIGHT_RED 0xf810 
#define GRAY1 0x8410 
#define GRAY2 0x4208 
#define BLUE2 0x051F

#define NAVY 0x000F
#define DGREEN 0x03E0
#define DCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define GREY 0xF7DE
#define LGRAY 0xC618
#define DGRAY 0x7BEF 

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;



void setup(void) {
  Serial.begin(9600);
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));
  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());
#endif // DEBUG
  tft.reset();

  uint16_t identifier = tft.readID();

if(identifier == 0x9325) {
#ifdef DEBUG
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {

    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {

    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {

    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {

    Serial.println(F("Found HX8357D LCD driver"));
#endif // DEBUG
    } else {
    #ifdef DEBUG
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.print(F("I try use ILI9341 LCD driver "));
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    #endif // DEBUG
    identifier = 0x9341;
  }


  tft.begin(identifier);
#ifdef DEBUG
  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(3000);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(CYAN));
  delay(500);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(RED, BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(YELLOW, MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println(F("Done!"));
#endif // DEBUG

  pinMode(LED,OUTPUT);
  
  tft.setRotation(1);
  tft.fillScreen(DCYAN);
  tft.setTextColor(WHITE);
  tft.setCursor(22, 10);   tft.setTextSize(2);
  tft.print("Redox");
  tft.fillRect(10, 30, 80, 50, RED);  //(x, y, xr, yr, color)
  tft.setCursor(150, 10);
  tft.print("PH");
  tft.fillRect(120, 30, 80, 50, RED);
  tft.setCursor(234, 10);
  tft.print("Temp.C");
  tft.fillRect(230, 30, 80, 50, RED);
  tft.setTextColor(BLACK);
  redoxdig();
  tft.setCursor(str1, 47);
  tft.print((int)orpValue);
  PHdig();
  tft.setCursor(str2, 47);
  tft.print(pHValue);
  tempdig();
  tft.setCursor(str3, 47);
  tft.print(TValue);
}
  
void loop(void)
{
  //int str1, str2;
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  //static float pHValue,voltage;
  //double orpValue=679;
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+OffsetPH;
      samplingTime=millis();
  }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    {
	TValue=28;
        orpValue=640;
        //pHValue=6.59;
        redoxdig();
        PHdig();
        tempdig();
        infoColorRx();  
        infoColorPH();
        infoColorT();        
        tft.setTextColor(BLACK); tft.setTextSize(2);
        tft.setCursor(str1, 47);
        tft.print((int)orpValue);
        tft.setCursor(str2, 47);
        tft.println(pHValue,2);
        tft.setCursor(str3, 47);  //x=257
        tft.println(TValue);
        Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
	Serial.println(pHValue,2);
        digitalWrite(LED,digitalRead(LED)^1);
        printTime=millis();
    }
}

    void redoxdig()
    {
        if((int)orpValue>999){
          str1=26;
        }
        else if((int)orpValue>99){
          str1=32;
        }
        else if((int)orpValue>9){
          str1=38;
        }
        else{
          str1=44;
        }
     }
     
     void PHdig()
     {
         if(pHValue<10.00){
          str2=136;
        }
        else{
          str2=130;
        }
     }
     
     void tempdig()
     {
           if(TValue>99){
             str3=252;
           }
           else if(TValue>9){
             str3=258;
           }
           else{
             str3=264;
           }
      }
      
      void infoColorPH()
      {
        if(pHValue<=7.40 & pHValue>=6.80)
        {
            tft.fillRect(120, 30, 80, 50, GREEN);
        }
        else if ((pHValue>7.40 & pHValue<=7.60) ||(pHValue>=6.60 & pHValue<6.80))
        {
            tft.fillRect(120, 30, 80, 50, YELLOW);
        }
        else
        {
            if (pHValue>7.60 || pHValue<6.60){
            tft.fillRect(120, 30, 80, 50, RED);
            }
        }
      }
      
      void infoColorRx()
      {
        if((int)orpValue<=660 & (int)orpValue>=600)
        {
            tft.fillRect(10, 30, 80, 50, GREEN);
        }
        else if((int)orpValue>660 & (int)orpValue<=680 || (int)orpValue>=580 & (int)orpValue<600)
        {
            tft.fillRect(10, 30, 80, 50, YELLOW);
        }
        else
        {
            if((int)orpValue>680 || (int)orpValue<580){
            tft.fillRect(10, 30, 80, 50, RED);
            }
        }
      }
      
      void infoColorT()
      {
        if(TValue<=28 & TValue>=24)
        {
            tft.fillRect(230, 30, 80, 50, GREEN);
        }
        else if(TValue>28 & TValue<=30 || TValue>=22 & TValue<24)
        {
            tft.fillRect(230, 30, 80, 50, YELLOW);
        }
        else
        {
            if(TValue>30 || TValue<22){
            tft.fillRect(230, 30, 80, 50, RED);
            }
        }
      }

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
/*void loop(void) {
  for(uint8_t rotation=0; rotation<4; rotation++) {
  tft.setRotation(rotation);
  tft.setCursor(10, 5);
  testText();
  delay(5000);  
  }
  
  testFilledRoundRects();
  testRoundRects();
  testFilledTriangles();
  testTriangles();
  testCircles(10, WHITE);
  testFillScreen();
  testText();
  testLines(CYAN);
  testFastLines(RED, BLUE);
  testRects(GREEN);
  testFilledRects(YELLOW, MAGENTA);
  testFilledCircles(10, MAGENTA);
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(10, 10);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  tft.setTextSize(2);
  tft.setTextColor(YELLOW);
  tft.setCursor(8,190);
  tft.println("Ура!!! Мы пишим по русски!");
  tft.setTextSize(2);
  tft.println("\xB1 \x01 \x02 \x03");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n = min(tft.width(), tft.height());
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()); i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }

  return micros() - start;
}*/

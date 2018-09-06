#define RTC_DS3231 1
#define RTC_EMBEDDED 1
#define TEST_EEPROM 1
#define USB_COMPOSITE 1
#define TEST_SPITFT 1
#define TFT_USE_SPI 1

#if USB_COMPOSITE >0
#include <USBComposite.h>
#endif

#if TEST_EEPROM >0
#include <Wire.h>
#endif

#if RTC_EMBEDDED >0
#include <RTClock.h>
#endif

#if RTC_DS3231>0
#include "ds3231.h"
#endif
#if RTC_EMBEDDED>0
RTClock rtclock (RTCSEL_LSE); // initialise
#endif

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library.h
#include <SPI.h>
#define TFT_BL     PB0
#define TFT_CS     PB12
#define TFT_RST    PB1  // you can also connect this to the Arduino reset
#define TFT_DC     PB2
#define TFT_SCLK   PB13
#define TFT_MOSI   PB15

#if TFT_USE_SPI>0
SPIClass spi2(2);
Adafruit_ST7735 tft = Adafruit_ST7735(&spi2,TFT_CS, TFT_DC, TFT_RST);
#else
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
#endif

const char * weekdays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char * months[] = {"Dummy", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

#if USB_COMPOSITE > 0
#define Serial CompositeSerial
#endif

#if TEST_EEPROM >0
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}
// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
        Wire.write(data[c]);
    Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}
#endif

float p = 3.1415926;

#if USB_COMPOSITE >0
#define Serial CompositeSerial
#endif


void setup(void) {
pinMode(LED_BUILTIN,OUTPUT);
digitalWrite(LED_BUILTIN,LOW);

#if USB_COMPOSITE >0
    USBComposite.setProductId(0x04);
    CompositeSerial.registerComponent();
    USBComposite.begin();
    pinMode(PC10,OUTPUT);
    digitalWrite(PC10,HIGH);
#endif  

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  Serial.begin(115200);

#if  defined(SERIAL_USB) || USB_COMPOSITE >0
  while(!Serial.isConnected());
#endif
  delay(100);
  Serial.print("Hello! ST7735 TFT Test");
  
  Serial.println("Initialized");

  uint16_t time = millis();
  pinMode(TFT_BL,PWM);
  pwmWrite(TFT_BL,0);
#if TEST_SPITFT > 0
#if TFT_USE_SPI >0
  spi2.begin();
  spi2.setClockDivider(4);  
#endif  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  pwmWrite(TFT_BL,0xFFFF); 

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  // large block of text
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST7735_WHITE);
  delay(1000);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  // tft print function!
  tftPrintTest();
  delay(4000);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  // a single pixel
  tft.drawPixel(tft.width()/2, tft.height()/2, ST7735_GREEN);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  // line draw test
  testlines(ST7735_YELLOW);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  // optimized lines
  testfastlines(ST7735_RED, ST7735_BLUE);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  testdrawrects(ST7735_GREEN);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  testfillrects(ST7735_YELLOW, ST7735_MAGENTA);
  delay(500);

  tft.fillScreen(ST7735_BLACK);
  testfillcircles(10, ST7735_BLUE);
  testdrawcircles(10, ST7735_WHITE);

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  testroundrects();

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  testtriangles();

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  mediabuttons();

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  Serial.println("done");

  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
#endif

#if TEST_EEPROM>0    
    char testdata[] = "TEST"; // data to write
    char testread[sizeof(testdata)+1];
    Wire.begin(); // initialise the connection
    //Wire.setClock(10000);             
    i2c_eeprom_read_buffer(0x50,0, (byte *)testread,sizeof(testdata));
    if(strcmp(testdata, testread) != 0)
    {
      Serial.print("Write \"");
      Serial.print(testdata);
      Serial.println("\" to EEPROM address 0");    
      i2c_eeprom_write_page(0x50, 0, (byte *)testdata, sizeof(testdata)); // write to EEPROM          
      delay(1000);
    }
    Serial.print("Read from EEPROM address 0 :");        
    i2c_eeprom_read_buffer(0x50,0, (byte *)testread,sizeof(testdata));
    /*int addr=0;
    byte b = i2c_eeprom_read_byte(0x50, addr); // access the first address from the memory
    while (b!=0 && addr < sizeof(testdata))
    {
        Serial.print((char)b); //print content to serial port
        testdata[addr] = (char)b;
        addr++; //increase address
        b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    }*/    
    Serial.println(testdata); 
    bool test_fail = false;
    for(int i=0;i<sizeof(testdata);i++){
      if(testread[i] != testdata[i]){
        test_fail = true;
        break;
      }
    }
      
    if(test_fail){
      Serial.println("Test EEPROM : Failed!");
    }
    else{
      Serial.println("Test EEPROM : pass");
    }    
#endif    
    char s[30];
#if RTC_DS3231>0
    Serial.println("Test DS3231 RTC");
    DS3231_init(DS3231_INTCN);
    struct ts t;
    DS3231_get(&t);
    snprintf(s, 30, "%d.%02d.%02d %02d:%02d:%02d", t.year,t.mon, t.mday, t.hour, t.min, t.sec);
    Serial.print("DS3231 RTC timestamp:");
    Serial.println(s);
    Serial.print("read temperature from DS3231 : ");
    Serial.println(DS3231_get_treg(), DEC);
#endif
#if RTC_EMBEDDED>0
    Serial.println("Test internal RTC use LSE 32.768kHz");    
    tm_t mtt;    
    rtclock.breakTime(rtclock.now(), mtt);
    Serial.print("Internal RTC (LSE) timestamp:");
    snprintf(s, 30,"%s %u %u, %s, %02u:%02u:%02u", months[mtt.month], mtt.day, mtt.year+1970, weekdays[mtt.weekday], mtt.hour, mtt.minute, mtt.second);      
    Serial.println(s);
#endif
    digitalWrite(LED_BUILTIN,HIGH);  
}

uint32_t t_ms=0;
bool invDisp = false;
void loop() {
    uint32_t ms = millis();
    if(ms > t_ms + 1000){
      digitalWrite(LED_BUILTIN,LOW);
      t_ms = ms;
      #if RTC_EMBEDDED>0 && TEST_SPITFT > 0
      tm_t mtt;    
      rtclock.breakTime(rtclock.now(), mtt);   
      char s[40];
      snprintf(s, 30,"%02u/%02u/%04u, %02u:%02u:%02u", mtt.day, mtt.month, mtt.year+1970, mtt.hour, mtt.minute, mtt.second);
      tft.fillRect(0,0,128,7,0);
      tft.setCursor(0,0);
      tft.print(s);            
      #endif
      delay(100);
      digitalWrite(LED_BUILTIN,HIGH);
      tft.invertDisplay(invDisp);
      invDisp = !invDisp;
    }
}

#if TEST_SPITFT > 0
void testlines(uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, 0, x, tft.height()-1, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, 0, tft.width()-1, y, color);
  }

  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, 0, 0, y, color);
  }

  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(0, tft.height()-1, x, 0, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
  }

  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
  }
  for (int16_t y=0; y < tft.height(); y+=6) {
    tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
  }
}

void testdrawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t y=0; y < tft.height(); y+=5) {
    tft.drawFastHLine(0, y, tft.width(), color1);
  }
  for (int16_t x=0; x < tft.width(); x+=5) {
    tft.drawFastVLine(x, 0, tft.height(), color2);
  }
}

void testdrawrects(uint16_t color) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=0; x < tft.width(); x+=6) {
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  tft.fillScreen(ST7735_BLACK);
  for (int16_t x=tft.width()-1; x > 6; x-=6) {
    tft.fillRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color1);
    tft.drawRect(tft.width()/2 -x/2, tft.height()/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=radius; x < tft.width(); x+=radius*2) {
    for (int16_t y=radius; y < tft.height(); y+=radius*2) {
      tft.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < tft.width()+radius; x+=radius*2) {
    for (int16_t y=0; y < tft.height()+radius; y+=radius*2) {
      tft.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  tft.fillScreen(ST7735_BLACK);
  int color = 0xF800;
  int t;
  int w = tft.width()/2;
  int x = tft.height()-1;
  int y = 0;
  int z = tft.width();
  for(t = 0 ; t <= 15; t++) {
    tft.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  tft.fillScreen(ST7735_BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = tft.width()-2;
    int h = tft.height()-2;
    for(i = 0 ; i <= 16; i+=1) {
      tft.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(ST7735_WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST7735_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST7735_WHITE);
  tft.print(" seconds.");
}

void mediabuttons() {
  // play
  tft.fillScreen(ST7735_BLACK);
  tft.fillRoundRect(25, 10, 78, 60, 8, ST7735_WHITE);
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_RED);
  delay(500);
  // pause
  tft.fillRoundRect(25, 90, 78, 60, 8, ST7735_WHITE);
  tft.fillRoundRect(39, 98, 20, 45, 5, ST7735_GREEN);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST7735_GREEN);
  delay(500);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_BLUE);
  delay(50);
  // pause color
  tft.fillRoundRect(39, 98, 20, 45, 5, ST7735_RED);
  tft.fillRoundRect(69, 98, 20, 45, 5, ST7735_RED);
  // play color
  tft.fillTriangle(42, 20, 42, 60, 90, 40, ST7735_GREEN);
}
#endif

#include "app_config.h"
#include "app_sdfat.h"
#include "app_tftbitmap.h"
#define TFT_DEBUG 1

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void init_tft(){
   uint16_t tftId = tft.readID();
  if(tftId!=0x9488||tftId!=0x8481||tftId!=0x9486){
    tftId = 0x9341;
  }
  tft.begin(tftId);  
}
#define BUFFPIXEL 20

uint16_t read16(StdioStream &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.getc(); // LSB
  ((uint8_t *)&result)[1] = f.getc(); // MSB
  return result;
}

uint32_t read32(StdioStream &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.getc(); // LSB
  ((uint8_t *)&result)[1] = f.getc();
  ((uint8_t *)&result)[2] = f.getc();
  ((uint8_t *)&result)[3] = f.getc(); // MSB
  return result;
}

void tft_drawBitmap(const char*filename,int x,int y){
  StdioStream bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel in buffer (R+G+B per pixel)
  uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();
  uint8_t  lcdidx = 0;
  boolean  first = true;

  if((x >= tft.width()) || (y >= tft.height())) return;

#if SERIAL_DEBUG >0
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');
#endif
  if (!bmpFile.fopen(filename, "r+")) {
    Serial.println(F("fopen fail"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) {
      // BMP signature
    #if SERIAL_DEBUG >0
      Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    #else
      read32(bmpFile);
    #endif
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    #if SERIAL_DEBUG >0
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    #else
    read32(bmpFile);
    #endif

    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      #if SERIAL_DEBUG >0
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      #endif
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        #if SERIAL_DEBUG >0
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);
        delay(3000);
        #endif
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.ftell() != pos) { // Need seek?
            bmpFile.fseek(pos,SEEK_SET);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each column...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              // Push LCD buffer to the display first
              if(lcdidx > 0) {
                tft.pushColors(lcdbuffer, lcdidx, first);
                lcdidx = 0;
                first  = false;
              }
              bmpFile.fread(sdbuffer, sizeof(sdbuffer),1);
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            lcdbuffer[lcdidx++] = tft.color565(r,g,b);
          } // end pixel
        } // end scanline
        // Write any remaining data to LCD
        if(lcdidx > 0) {
          tft.pushColors(lcdbuffer, lcdidx, first);
        }

      } // end goodBmp
    }
  }

  bmpFile.fclose();
  if(!goodBmp) tft.println(F("BMP format not recognized."));
}




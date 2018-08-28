#include "app_config.h"
#include "app_sdfat.h"
#include "app_usb_mass.h"
#include "app_tftbitmap.h"

void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(USB_DESC_PIN,OUTPUT);
  digitalWrite(USB_DESC_PIN,LOW);  
  Serial.begin(115200);
  init_tft();
  init_sd();
  #if SERIAL_DEBUG >0 && DEBUG_SD_INFO >0 
  printSdCardInfo();
  Serial.println("----root-SD----");
  sd.ls();
  Serial.println("---------------");
  #endif
  tft.fillScreen(WHITE);
  tft_drawBitmap("logo_128.bmp",104,204);
  tft_drawBitmap("name.bmp",36,175);
}

void loop(){
  usb_mass_loop();
}


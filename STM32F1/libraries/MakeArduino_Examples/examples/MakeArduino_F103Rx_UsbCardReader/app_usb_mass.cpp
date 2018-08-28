#include "app_usb_mass.h"
#include "app_sdfat.h"
#define DEBUG_READ_WRITE 0

bool sd_write_buffer(uint32_t memoryOffset, const uint8_t *writebuff, uint16_t transferLength) 
{
  digitalWrite(LED_BUILTIN,LOW);
  #if SERIAL_DEBUG >0 && DEBUG_SD_INFO > 0 && DEBUG_READ_WRITE > 0 
  Serial.print("Write [0x");
  Serial.print(memoryOffset,HEX);
  Serial.print(":");
  Serial.print(transferLength,DEC);  
  Serial.print("]...");
  #endif
  bool ret = sd.card()->writeBlocks(memoryOffset/512, writebuff, transferLength/512);
  #if SERIAL_DEBUG >0 && DEBUG_SD_INFO > 0 && DEBUG_READ_WRITE > 0
  if(ret)
    Serial.println("Pass");
  else
    Serial.println("Fail");
  #endif
  digitalWrite(LED_BUILTIN,HIGH);
  return ret;
}

bool sd_read_buffer(uint32_t memoryOffset, uint8_t *readbuff, uint16_t transferLength) {
  digitalWrite(LED_BUILTIN,LOW);
  int c = 0;
  #if SERIAL_DEBUG >0 && DEBUG_SD_INFO > 0 && DEBUG_READ_WRITE > 0
  Serial.print("Read [0x");
  Serial.print(memoryOffset,HEX);
  Serial.print(":");
  Serial.print(transferLength,DEC);  
  Serial.print("]...");
  #endif  
  bool ret= sd.card()->readBlocks(memoryOffset/512, readbuff, transferLength/512);
  #if SERIAL_DEBUG >0 && DEBUG_SD_INFO > 0 && DEBUG_READ_WRITE > 0
  if(ret)
    Serial.println("Pass");
  else
    Serial.println("Fail");
  
  c = 0;
  CompositeSerial.print("Read [0x");
  CompositeSerial.print(memoryOffset,HEX);
  CompositeSerial.print(":");
  CompositeSerial.print(transferLength,DEC);  
  CompositeSerial.println("]");

  while(c<transferLength){
    for(int i=0;i<32;i++){
      uint8_t v = readbuff[c++];
      if(v<0x10)CompositeSerial.print("0");
      CompositeSerial.print(v,HEX);
      CompositeSerial.print(" ");
    }
    CompositeSerial.println();
  }
  #endif
  digitalWrite(LED_BUILTIN,HIGH);
  return ret;
}


void init_reader() {  
  Serial.println(F("Initialize USB Mass Storage"));
  cardSize = sd.card()->cardSize();
  USBComposite.setProductId(PRODUCT_ID);    
  MassStorage.setDrive(0, cardSize*512, sd_read_buffer, sd_write_buffer);
  MassStorage.registerComponent();
  CompositeSerial.registerComponent();
  USBComposite.begin();
  digitalWrite(USB_DESC_PIN,HIGH);
  usb_mass_enabled=true;
}

void usb_mass_loop(){
  if (!usb_mass_enabled) {
    init_reader();
  }
  else {
    MassStorage.loop();
  }
}


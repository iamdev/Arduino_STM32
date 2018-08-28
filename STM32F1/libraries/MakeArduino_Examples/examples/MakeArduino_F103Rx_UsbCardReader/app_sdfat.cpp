#include "app_sdfat.h"

#if USE_STANDARD_SPI_LIBRARY>0
SdFat sd;  
#else
  #if ENABLE_EXTENDED_TRANSFER_CLASS>0
SdFatEX sd(SD_SPI_NO);
  #else
SdFat sd(SD_SPI_NO);  
  #endif
#endif

void printSdCardInfo(){
    cid_t cid;
  if (!sd.card()->readCID(&cid)) {
    Serial.print("Read CID failed");
    return;
  }else{
    Serial.print(F("\nManufacturer ID: "));
    Serial.println(int(cid.mid),HEX);
    Serial.print(F("OEM ID: "));
    Serial.print(cid.oid[0]);
    Serial.println(cid.oid[1]);;
    Serial.print(F("Product: "));
    for (uint8_t i = 0; i < 5; i++) {
      Serial.print(cid.pnm[i]);
    }
    Serial.print(F("\nVersion: ")); 
    Serial.print(int(cid.prv_n));
    Serial.print(".");
    Serial.println(int(cid.prv_m));
    Serial.print(F("Serial number: "));
    Serial.println(cid.psn,HEX);    
  }
  
  cardSize = sd.card()->cardSize();
    if (cardSize == 0) {
    Serial.println("cardSize failed");
    return;
  }
  
  Serial.print(F("Card type: "));
  switch (sd.card()->type()) {
    case SD_CARD_TYPE_SD1: Serial.println(F("SD1"));
      break;
    case SD_CARD_TYPE_SD2:Serial.println(F("SD2"));
      break;
    case SD_CARD_TYPE_SDHC:
      if (cardSize < 70000000)
        Serial.println(F("SDHC"));
      else 
        Serial.println(F("SDXC"));      
      break; 
    default:
      Serial.println(F("Unknown"));
  } 
  Serial.print(F("Volume : FAT"));
  Serial.println(int(sd.vol()->fatType()));
  Serial.print(F("Card Size :"));
  Serial.print(cardSize/2048);
  Serial.println(F("MB"));
  Serial.print(F("Blocks/Cluster: "));
  Serial.println(int(sd.vol()->blocksPerCluster()));
  Serial.print(F("Number of Cluster: "));
  Serial.println(sd.vol()->clusterCount());
  Serial.print(F("Free Clusters: "));
  long _us = micros();
  uint32_t volFree = sd.vol()->freeClusterCount();
  Serial.print(volFree);
  Serial.print(" (");
  Serial.print((float)(micros()-_us)/1000.0);
  Serial.println("ms.)");
  float fs = volFree*sd.vol()->blocksPerCluster()/2048;
  Serial.print(F("freeSpace: "));
  Serial.print(fs);
  Serial.println(F(" MB"));
  Serial.print(F("fatStartBlock: "));
  Serial.println(sd.vol()->fatStartBlock());
  Serial.print(F("fatCount: "));
  Serial.println(int(sd.vol()->fatCount()));
  Serial.print(F("blocksPerFat: "));
  Serial.println(sd.vol()->blocksPerFat());
  Serial.print(F("rootDirStart: "));
  Serial.println(sd.vol()->rootDirStart());
  Serial.print(F("dataStartBlock: "));
  Serial.println(sd.vol()->dataStartBlock());
}

void init_sd(){
#if USE_STANDARD_SPI_LIBRARY == 0 && SD_SPI_NO == 1
  afio_remap(AFIO_REMAP_SPI1); // remap SPI1
  gpio_set_mode(GPIOB, 3, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOB, 4, GPIO_INPUT_FLOATING);
  gpio_set_mode(GPIOB, 5, GPIO_AF_OUTPUT_PP);
  gpio_set_mode(GPIOA, 15, GPIO_AF_OUTPUT_PP);    
#endif

#if ENABLE_EXTENDED_TRANSFER_CLASS  > 0
  if (!sd.begin(SD_CHIP_SELECT, SD_CARD_SPEED)) {
    Serial.print("cardBegin failed");
    return;
  }
#else
  if (!sd.cardBegin(SD_CHIP_SELECT, SD_CARD_SPEED)) {
    Serial.print("cardBegin failed");
    return;
  } 
  if (!sd.fsBegin()) {
    Serial.println("\nFile System initialization failed.\n");
    return;
  }
#endif
}



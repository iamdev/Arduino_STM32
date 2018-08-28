#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H
#include <USBComposite.h>

#define USB_DESC_PIN PC10


#define DEBUG_SD_INFO 1
#define SERIAL_DEBUG 1

//SPI No 1 or 3
#define SD_SPI_NO 1

#if SERIAL_DEBUG >0 
#define Serial_Debug(s) Serial.print(s)
#else
#define Serial_Debug(s)
#endif

#endif

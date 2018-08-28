#ifndef _APP_SDFAT_H
#define _APP_SDFAT_H
#include "app_config.h"
#include "SdFatConfigEx.h"
#include <SdFat.h>

#define SD_CHIP_SELECT 10
#define SD_CARD_SPEED SD_SCK_MHZ(36)

static uint64_t cardSize;

#if USE_STANDARD_SPI_LIBRARY==0 && ENABLE_EXTENDED_TRANSFER_CLASS > 0
extern SdFatEX sd;
#else
extern SdFat sd;
#endif

void init_sd();

#if DEBUG_SD_INFO > 0 
void printSdCardInfo();
#endif
#endif

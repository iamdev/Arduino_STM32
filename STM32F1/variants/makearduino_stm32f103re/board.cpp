/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file   wirish/boards/maple/board.cpp
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Maple board file.
 */

#include <board/board.h>         // For this board's header file

/* Roger Clark. Added next to includes for changes to Serial */
#include <libmaple/usart.h>
#include <HardwareSerial.h>

#include <wirish_types.h> // For stm32_pin_info and its contents
                                 // (these go into PIN_MAP).

#include "boards_private.h"      // For PMAP_ROW(), which makes
                                 // PIN_MAP easier to read.

// boardInit(): nothing special to do for Maple.
//
// When defining your own board.cpp, you can put extra code in this
// function if you have anything you want done on reset, before main()
// or setup() are called.
//
// If there's nothing special you need done, feel free to leave this
// function out, as we do here.
/*
void boardInit(void) {
}
*/

// Pin map: this lets the basic I/O functions (digitalWrite(),
// analogRead(), pwmWrite()) translate from pin numbers to STM32
// peripherals.
//
// PMAP_ROW() lets us specify a row (really a struct stm32_pin_info)
// in the pin map. Its arguments are:
//
// - GPIO device for the pin (&gpioa, etc.)
// - GPIO bit for the pin (0 through 15)
// - Timer device, or NULL if none
// - Timer channel (1 to 4, for PWM), or 0 if none
// - ADC device, or NULL if none
// - ADC channel, or ADCx if none

extern const stm32_pin_info PIN_MAP[BOARD_NR_GPIO_PINS] = {
/*
    gpio_dev *gpio_device;      GPIO device 
    timer_dev *timer_device;    Pin's timer device, if any.
    const adc_dev *adc_device;  ADC device, if any. 
    uint8 gpio_bit;             Pin's GPIO port bit. 
    uint8 timer_channel;        Timer channel, or 0 if none. 
    uint8 adc_channel;          Pin ADC channel, or ADCx if none. 
*/

	/* Top header */	

#ifdef MCU_STM32F103RE
{ &gpioa, &timer1, NULL , 10, 3, ADCx},// D0 |PA10,		//TIM1_3|RX_1
{ &gpioa, &timer1, NULL , 9 , 2, ADCx},// D1 |PA9	,	//TIM1_2|TX_1
{ &gpioc, NULL   , &adc1, 2 , 0, 12  },// D2 |PC2	,	//ADC_12
{ &gpioc, NULL   , &adc1, 3 , 0, 13  },// D3 |PC3	,	//ACD_13
{ &gpioc, NULL   , &adc1, 4 , 0, 14  },// D4 |PC4	,	//ADC_14	
{ &gpioc, NULL   , &adc1, 5 , 0, 15  },// D5 |PC5	,	//ADC_15
{ &gpioc, &timer8, NULL , 6 , 1, ADCx},// D6 |PC6	,	//TIM3_1|TIM8_1|I2S2_MCK
{ &gpioc, &timer8, NULL , 7 , 2, ADCx},// D7 |PC7	,	//TIM3_2|TIM8_2|I2S3_MCK

{ &gpioc, NULL   , &adc1, 0 , 0, 10	},// D8 |PC0	,	//ADC_10
{ &gpioc, NULL   , &adc1, 1 , 0, 11	},// D9 |PC1	,	//ADC_11
{ &gpioa, &timer2, NULL , 15, 1, ADCx},// D10|PA15,		//NSS_1|NSS_3|TIM2_1|I2S3_WS
{ &gpiob, &timer3, NULL , 5 , 2, ADCx},// D11|PB5	,	//MOSI_3|MOSI_1|TIM3_2|I2S3_SD
{ &gpiob, &timer3, NULL , 4 , 1, ADCx},// D12|PB4	,	//MISO_3|MISO_1|TIM3_1
{ &gpiob, &timer2, NULL , 3 , 2, ADCx},// D13|PB3	,	//SCK_3|SCK_1|TIM2_2|I2S3_CK

{ &gpiob, &timer4, NULL , 7 , 2, ADCx},// D14|PB7	,	//SDA_1|TIM4_2|RX_1
{ &gpiob, &timer4, NULL , 6 , 1, ADCx},// D15|PB6	,	//SCL_1|TIM4_1|TX_1

{ &gpioc, &timer8, NULL , 8 , 3, ADCx},// D16|PC8	,	//TIM3_3|TIM8_3|SDIO_D0
{ &gpioc, &timer8, NULL , 9 , 4, ADCx},// D17|PC9	,	//TIM3_4|TIM8_4|SDIO_D1	
{ &gpiob, &timer4, NULL , 8 , 3, ADCx},// D18|PB8	,	//CAN_RX|SCL_1|TIM4_3|SDIO_D4
{ &gpiob, &timer4, NULL , 9 , 4, ADCx},// D19|PB9	,	//CAN_TX|SDA_1|TIM4_4|SDIO_D5
{ &gpiob, &timer2, NULL , 10, 3, ADCx},// D20|PB10,		//SCL_2|TX_3|TIM2_3
{ &gpiob, &timer2, NULL , 11, 4, ADCx},// D21|PB11,		//SDA_2|RX_3|TIM2_4
 
{ &gpiod, NULL   , NULL , 2 , 0, ADCx},// D22|PD2	,	//RX_5|SDIO_CMD
{ &gpioc, NULL   , NULL , 11, 0, ADCx},// D23|PC11,		//RX_3|RX_4|SDIO_D3
{ &gpioc, NULL   , NULL , 12, 0, ADCx},// D24|PC12,		//TX_5|SDIO_CK
{ &gpioa, &timer3, &adc1, 6 , 1, 6	 },// D25|PA6	,	//ADC_6|TIM3_1|MISO_1
{ &gpioa, &timer3, &adc1, 7 , 2, 7	 },// D26|PA7	,	//ADC_7|TIM3_2|MOSI_1
{ &gpioa, &timer1, NULL , 8 , 1, ADCx},// D27|PA8	,	//TIM1_1

{ &gpiob, &timer3, &adc1, 1 , 4, 9   },// D28|PB1	,	//ADC_9|TIM3_4
{ &gpiob, NULL	 , NULL , 12, 0, ADCx},// D29|PB12,		//NSS_2
{ &gpiob, NULL	 , NULL , 2 , 0, ADCx},// D30|PB2	,	
{ &gpiob, NULL	 , NULL , 15, 0, ADCx},// D31|PB15,		//MOSI_2
{ &gpiob, NULL	 , NULL , 13, 0, ADCx},// D32|PB13,		//SCK_2
{ &gpiob, &timer3, &adc1, 0 , 3, 8   },// D33|PB0	,	//ADC_8|TIM3_3

{ &gpiob, NULL   , NULL , 14, 0, ADCx},// D34|PB14,
{ &gpioc, NULL   , NULL , 13, 0, ADCx},// D35|PC13,LED_BUILDIN
{ &gpioc, NULL   , NULL , 10, 0, ADCx},// D36|PC10,USB_DISC
{ &gpioa, NULL   , NULL , 11, 0, ADCx},// D37|PA11,USB_DM
{ &gpioa, NULL   , NULL , 12, 0, ADCx},// D38|PA12,USB_DP
{ &gpioa, NULL   , NULL , 13, 0, ADCx},// D39|PA13,SWDIO
{ &gpioa, NULL   , NULL , 14, 0, ADCx},// D40|PA14,SWCLK

{ &gpioa, &timer5, &adc1, 0 , 1, 0   },// D41,A0|PA0	,	//ADC_0|TIM2_1|TIM5_1|WKUP
{ &gpioa, &timer5, &adc1, 1 , 2, 1   },// D42,A1|PA1	,	//ADC_1|TIM2_2|TIM5_2
{ &gpioa, &timer5, &adc1, 2 , 3, 2   },// D43,A2|PA2	,	//ADC_2|TIM2_3|TIM5_3|TX_2
{ &gpioa, &timer5, &adc1, 3 , 4, 3   },// D44,A3|PA3	,	//ADC_3|TIM2_4|TIM5_4|RX_2
{ &gpioa, NULL   , &adc1, 4 , 0, 4   },// D45,A4|PA4	,	//ADC_4|NSS_1|DAC_1
{ &gpioa, NULL   , &adc1, 5 , 0, 5   } // D46,A5|PA5		//ADC_5|SCK_1|DAC_2
#else
{ &gpioa, &timer1, NULL , 10, 3, ADCx},// D0 |PA10,		//TIM1_3|RX_1
{ &gpioa, &timer1, NULL , 9 , 2, ADCx},// D1 |PA9	,	//TIM1_2|TX_1
{ &gpioc, NULL   , &adc1, 2 , 0, 12  },// D2 |PC2	,	//ADC_12
{ &gpioc, NULL   , &adc1, 3 , 0, 13  },// D3 |PC3	,	//ACD_13
{ &gpioc, NULL   , &adc1, 4 , 0, 14  },// D4 |PC4	,	//ADC_14	
{ &gpioc, NULL   , &adc1, 5 , 0, 15  },// D5 |PC5	,	//ADC_15
{ &gpioc, &timer3, NULL , 6 , 1, ADCx},// D6 |PC6	,	//TIM3_1|TIM8_1|I2S2_MCK
{ &gpioc, &timer3, NULL , 7 , 2, ADCx},// D7 |PC7	,	//TIM3_2|TIM8_2|I2S3_MCK

{ &gpioc, NULL   , &adc1, 0 , 0, 10	},// D8 |PC0	,	//ADC_10
{ &gpioc, NULL   , &adc1, 1 , 0, 11	},// D9 |PC1	,	//ADC_11
{ &gpioa, &timer2, NULL , 15, 1, ADCx},// D10|PA15,		//NSS_1|NSS_3|TIM2_1|I2S3_WS
{ &gpiob, &timer3, NULL , 5 , 2, ADCx},// D11|PB5	,	//MOSI_3|MOSI_1|TIM3_2|I2S3_SD
{ &gpiob, &timer3, NULL , 4 , 1, ADCx},// D12|PB4	,	//MISO_3|MISO_1|TIM3_1
{ &gpiob, &timer2, NULL , 3 , 2, ADCx},// D13|PB3	,	//SCK_3|SCK_1|TIM2_2|I2S3_CK

{ &gpiob, &timer4, NULL , 7 , 2, ADCx},// D14|PB7	,	//SDA_1|TIM4_2|RX_1
{ &gpiob, &timer4, NULL , 6 , 1, ADCx},// D15|PB6	,	//SCL_1|TIM4_1|TX_1

{ &gpioc, &timer3, NULL , 8 , 3, ADCx},// D16|PC8	,	//TIM3_3|TIM8_3|SDIO_D0
{ &gpioc, &timer3, NULL , 9 , 4, ADCx},// D17|PC9	,	//TIM3_4|TIM8_4|SDIO_D1	
{ &gpiob, &timer4, NULL , 8 , 3, ADCx},// D18|PB8	,	//CAN_RX|SCL_1|TIM4_3|SDIO_D4
{ &gpiob, &timer4, NULL , 9 , 4, ADCx},// D19|PB9	,	//CAN_TX|SDA_1|TIM4_4|SDIO_D5
{ &gpiob, &timer2, NULL , 10, 3, ADCx},// D20|PB10,		//SCL_2|TX_3|TIM2_3
{ &gpiob, &timer2, NULL , 11, 4, ADCx},// D21|PB11,		//SDA_2|RX_3|TIM2_4
 
{ &gpiod, NULL   , NULL , 2 , 0, ADCx},// D22|PD2	,	//RX_5|SDIO_CMD
{ &gpioc, NULL   , NULL , 11, 0, ADCx},// D23|PC11,		//RX_3|RX_4|SDIO_D3
{ &gpioc, NULL   , NULL , 12, 0, ADCx},// D24|PC12,		//TX_5|SDIO_CK
{ &gpioa, &timer3, &adc1, 6 , 1, 6	 },// D25|PA6	,	//ADC_6|TIM3_1|MISO_1
{ &gpioa, &timer3, &adc1, 7 , 2, 7	 },// D26|PA7	,	//ADC_7|TIM3_2|MOSI_1
{ &gpioa, &timer1, NULL , 8 , 1, ADCx},// D27|PA8	,	//TIM1_1

{ &gpiob, &timer3, &adc1, 1 , 4, 9   },// D28|PB1	,	//ADC_9|TIM3_4
{ &gpiob, NULL	 , NULL , 12, 0, ADCx},// D29|PB12,		//NSS_2
{ &gpiob, NULL	 , NULL , 2 , 0, ADCx},// D30|PB2	,	
{ &gpiob, NULL	 , NULL , 15, 0, ADCx},// D31|PB15,		//MOSI_2
{ &gpiob, NULL	 , NULL , 13, 0, ADCx},// D32|PB13,		//SCK_2
{ &gpiob, &timer3, &adc1, 0 , 3, 8   },// D33|PB0	,	//ADC_8|TIM3_3

{ &gpiob, NULL   , NULL , 14, 0, ADCx},// D34|PB14,
{ &gpioc, NULL   , NULL , 13, 0, ADCx},// D35|PC13,LED_BUILDIN
{ &gpioc, NULL   , NULL , 10, 0, ADCx},// D36|PC10,USB_DISC
{ &gpioa, NULL   , NULL , 11, 0, ADCx},// D37|PA11,USB_DM
{ &gpioa, NULL   , NULL , 12, 0, ADCx},// D38|PA12,USB_DP
{ &gpioa, NULL   , NULL , 13, 0, ADCx},// D39|PA13,SWDIO
{ &gpioa, NULL   , NULL , 14, 0, ADCx},// D40|PA14,SWCLK

{ &gpioa, &timer2, &adc1, 0 , 1, 0   },// D41,A0|PA0	,	//ADC_0|TIM2_1|TIM5_1|WKUP
{ &gpioa, &timer2, &adc1, 1 , 2, 1   },// D42,A1|PA1	,	//ADC_1|TIM2_2|TIM5_2
{ &gpioa, &timer2, &adc1, 2 , 3, 2   },// D43,A2|PA2	,	//ADC_2|TIM2_3|TIM5_3|TX_2
{ &gpioa, &timer2, &adc1, 3 , 4, 3   },// D44,A3|PA3	,	//ADC_3|TIM2_4|TIM5_4|RX_2
{ &gpioa, NULL   , &adc1, 4 , 0, 4   },// D45,A4|PA4	,	//ADC_4|NSS_1|DAC_1
{ &gpioa, NULL   , &adc1, 5 , 0, 5   } // D46,A5|PA5		//ADC_5|SCK_1|DAC_2

#endif
};

/*  Basically everything that is defined as having a timer us PWM */
extern const uint8 boardPWMPins[BOARD_NR_PWM_PINS] __FLASH__ = {
    PA10,PA9 ,PC6 ,PC7 ,PA15,PB5 ,PB4 ,PB3 ,PB7 ,PB6 ,
	PC8 ,PC9 ,PB8 ,PB9 ,PB10,PB11,PA6 ,PA7 ,PA8 ,PB1 ,
	PB0 ,PA0 ,PA1 ,PA2 ,PA3
};

/*  Basically everything that is defined having ADC */
extern const uint8 boardADCPins[BOARD_NR_ADC_PINS] __FLASH__ = {
    PA0,PA1,PA2,PA3,PA4,PA5,PC2,PC3,PC4,PC5,PC0,PC1,PA6,PA7,PB1,PB0
};

/* not sure what this us used for */
extern const uint8 boardUsedPins[BOARD_NR_USED_PINS] __FLASH__ = {
    BOARD_JTMS_SWDIO_PIN,
    BOARD_JTCK_SWCLK_PIN,
	USB_DP,
	USB_DM

};

/* 
 * Roger Clark
 * 
 * 2015/05/28
 *
 * Moved definitions for Hardware Serial devices from HardwareSerial.cpp so that each board can define which Arduino "Serial" instance
 * Maps to which hardware serial port on the microprocessor
 *
 * Note. Maple Ret always has SERIAL USB, so there is no need for the #fidef for this
 * As its a High Density device (F103RE), it has 6 hardware serial devices. 
 */		

DEFINE_HWSERIAL(Serial1, 1);
DEFINE_HWSERIAL(Serial2, 2);
DEFINE_HWSERIAL(Serial3, 3);
#ifdef MCU_STM32F103RE
DEFINE_HWSERIAL_UART(Serial4, 4);
DEFINE_HWSERIAL_UART(Serial5, 5);
#endif

#ifndef SERIAL_USB
#define Serial Serial1
#endif


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
 * @file   MakeArduino_F103RE.h
 * @author Marti Bolivar <mbolivar@leaflabs.com>
 * @brief  Private include file for Maple RET6 Edition in boards.h
 *
 * See maple.h for more information on these definitions.
 */

#ifndef _BOARDS_MAKEARDUINO_STM32F103RE_H_
#define _BOARDS_MAKEARDUINO_STM32F103RE_H_

/* A few of these values will seem strange given that it's a
 * high-density board. */
#define CYCLES_PER_MICROSECOND  72
#define SYSTICK_RELOAD_VAL     (F_CPU/1000) - 1 /* takes a cycle to reload */

// USARTS
#define BOARD_NR_USARTS         5
#define BOARD_USART1_TX_PIN     PA9
#define BOARD_USART1_RX_PIN     PA10

#define BOARD_USART2_TX_PIN     PA2
#define BOARD_USART2_RX_PIN     PA3

#define BOARD_USART3_TX_PIN     PB10
#define BOARD_USART3_RX_PIN     PB11

#define BOARD_USART4_TX_PIN     PC10 /* USB_DESC*/
#define BOARD_USART4_RX_PIN     PC11

#define BOARD_USART5_TX_PIN     PC12
#define BOARD_USART5_RX_PIN     PD2

/* Note:
 *
 * SPI3 is unusable due to pin 43 (PB4) and NRST tie-together :(, but
 * leave the definitions so as not to clutter things up.  This is only
 * OK since RET6 Ed. is specifically advertised as a beta board. */
#define BOARD_NR_SPI            3
#define BOARD_SPI1_NSS_PIN      PA4
#define BOARD_SPI1_SCK_PIN      PA5
#define BOARD_SPI1_MISO_PIN     PA6
#define BOARD_SPI1_MOSI_PIN     PA7

#define BOARD_SPI2_NSS_PIN      PB12
#define BOARD_SPI2_SCK_PIN      PB13
#define BOARD_SPI2_MISO_PIN     PB14
#define BOARD_SPI2_MOSI_PIN     PB15


#define BOARD_SPI3_NSS_PIN      PA15
#define BOARD_SPI3_SCK_PIN      PB3
#define BOARD_SPI3_MISO_PIN     PB4
#define BOARD_SPI3_MOSI_PIN     PB5


/* GPIO A to E = 5 * 16  - BOOT1 not used = 79*/
#define BOARD_NR_GPIO_PINS      47
/* Note: NOT 19. The missing one is D38 a.k.a. BOARD_BUTTON_PIN, which
 * isn't broken out to a header and is thus unusable for PWM. */
#define BOARD_NR_PWM_PINS       25
#define BOARD_NR_ADC_PINS       16
#define BOARD_NR_USED_PINS      4

/* USB configuration.  BOARD_USB_DISC_DEV is the GPIO port containing
 * the USB_DISC pin, and BOARD_USB_DISC_BIT is that pin's bit. */
#define BOARD_USB_DISC_DEV      GPIOA
#define BOARD_USB_DISC_BIT      12


/* Pin aliases: these give the GPIO port/bit for each pin as an
 * enum. These are optional, but recommended. They make it easier to
 * write code using low-level GPIO functionality. */
enum {
	//Digital Pin
PA10,		//D0 ,TIM1_3|RX_1
PA9	,		//D1 ,TIM1_2|TX_1
PC2	,		//D2 ,ADC_12
PC3	,		//D3 ,ACD_13
PC4	,		//D4 ,ADC_14	
PC5	,		//D5 ,ADC_15
PC6	,		//D6 ,TIM3_1|TIM8_1|I2S2_MCK
PC7	,		//D7 ,TIM3_2|TIM8_2|I2S3_MCK

PC0	,		//D8 ,ADC_10
PC1	,		//D9 ,ADC_11
PA15,		//D10,NSS_1|NSS_3|TIM2_1|I2S3_WS
PB5	,		//D11,MOSI_3|MOSI_1|TIM3_2|I2S3_SD
PB4	,		//D12,MISO_3|MISO_1|TIM3_1
PB3	,		//D13,SCK_3|SCK_1|TIM2_2|I2S3_CK
//I2C	
PB7	,		//D14,SDA_1|TIM4_2|RX_1
PB6	,		//D15,SCL_1|TIM4_1|TX_1

//EXT1
PC8	,		//D16,TIM3_3|TIM8_3|SDIO_D0
PC9	,		//D17,TIM3_4|TIM8_4|SDIO_D1	
PB8	,		//D18,CAN_RX|SCL_1|TIM4_3|SDIO_D4
PB9	,		//D19,CAN_TX|SDA_1|TIM4_4|SDIO_D5
PB10,		//D20,SCL_2|TX_3|TIM2_3
PB11,		//D21,SDA_2|RX_3|TIM2_4

//EXT_2
PD2	,		//D22,RX_5|SDIO_CMD
PC11,		//D23,RX_3|RX_4|SDIO_D3
PC12,		//D24,TX_5|SDIO_CK
PA6	,		//D25,ADC_6|TIM3_1|MISO_1
PA7	,		//D26,ADC_7|TIM3_2|MOSI_1
PA8	,		//D27,TIM1_1
//SPI TFT Port
PB1	,		//D28,ADC_9|TIM3_4
PB12,		//D29,NSS_2
PB2	,		//D30,
PB15,		//D31,MOSI_2
PB13,		//D32,SCK_2
PB0	,		//D33,ADC_8|TIM3_3
//System Pins
PB14,		//D34,
PC13,		//D35,BOARD_LED
PC10,		//D36,USB_DISC
PA11,		//D37,USB_DM
PA12,		//D38,USB_DP
PA13,		//D39,SWDIO
PA14,		//D40,SWCLK

//Analog Pins
PA0	,		//D41,ADC_0|TIM2_1|TIM5_1|WKUP
PA1	,		//D42,ADC_1|TIM2_2|TIM5_2
PA2	,		//D43,ADC_2|TIM2_3|TIM5_3|TX_2
PA3	,		//D44,ADC_3|TIM2_4|TIM5_4|RX_2
PA4	,		//D45,ADC_4|NSS_1|DAC_1
PA5			//D46,ADC_5|SCK_1|DAC_2
};

#define USB_DM 					PA11
#define USB_DP 					PA12
#define BOARD_JTMS_SWDIO_PIN    PA13
#define BOARD_JTCK_SWCLK_PIN    PA14
/*
 * SDIO Pins
 */
#define BOARD_SDIO_D0 			PC8
#define BOARD_SDIO_D1 			PC9
#define BOARD_SDIO_D2 			PC10 /*Unuseable*/
#define BOARD_SDIO_D3 			PC11
#define BOARD_SDIO_CLK 			PC12
#define BOARD_SDIO_CMD 			PD2

#define LED_BUILTIN				PC13

#endif


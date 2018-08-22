#ifndef _pin_magic_
#define _pin_magic_

// This header file serves two purposes:
//
// 1) Isolate non-portable MCU port- and pin-specific identifiers and
// operations so the library code itself remains somewhat agnostic
// (PORTs and pin numbers are always referenced through macros).
//
// 2) GCC doesn't always respect the "inline" keyword, so this is a
// ham-fisted manner of forcing the issue to minimize function calls.
// This sometimes makes the library a bit bigger than before, but fast++.
// However, because they're macros, we need to be SUPER CAREFUL about
// parameters -- for example, write8(x) may expand to multiple PORT
// writes that all refer to x, so it needs to be a constant or fixed
// variable and not something like *ptr++ (which, after macro
// expansion, may increment the pointer repeatedly and run off into
// la-la land). Macros also give us fine-grained control over which
// operations are inlined on which boards (balancing speed against
// available program space).
// When using the TFT shield, control and data pins exist in set physical
// locations, but the ports and bitmasks corresponding to each vary among
// boards. A separate set of pin definitions is given for each supported
// board type.
// When using the TFT breakout board, control pins are configurable but
// the data pins are still fixed -- making every data pin configurable
// would be much too slow. The data pin layouts are not the same between
// the shield and breakout configurations -- for the latter, pins were
// chosen to keep the tutorial wiring manageable more than making optimal
// use of ports and bitmasks. So there's a second set of pin definitions
// given for each supported board.
// Shield pin usage:
/*
// Breakout pin usage:
// LCD Data Bit : 7 6 5 4 3 2 1 0
// Uno dig. pin : 7 6 5 4 3 2 9 8
// Uno port/pin : PD7 PD6 PD5 PD4 PD3 PD2 PB1 PB0

// Mega dig. pin: 27 6 5 4 3 2 9 8
// Mega port/pin: PH4 PH3 PE3 PG5 PE5 PE4 PH6 PH5

// Pixel read operations require a minimum 400 nS delay from RD_ACTIVE
// to polling the input pins. At 16 MHz, one machine cycle is 62.5 nS.
// This code burns 7 cycles (437.5 nS) doing nothing; the RJMPs are
// equivalent to two NOPs each, final NOP burns the 7th cycle, and the
// last line is a radioactive mutant emoticon.

#define DELAY7 \
asm volatile( \
"rjmp .+0" "\n\t" \
"rjmp .+0" "\n\t" \
"rjmp .+0" "\n\t" \
"nop" "\n" \
::);


// As part of the inline control, macros reference other macros...if any
// of these are left undefined, an equivalent function version (non-inline)
// is declared later. The Uno has a moderate amount of program space, so
// only write8() is inlined -- that one provides the most performance
// benefit, but unfortunately also generates the most bloat. This is
// why only certain cases are inlined for each board.

// Arduino Mega, ADK, etc.
// Mega port/pin: PH4 PH3 PE3 PG5 PE5 PE4 PH6 PH5
// Because the MEGA port to pin mapping is very messy it is necessary to shift the data bits around a lot.

  #define write8inline(d) {                          \
    PORTE = (PORTE & B11001111) | ((d << 2) & B00110000); \
	PORTE = (PORTE & B11110111) | ((d >> 2) & B00001000); \
	PORTG = (PORTG & B11011111) | ((d << 1) & B00100000); \
	PORTH = (PORTH & B11100111) | ((d >> 3) & B00011000); \
	PORTH = (PORTH & B10011111) | ((d << 5) & B01100000); \
	WR_STROBE; }
  #define read8inline(result) {                       \
    RD_ACTIVE;                                        \
    DELAY7;                                           \
    result = ((PINH & B00011000) << 3) | ((PINE & B00001000) << 2) | ((PING & B00100000) >> 1) |((PINE & B00110000) >> 2) | ((PINH & B01100000) >> 5); \
    RD_IDLE; }

 // // These set the PORT directions as required before the write and read
 // // operations.
  #define setWriteDirInline() { DDRE |=  B00111000; DDRG |=  B00100000; DDRH |= B01111000;}
  #define setReadDirInline() { DDRE &=  ~B00111000; DDRG &=  ~B00100000; DDRH &= ~B01111000;}


// All of the functions are inlined on the Arduino Mega. When using the
// breakout board, the macro versions aren't appreciably larger than the
// function equivalents, and they're super simple and fast. When using
// the shield, the macros become pretty complicated...but this board has
// so much code space, the macros are used anyway. If you need to free
// up program space, some macros can be removed, at a minor cost in speed.
#define write8 write8inline
#define read8 read8inline
#define setWriteDir setWriteDirInline
#define setReadDir setReadDirInline
#define writeRegister8 writeRegister8inline
#define writeRegister16 writeRegister16inline
#define writeRegisterPair writeRegisterPairInline
// When using the TFT breakout board, control pins are configurable.
#define RD_ACTIVE *rdPort &= rdPinUnset
#define RD_IDLE *rdPort |= rdPinSet
#define WR_ACTIVE *wrPort &= wrPinUnset
#define WR_IDLE *wrPort |= wrPinSet
#define CD_COMMAND *cdPort &= cdPinUnset
#define CD_DATA *cdPort |= cdPinSet
#define CS_ACTIVE *csPort &= csPinUnset
#define CS_IDLE *csPort |= csPinSet
*/

#if defined(__SAM3X8E__)

// Arduino Due

 #ifdef USE_ADAFRUIT_SHIELD_PINOUT

  #define RD_PORT PIOA				/*pin A0 */	
  #define WR_PORT PIOA				/*pin A1 */
  #define CD_PORT PIOA				/*pin A2 */
  #define CS_PORT PIOA				/*pin A3 */

  #define RD_MASK 0x00010000
  #define WR_MASK 0x01000000
  #define CD_MASK 0x00800000
  #define CS_MASK 0x00400000

  #define write8inline(d) { \
   PIO_Set(PIOD, (((d) & 0x08)<<(7-3))); \
   PIO_Clear(PIOD, (((~d) & 0x08)<<(7-3))); \
   PIO_Set(PIOC, (((d) & 0x01)<<(22-0)) | (((d) & 0x02)<<(21-1))| (((d) & 0x04)<<(29-2))| (((d) & 0x10)<<(26-4))| (((d) & 0x40)<<(24-6))| (((d) & 0x80)<<(23-7))); \
   PIO_Clear(PIOC, (((~d) & 0x01)<<(22-0)) | (((~d) & 0x02)<<(21-1))| (((~d) & 0x04)<<(29-2))| (((~d) & 0x10)<<(26-4))| (((~d) & 0x40)<<(24-6))| (((~d) & 0x80)<<(23-7))); \
   PIO_Set(PIOB, (((d) & 0x20)<<(27-5))); \
   PIO_Clear(PIOB, (((~d) & 0x20)<<(27-5))); \
   WR_STROBE; }

  #define read8inline(result) { \    
   RD_ACTIVE;   \
   delayMicroseconds(1);      \
   result = (((PIOC->PIO_PDSR & (1<<23)) >> (23-7)) | ((PIOC->PIO_PDSR & (1<<24)) >> (24-6)) | \
             ((PIOB->PIO_PDSR & (1<<27)) >> (27-5)) | ((PIOC->PIO_PDSR & (1<<26)) >> (26-4)) | \
             ((PIOD->PIO_PDSR & (1<< 7)) >> ( 7-3)) | ((PIOC->PIO_PDSR & (1<<29)) >> (29-2)) | \
             ((PIOC->PIO_PDSR & (1<<21)) >> (21-1)) | ((PIOC->PIO_PDSR & (1<<22)) >> (22-0))); \
   RD_IDLE;}

  #define setWriteDirInline() { \
   PIOD->PIO_MDDR |=  0x00000080; /*PIOD->PIO_SODR =  0x00000080;*/ PIOD->PIO_OER |=  0x00000080; PIOD->PIO_PER |=  0x00000080; \
   PIOC->PIO_MDDR |=  0x25E00000; /*PIOC->PIO_SODR =  0x25E00000;*/ PIOC->PIO_OER |=  0x25E00000; PIOC->PIO_PER |=  0x25E00000; \
   PIOB->PIO_MDDR |=  0x08000000; /*PIOB->PIO_SODR =  0x08000000;*/ PIOB->PIO_OER |=  0x08000000; PIOB->PIO_PER |=  0x08000000; }

  #define setReadDirInline() { \
	  pmc_enable_periph_clk( ID_PIOD ) ;	  pmc_enable_periph_clk( ID_PIOC ) ;	  pmc_enable_periph_clk( ID_PIOB ) ; \
   PIOD->PIO_PUDR |=  0x00000080; PIOD->PIO_IFDR |=  0x00000080; PIOD->PIO_ODR |=  0x00000080; PIOD->PIO_PER |=  0x00000080; \
   PIOC->PIO_PUDR |=  0x25E00000; PIOC->PIO_IFDR |=  0x25E00000; PIOC->PIO_ODR |=  0x25E00000; PIOC->PIO_PER |=  0x25E00000; \
   PIOB->PIO_PUDR |=  0x08000000; PIOB->PIO_IFDR |=  0x08000000; PIOB->PIO_ODR |=  0x08000000; PIOB->PIO_PER |=  0x08000000; }

   // Control signals are ACTIVE LOW (idle is HIGH)
   // Command/Data: LOW = command, HIGH = data
   // These are single-instruction operations and always inline
   #define RD_ACTIVE  RD_PORT->PIO_CODR |= RD_MASK
   #define RD_IDLE    RD_PORT->PIO_SODR |= RD_MASK
   #define WR_ACTIVE  WR_PORT->PIO_CODR |= WR_MASK
   #define WR_IDLE    WR_PORT->PIO_SODR |= WR_MASK
   #define CD_COMMAND CD_PORT->PIO_CODR |= CD_MASK
   #define CD_DATA    CD_PORT->PIO_SODR |= CD_MASK
   #define CS_ACTIVE  CS_PORT->PIO_CODR |= CS_MASK
   #define CS_IDLE    CS_PORT->PIO_SODR |= CS_MASK


 #else // Due w/Breakout board

    #define write8inline(d) { \
		PIO_Set(PIOC, (((d) & 0xFF)<<1)); \
		PIO_Clear(PIOC, (((~d) & 0xFF)<<1)); \
		WR_STROBE; }

    #define read8inline(result) { \
		RD_ACTIVE;   \
		delayMicroseconds(1);      \
		result = ((PIOC->PIO_PDSR & 0x1FE) >> 1); \
		RD_IDLE;}

    #define setWriteDirInline() { \
	    PIOC->PIO_MDDR |=  0x000001FE; /*PIOC->PIO_SODR |=  0x000001FE;*/ PIOC->PIO_OER |=  0x000001FE; PIOC->PIO_PER |=  0x000001FE; }

    #define setReadDirInline() { \
		pmc_enable_periph_clk( ID_PIOC ) ; \
		PIOC->PIO_PUDR |=  0x000001FE; PIOC->PIO_IFDR |=  0x000001FE; PIOC->PIO_ODR |=  0x000001FE; PIOC->PIO_PER |=  0x000001FE; }

    // When using the TFT breakout board, control pins are configurable.
    #define RD_ACTIVE	rdPort->PIO_CODR |= rdPinSet		//PIO_Clear(rdPort, rdPinSet)
    #define RD_IDLE		rdPort->PIO_SODR |= rdPinSet		//PIO_Set(rdPort, rdPinSet)	
    #define WR_ACTIVE	wrPort->PIO_CODR |= wrPinSet		//PIO_Clear(wrPort, wrPinSet)
    #define WR_IDLE		wrPort->PIO_SODR |= wrPinSet		//PIO_Set(wrPort, wrPinSet)
    #define CD_COMMAND	cdPort->PIO_CODR |= cdPinSet		//PIO_Clear(cdPort, cdPinSet)
    #define CD_DATA		cdPort->PIO_SODR |= cdPinSet		//PIO_Set(cdPort, cdPinSet)
    #define CS_ACTIVE	csPort->PIO_CODR |= csPinSet		//PIO_Clear(csPort, csPinSet)
    #define CS_IDLE		csPort->PIO_SODR |= csPinSet		//PIO_Set(csPort, csPinSet)

 #endif

 #endif



// Data read and write strobes, ~2 instructions and always inline
#define RD_STROBE { RD_ACTIVE; RD_IDLE; }
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }
// These higher-level operations are usually functionalized,
// except on Mega where's there's gobs and gobs of program space.
// Set value of TFT register: 8-bit address, 8-bit value
#define writeRegister8inline(a, d) { \
CD_COMMAND; write8(a); CD_DATA; write8(d); }
// Set value of TFT register: 16-bit address, 16-bit value
// See notes at top about macro expansion, hence hi & lo temp vars
#define writeRegister16inline(a, d) { \
uint8_t hi, lo; \
hi = (a) >> 8; lo = (a); CD_COMMAND; write8(hi); write8(lo); \
hi = (d) >> 8; lo = (d); CD_DATA ; write8(hi); write8(lo); }
// Set value of 2 TFT registers: Two 8-bit addresses (hi & lo), 16-bit value
#define writeRegisterPairInline(aH, aL, d) { \
uint8_t hi = (d) >> 8, lo = (d); \
CD_COMMAND; write8(aH); CD_DATA; write8(hi); \
CD_COMMAND; write8(aL); CD_DATA; write8(lo); }


  #define RD_PORT PIOA
  #define RD_PIN  16
  #define WR_PORT PIOA
  #define WR_PIN  24
  #define CD_PORT PIOA
  #define CD_PIN  23
  #define CS_PORT PIOA
  #define CS_PIN  22
  #define RESET_PORT PIOA
  #define RESET_PIN  6

  // configure macros for data bus

#define BMASK         (1<<25)
#define CMASK         (0xBF << 21)

#define write_8(x)   {  PIOB->PIO_CODR = BMASK; PIOC->PIO_CODR = CMASK; \
                        PIOC->PIO_SODR = (((x) & (1<<0)) << 22); \
                        PIOC->PIO_SODR = (((x) & (1<<1)) << 20); \
                        PIOB->PIO_SODR = (((x) & (1<<2)) << 23); \
                        PIOC->PIO_SODR = (((x) & (1<<3)) << 25); \
                        PIOC->PIO_SODR = (((x) & (1<<4)) << 22); \
                        PIOC->PIO_SODR = (((x) & (1<<5)) << 20); \
                        PIOC->PIO_SODR = (((x) & (1<<6)) << 18); \
                        PIOC->PIO_SODR = (((x) & (1<<7)) << 16); \
					 }

#define read_8()      ( ((PIOC->PIO_PDSR & (1<<22)) >> 22)\
                      | ((PIOC->PIO_PDSR & (1<<21)) >> 20)\
                      | ((PIOB->PIO_PDSR & (1<<25)) >> 23)\
                      | ((PIOC->PIO_PDSR & (1<<28)) >> 25)\
                      | ((PIOC->PIO_PDSR & (1<<26)) >> 22)\
                      | ((PIOC->PIO_PDSR & (1<<25)) >> 20)\
                      | ((PIOC->PIO_PDSR & (1<<24)) >> 18)\
                      | ((PIOC->PIO_PDSR & (1<<23)) >> 16)\
                      )
  #define setWriteDir() { PIOB->PIO_OER = BMASK; PIOC->PIO_OER = CMASK; }
  #define setReadDir()  { \
                          PMC->PMC_PCER0 = (1 << ID_PIOB)|(1 << ID_PIOC);\
						  PIOB->PIO_ODR = BMASK; PIOC->PIO_ODR = CMASK;\
						}      
#define write8(x)     { write_8(x); WR_ACTIVE; WR_STROBE; WR_IDLE; }
#define write16(x)    { uint8_t h = (x)>>8, l = x; write8(h); write8(l); }
#define READ_8(dst)   { RD_STROBE; dst = read_8(); RD_IDLE; }
#define READ_16(dst)  { RD_STROBE; dst = read_8(); RD_IDLE; RD_STROBE; dst = (dst<<8) | read_8(); RD_IDLE; }
// Shield Control macros.
#define PIN_LOW(port, pin)    (port)->PIO_CODR = (1<<(pin))
#define PIN_HIGH(port, pin)   (port)->PIO_SODR = (1<<(pin))
#define PIN_OUTPUT(port, pin) (port)->PIO_OER = (1<<(pin))

 #endif


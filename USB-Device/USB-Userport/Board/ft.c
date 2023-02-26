/*
  * The USB-Userport *
  Copyright 2023  René Trapp (rene [dot] trapp (-at-) web [dot] de)
  License: "The MIT license" - see license.txt
*/


/** @file
 *  \~English
 *   @brief A Sparkfun ProMicro interfaces to a fischertechnik 66843.
 *
 *  \~German
 *   @brief Ein Sparkfun ProMicro "spricht" mit einem fischertechnik
 *   66843.
 */


#include "ft.h"
#include <util/delay.h>


#define CLK_WIRE              1
#define DATA_OUT_WIRE         2
#define C64_DATA_IN_WIRE      3
#define LOAD_IN_WIRE          4
#define TRIGGER_X_WIRE        6
#define TRIGGER_Y_WIRE        7
#define MMV_PULSES_WIRE       6
#define COUNT_IN_WIRE         7
#define LOAD_OUT_WIRE         6

#define CLK_CLEAR             PORTB &= ~(1 << CLK_WIRE)
#define CLK_SET               PORTB |=  (1 << CLK_WIRE)
#define LOAD_IN_CLEAR         PORTB &= ~(1 << LOAD_IN_WIRE)
#define LOAD_IN_SET           PORTB |=  (1 << LOAD_IN_WIRE)
#define LOAD_OUT_CLEAR        PORTE &= ~(1 << LOAD_OUT_WIRE)
#define LOAD_OUT_SET          PORTE |=  (1 << LOAD_OUT_WIRE)
#define DATA_OUT_CLEAR        PORTB &= ~(1 << DATA_OUT_WIRE)
#define DATA_OUT_SET          PORTB |=  (1 << DATA_OUT_WIRE)
#define DATA_IN_IS_SET        (PINB  &  (1 << C64_DATA_IN_WIRE))
#define TRIGGER_EX_CLEAR      PORTF &= ~(1 << TRIGGER_X_WIRE)
#define TRIGGER_EX_SET        PORTF |=  (1 << TRIGGER_X_WIRE)
#define TRIGGER_EY_CLEAR      PORTF &= ~(1 << TRIGGER_Y_WIRE)
#define TRIGGER_EY_SET        PORTF |=  (1 << TRIGGER_Y_WIRE)
#define MMV_PULSES_IS_SET     (PINC  &  (1 << MMV_PULSES_WIRE))


/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
   extern "C" {
#endif


typedef struct
{
   uint16_t OutToIf;
   uint16_t InFromIf;
   uint16_t stopSwitchEnabled;
   uint16_t stopSwitchInverted;
   uint16_t stopPatternFwd;
   uint16_t stopPatternBwd;
} ftIfRawData_t;


uint8_t ftModeEnabled = 0;
volatile ftIfRawData_t ifaceDataA;
volatile ftIfRawData_t ifaceDataB;
uint8_t selectWr;
uint8_t selectRd;
uint8_t selectSw;
volatile int8_t potiX;
volatile int8_t potiY;
volatile uint8_t outChange;
volatile uint8_t inpChange;
volatile uint8_t potxChange;
volatile uint8_t potyChange;
volatile uint8_t outChangePrev = 0;
volatile uint8_t inpChangePrev = 0;
volatile uint8_t potxChangePrev = 0;
volatile uint8_t potyChangePrev = 0;


typedef enum
{
   init = 0,
   potx_start,
   potx_wait,
   potx_done,
   poty_start,
   poty_wait,
   poty_done
} ftIfPotState_t;


/* -------------------------------------------------------------------------- */
/* Interface to application                                                   */
/* -------------------------------------------------------------------------- */


void ft66843_OutputsSetMaster(uint8_t vector)
{
   if (selectWr)
   {
      ifaceDataA.OutToIf = (ifaceDataA.OutToIf & 0xFF00) | vector;
      selectWr = 0;
   }
   else
   {
      ifaceDataB.OutToIf = (ifaceDataB.OutToIf & 0xFF00) | vector;
      selectWr = 1;
   }
}


void ft66843_OutputsSetSlave(uint8_t vector)
{
   if (selectWr)
      ifaceDataA.OutToIf = (ifaceDataA.OutToIf & 0x00FF) | (vector << 8);
   else
      ifaceDataB.OutToIf = (ifaceDataB.OutToIf & 0x00FF) | (vector << 8);
}


uint8_t ft66843_OutputsGetMaster(void)
{
   if (selectWr)
      return ifaceDataB.OutToIf;
   else
      return ifaceDataA.OutToIf;
}


uint8_t ft66843_OutputsGetSlave(void)
{
   if (selectWr)
      return ifaceDataB.OutToIf >> 8;
   else
      return ifaceDataA.OutToIf >> 8;
}


uint8_t ft66843_InputsGetMaster(void)
{
   if (selectRd)
   {
      selectRd = 0;
      return ifaceDataB.InFromIf;
   }
   else
   {
      selectRd = 1;
      return ifaceDataA.InFromIf;
   }
}


uint8_t ft66843_InputsGetSlave(void)
{
   if (selectRd)
      return ifaceDataB.InFromIf >> 8;
   else
      return ifaceDataA.InFromIf >> 8;
}


void ft66843_StopEnableSet(uint8_t master, uint8_t slave)
{
   if (selectSw)
      ifaceDataA.stopSwitchEnabled = (slave << 8) | master;
   else
      ifaceDataB.stopSwitchEnabled = (slave << 8) | master;
}


void ft66843_StopInversionSet(uint8_t master, uint8_t slave)
{
   if (selectSw)
      ifaceDataA.stopSwitchInverted = (slave << 8) | master;
   else
      ifaceDataB.stopSwitchInverted = (slave << 8) | master;
}


void ft66843_StopResponseFwdSet(uint8_t master, uint8_t slave)
{
   if (selectSw)
      ifaceDataA.stopPatternFwd = (slave << 8) | master;
   else
      ifaceDataB.stopPatternFwd = (slave << 8) | master;
}


void ft66843_StopResponseBwdSet(uint8_t master, uint8_t slave)
{
   if (selectSw)
   {
      ifaceDataA.stopPatternBwd = (slave << 8) | master;
      selectSw = 0;
   }
   else
   {
      ifaceDataB.stopPatternBwd = (slave << 8) | master;
      selectSw = 1;
   }
}


uint8_t ft66843_PotXGet(void)
{
   return potiX;
}


uint8_t ft66843_PotYGet(void)
{
   return potiY;
}


uint8_t ft66843_HasChanges(void)
{
   uint8_t retVal = 0;
   uint8_t x = outChange;
   if (x != outChangePrev)
      retVal |= ft66843_OUT_CHG;
   outChangePrev = x;
   x = inpChange;
   if (x != inpChangePrev)
      retVal |= ft66843_INP_CHG;
   inpChangePrev = x;
   x = potxChange;
   if (x != potxChangePrev)
      retVal |= ft66843_EX_CHG;
   potxChangePrev = x;
   x = potyChange;
   if (x != potyChangePrev)
      retVal |= ft66843_EY_CHG;
   potyChangePrev = x;
   return retVal;
}


/* -------------------------------------------------------------------------- */
/* Physical Layer Serial Data Exchange                                        */
/* -------------------------------------------------------------------------- */

// '4014: Relevant datasheet parameters vary with manufacturer!
// Worst case values found in a Motorola specification from 1995 but there
// might be worse around ...
// Propagation delay from clock pulse to Q outputs = 800ns @ 5V/50pF
// Fairchild says just 320ns @ 5V for same parameter
// Clock pulse width = 400ns @ 5V
// Parallel/Serial Control pulse width = 400ns @ 5V
// Setup Time P/S to Clock rise = 200ns @ 5V
// Clock frequency = 1.5MHz @ 5V => 11 CPU clocks @ 16MHz
// Shifts at clock posedge, master shall sample MISO a clock negedge.
// => SPI-MODE 1.
// Due to propagation delay from clock to any Q the master could also sample
// MISO with rising clock edge. No MISO hold time violation with ATmega32U4!
// => SPI-MODE 3 possible.
//
// ft66843: ST HCF4014BE
// Propagation delay from clock pulse to Q outputs = 320ns @ 5V/50pF
// Minimum Strobe Pulse time = 160ns @ 5V
// Minimum Clock Pulse Width = 180ns @ 5V
// Data Setup time D to Clock = 80ns @ 5V
// Clock frequency = 3MHz @ 5V => 6 CPU clocks @ 16MHz
//
// '4094: Relevant datasheet parameters similar at Motorola and Fairchild.
// Minimum Strobe Pulse time = 200ns @ 5V
// Minimum Clock Pulse Width = 200ns @ 5V
// Data Setup time D to Clock = 80ns @ 5V
// Clock frequency = 1.5MHz @ 5V => 11 CPU clocks @ 16MHz
// Shifts at clock posedge, master shall update MOSI at clock negedge.
// => SPI-MODE 3.
//
// One transfer can exchange a pair of bytes without restrictions.
// => ATmega32U4 HW-SPI, mode 3, up to 1 MHz, use 500 kHz just for robustness
/*
#define SPI_DISABLE           (0 << SPE)
#define SPI_ENABLE            (1 << SPE)
#define SPI_MSB_1st           (0 << DORD)
#define SPI_AS_MASTER         (1 << MSTR)
#define SPI_MODE0             ((0 << CPOL) | (0 << CPHA))
#define SPI_MODE1             ((0 << CPOL) | (1 << CPHA))
#define SPI_MODE2             ((1 << CPOL) | (0 << CPHA))
#define SPI_MODE3             ((1 << CPOL) | (1 << CPHA))
#define SPI_CLKDIV64          (1 << SPR1)
#define SPI_CLKx2             (1 << SPI2X)
#define SPI_IS_BUSY           (~(SPSR & (1 << SPIF)))

void ft66843_Xfer(void)
{
   LOAD_IN_SET;
   SPCR = SPI_DISABLE | SPI_MSB_1st | SPI_AS_MASTER | SPI_MODE3 | SPI_CLKDIV64;
   CLK_CLEAR;
   _delay_us(1);
   CLK_SET;
   _delay_us(1);
   LOAD_IN_CLEAR;
   _delay_us(1);
   SPSR = SPI_CLKx2;
   SPCR = SPI_ENABLE | SPI_MSB_1st | SPI_AS_MASTER | SPI_MODE3 | SPI_CLKDIV64;
   SPDR = selectWr ? ifaceDataA.OutToIfSlave : ifaceDataB.OutToIfSlave;
   while (SPI_IS_BUSY)
      ;
   if (selectRd)
      ifaceDataA.InFromMaster = SPDR;
   else
      ifaceDataB.InFromMaster = SPDR;
   SPDR = selectWr ? ifaceDataA.OutToIfMaster : ifaceDataB.OutToIfMaster;
   while (SPI_IS_BUSY)
      ;
   if (selectRd)
      ifaceDataA.InFromSlave = SPDR;
   else
      ifaceDataB.InFromSlave = SPDR;
   LOAD_OUT_SET;
   _delay_us(1);
   LOAD_OUT_CLEAR;
}
*/
// Even if SPI-Mode 3 and HW-SPI works @ 500 kHz (CLK:32) the transfer is far
// from being optimal and the CPU just waits roughly 250 clocks for nothing.
// It is way more convenient to do the transfers by a software routine and
// perform optimized shifting and sampling. Besides getting a faster transfer
// this also gives way to optimize the layout by using different GPIOs than
// the predefined ones for SPI.

#define DELAY_2_MCU_CLOCKS    asm volatile ("RJMP ."::) // 125 ns @ 16 MHz

uint16_t ft66843_Xfer(uint16_t toShift)
{
   // latch parallel data into *4014 shifter, CLK low for 500 ns
   LOAD_IN_SET;
   CLK_CLEAR;
   DELAY_2_MCU_CLOCKS;
   DELAY_2_MCU_CLOCKS;
   DELAY_2_MCU_CLOCKS;
   CLK_SET;
   LOAD_IN_CLEAR;
   // shift out data (MSB 1st) and collect incoming data (MSB 1st)
   // master data is in the low byte, slave data in the high byte
   // one bit takes 18 / 19 clocks (1125 ns / 1187.5 ns)
   // CLK lo =    8    clocks (500 ns)
   // CLK hi = 10 / 11 clocks (625 ns / 687.5 ns)
   for (int8_t i = 16; i > 0; i--)
   {
   
      if (toShift & 0x8000)
         DATA_OUT_SET;
      else
         DATA_OUT_CLEAR;
      CLK_CLEAR;
      toShift <<= 1;
      DELAY_2_MCU_CLOCKS;
      if (DATA_IN_IS_SET)
         toShift |= 0x0001;
      CLK_SET;
   }
   // prepare output latching
   LOAD_OUT_SET;
   _delay_us(1);
   // latch serial data to outputs of *4094 shifter 20 clocks after CLK_SET
   LOAD_OUT_CLEAR;
   // complete transfer takes about 21 µs at about 850 kHz
   // master data is in the low byte, slave data in the high byte
   return (toShift >> 8) | (toShift << 8);
}


/* -------------------------------------------------------------------------- */
/* Entry and Exit                                                             */
/* -------------------------------------------------------------------------- */


void ft66843_Init(void)
{
   // inputs from ft-interface
   DDRB  &= ~((1 << C64_DATA_IN_WIRE));
   DDRC  &= ~((1 << MMV_PULSES_WIRE));
   DDRD  &= ~((1 << COUNT_IN_WIRE));
   // output levels and pullups
   PORTB &= ~((1 << LOAD_IN_WIRE) | (1 << DATA_OUT_WIRE));
   PORTB |=   (1 << CLK_WIRE) | (1 << C64_DATA_IN_WIRE);
   PORTC |=   (1 << MMV_PULSES_WIRE);
   PORTD |=   (1 << COUNT_IN_WIRE);
   PORTE &= ~((1 << LOAD_OUT_WIRE));
   PORTF |=   (1 << TRIGGER_Y_WIRE) | (1 << TRIGGER_X_WIRE);
   // outputs to ft-interface
   DDRB  |=   (1 << LOAD_IN_WIRE) | (1 << DATA_OUT_WIRE) | (1 << CLK_WIRE);
   DDRE  |=   (1 << LOAD_OUT_WIRE);
   DDRF  |=   (1 << TRIGGER_Y_WIRE) | (1 << TRIGGER_X_WIRE);
   // pulse counter
   TIMSK0 = 0;
   TIFR0 = (1 << TOV0);
   TCNT0 = 0;
   TCCR0A = 0;
   TCCR0B = (0b111 << CS00);
   // function logic
   selectRd = 0;
   selectWr = 0;
   selectSw = 0;
   outChange = 0;
   inpChange = 0;
   potxChange = 0;
   potyChange = 0;
   potiX = 0;
   potiY = 0;
   ifaceDataB.OutToIf = 0;
   ifaceDataB.stopSwitchEnabled = 0;
   ifaceDataB.stopSwitchInverted = 0;
   ifaceDataB.stopPatternFwd = 0;
   ifaceDataB.stopPatternBwd = 0;
   ifaceDataB.InFromIf = 0;
   ifaceDataA.OutToIf = 0;
   ifaceDataA.stopSwitchEnabled = 0;
   ifaceDataA.stopSwitchInverted = 0;
   ifaceDataA.stopPatternFwd = 0;
   ifaceDataA.stopPatternBwd = 0;
   ifaceDataA.InFromIf = ft66843_Xfer(0);
   ftModeEnabled = ~0;
}


void ft66843_Leave(void)
{
   if (ftModeEnabled)
   {
      ftModeEnabled = 0;
      ft66843_Xfer(0);
   }
}


uint8_t ft66843_IsEnabled()
{
   return ftModeEnabled;
}


/* -------------------------------------------------------------------------- */
/* Needfull things                                                            */
/* -------------------------------------------------------------------------- */


// Take 'a' input when 'sel' = 0 and 'b' input when 'sel' = 1, bitwise!
uint16_t MUX(uint16_t sel, uint16_t a, uint16_t b)
{
   return (b & sel) | (a & ~sel);
}


/* -------------------------------------------------------------------------- */
/* Transfer and stop switch logic - real time capable                         */
/* -------------------------------------------------------------------------- */


void ft66843_CoreTask(void)
{
   static uint16_t stpSwActive = 0;
   static uint16_t inputsPrev = 0;
   static uint16_t outputsPrev = 0;
   static int8_t potiXPrev = 0;
   static int8_t potiYPrev = 0;
   static ftIfPotState_t potState = init;

   if (ftModeEnabled)
   {
      uint16_t outputs = selectWr ? ifaceDataB.OutToIf : ifaceDataA.OutToIf;
      uint16_t stpPatFwd = selectSw ? ifaceDataB.stopPatternFwd : ifaceDataA.stopPatternFwd;
      uint16_t stpPatBwd = selectSw ? ifaceDataB.stopPatternBwd : ifaceDataA.stopPatternBwd;
      // Master M1 <=> O2, O1
      if ((stpSwActive & 0b0000000000000001) && ((outputs & 0b0000000000000011) == 0b0000000000000001))
         outputs =   MUX(0b0000000000000011, outputs, stpPatFwd);
      if ((stpSwActive & 0b0000000000000010) && ((outputs & 0b0000000000000011) == 0b0000000000000010))
         outputs =   MUX(0b0000000000000011, outputs, stpPatBwd);
      // Master M2 <=> O4, O3
      if ((stpSwActive & 0b0000000000000100) && ((outputs & 0b0000000000001100) == 0b0000000000000100))
         outputs =   MUX(0b0000000000001100, outputs, stpPatFwd);
      if ((stpSwActive & 0b0000000000001000) && ((outputs & 0b0000000000001100) == 0b0000000000001000))
         outputs =   MUX(0b0000000000001100, outputs, stpPatBwd);
      // Master M3 <=> O6, O5
      if ((stpSwActive & 0b0000000000010000) && ((outputs & 0b0000000000110000) == 0b0000000000010000))
         outputs =   MUX(0b0000000000110000, outputs, stpPatFwd);
      if ((stpSwActive & 0b0000000000100000) && ((outputs & 0b0000000000110000) == 0b0000000000100000))
         outputs =   MUX(0b0000000000110000, outputs, stpPatBwd);
      // Master M4 <=> O8, O7
      if ((stpSwActive & 0b0000000001000000) && ((outputs & 0b0000000011000000) == 0b0000000001000000))
         outputs =   MUX(0b0000000011000000, outputs, stpPatFwd);
      if ((stpSwActive & 0b0000000010000000) && ((outputs & 0b0000000011000000) == 0b0000000010000000))
         outputs =   MUX(0b0000000011000000, outputs, stpPatBwd);
      // Slave M1 <=> O2, O1
      if ((stpSwActive & 0b0000000100000000) && ((outputs & 0b0000001100000000) == 0b0000000100000000))
         outputs =   MUX(0b0000001100000000, outputs, stpPatFwd);
      if ((stpSwActive & 0b0000001000000000) && ((outputs & 0b0000001100000000) == 0b0000001000000000))
         outputs =   MUX(0b0000001100000000, outputs, stpPatBwd);
      // Slave M2 <=> O4, O3
      if ((stpSwActive & 0b0000010000000000) && ((outputs & 0b0000110000000000) == 0b0000010000000000))
         outputs =   MUX(0b0000110000000000, outputs, stpPatFwd);
      if ((stpSwActive & 0b0000100000000000) && ((outputs & 0b0000110000000000) == 0b0000100000000000))
         outputs =   MUX(0b0000110000000000, outputs, stpPatBwd);
      // Slave M3 <=> O6, O5
      if ((stpSwActive & 0b0001000000000000) && ((outputs & 0b0011000000000000) == 0b0001000000000000))
         outputs =   MUX(0b0011000000000000, outputs, stpPatFwd);
      if ((stpSwActive & 0b0010000000000000) && ((outputs & 0b0011000000000000) == 0b0010000000000000))
         outputs =   MUX(0b0011000000000000, outputs, stpPatBwd);
      // Slave M4 <=> O8, O7
      if ((stpSwActive & 0b0100000000000000) && ((outputs & 0b1100000000000000) == 0b0100000000000000))
         outputs =   MUX(0b1100000000000000, outputs, stpPatFwd);
      if ((stpSwActive & 0b1000000000000000) && ((outputs & 0b1100000000000000) == 0b1000000000000000))
         outputs =   MUX(0b1100000000000000, outputs, stpPatBwd);
      if (outputs != outputsPrev)
         outChange = outChangePrev + 1;
      outputsPrev = outputs;
      if (selectWr)
         ifaceDataB.OutToIf = outputs;
      else
         ifaceDataA.OutToIf = outputs;

      uint16_t inputs = ft66843_Xfer(outputs);

      if (selectRd)
         ifaceDataB.InFromIf = inputs;
      else
         ifaceDataA.InFromIf = inputs;
      if (inputs != inputsPrev)
         inpChange = inpChangePrev + 1;
      inputsPrev = inputs;
      stpSwActive = inputs ^ (selectSw ? ifaceDataB.stopSwitchInverted : ifaceDataA.stopSwitchInverted);
      stpSwActive &= selectSw ? ifaceDataB.stopSwitchEnabled : ifaceDataA.stopSwitchEnabled;

      if (!MMV_PULSES_IS_SET)
      { // MMV not running
         switch (potState)
         {
            case potx_done:
               if (TIFR0 & (1 << TOV0))
                  potiX = 255;
               else
                  potiX = TCNT0;
               if (potiX != potiXPrev)
                  potxChange = potxChangePrev + 1;
               potiXPrev = potiX;
            case poty_start:
               TIFR0 = (1 << TOV0);
               TCNT0 = 0;
               TRIGGER_EY_CLEAR;
               _delay_us(2);
               TRIGGER_EY_SET;
               potState = poty_done;
               break;
            case poty_done:
               if (TIFR0 & (1 << TOV0))
                  potiY = 255;
               else
                  potiY = TCNT0;
               if (potiY != potiYPrev)
                  potyChange = potyChange + 1;
               potiYPrev = potiY;
            case potx_start:
               TIFR0 = (1 << TOV0);
               TCNT0 = 0;
               TRIGGER_EX_CLEAR;
               _delay_us(2);
               TRIGGER_EX_SET;
               potState = potx_done;
               break;
            default:
               potState = potx_start;
               break;
         }
      }
      else
      { // MMV (still) running
         // intentionally left blank
      }
   }
}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
   }
#endif

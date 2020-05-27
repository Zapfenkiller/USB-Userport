/*
  * The USB-Userport *
  Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)

  * LUFA Library *
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/


/** @file
 *  @brief GPIO handling for Sparkfun ProMicro.
 *
 *  API to control the 18 port lines a Sparkfun ProMicro gives us.
 *  It serves as a kind of hardware abstraction layer. Some port
 *  lines of the ATmega32U4 are not accessible by board terminals.
 *  Those port lines are brought to sane defaults and excluded
 *  from application access.
 <table>
   <tr><th><b>ATmega32U4</b></th> <th><b>ProMicro</b>  </th></tr>
   <tr><td>       PB0       </td> <td>      -n/a-      </td></tr>
   <tr><td>       PB1       </td> <td>        15       </td></tr>
   <tr><td>       PB2       </td> <td>        16       </td></tr>
   <tr><td>       PB3       </td> <td>        14       </td></tr>
   <tr><td>       PB4       </td> <td>         8       </td></tr>
   <tr><td>       PB5       </td> <td>         9       </td></tr>
   <tr><td>       PB6       </td> <td>        10       </td></tr>
   <tr><td>       PB7       </td> <td>      -n/a-      </td></tr>
   <tr><td>       PC6       </td> <td>         7       </td></tr>
   <tr><td>       PC7       </td> <td>      -n/a-      </td></tr>
   <tr><td>       PD0       </td> <td>         3       </td></tr>
   <tr><td>       PD1       </td> <td>         2       </td></tr>
   <tr><td>       PD2       </td> <td>         0       </td></tr>
   <tr><td>       PD3       </td> <td>         1       </td></tr>
   <tr><td>       PD4       </td> <td>         4       </td></tr>
   <tr><td>       PD5       </td> <td>      -n/a-      </td></tr>
   <tr><td>       PD6       </td> <td>      -n/a-      </td></tr>
   <tr><td>       PD7       </td> <td>         6       </td></tr>
   <tr><td>    PE2 / HWB    </td> <td>       GND       </td></tr>
   <tr><td>       PE6       </td> <td>         5       </td></tr>
   <tr><td>       PF0       </td> <td>      -n/a-      </td></tr>
   <tr><td>       PF1       </td> <td>      -n/a-      </td></tr>
   <tr><td>       PF4       </td> <td>        21       </td></tr>
   <tr><td>       PF5       </td> <td>        20       </td></tr>
   <tr><td>       PF6       </td> <td>        19       </td></tr>
   <tr><td>       PF7       </td> <td>        18       </td></tr>
 </table>
 */


#ifndef __GPIO_H__
#define __GPIO_H__

   /* Includes: */
   #include <avr/io.h>


   /* Enable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      extern "C" {
   #endif


   /* Private Interface - For use in library only: */
   #if !defined(__DOXYGEN__)
   /* Macros: */

      // Define bits available to GPIO.
      // Exclude bits not to touch for some reasons.

      // PORTB[7] is not available, PORTB[0] drives the RX-LED.
      #define PORTB_USAGE_MASK    0b01111110

      // PORTC[6] is the only available IO line of PORTC.
      #define PORTC_USAGE_MASK    0b01000000

      // PORTD[6] is not available, PORTD[5] drives the TX-LED.
      #define PORTD_USAGE_MASK    0b10011111

      // PORTE[6] is the only available IO line of PORTE.
      #define PORTE_USAGE_MASK    0b01000000

      // PORTF[7] to PORT PORTF[4] are available at all.
      #define PORTF_USAGE_MASK    0b11110000

   #endif


   /* Public Interface - May be used in end-application: */
   /* Inline Functions: */

   static inline void GPIO_Init(void)
   {
      DDRB  &=  ~PORTB_USAGE_MASK; // All available bits get INPUTs
      PORTB &=  ~PORTB_USAGE_MASK; // No pull-ups
      DDRC  &=  ~PORTC_USAGE_MASK; // All available bits get INPUTs
      PORTC &=  ~PORTC_USAGE_MASK; // No pull-ups
      DDRD  &=  ~PORTD_USAGE_MASK; // All available bits get INPUTs
      PORTD &=  ~PORTD_USAGE_MASK; // No pull-ups
      DDRE  &=  ~PORTE_USAGE_MASK; // All available bits get INPUTs
      PORTE &=  ~PORTE_USAGE_MASK; // No pull-ups
      DDRF  &=  ~PORTF_USAGE_MASK; // All available bits get INPUTs
      PORTF &=  ~PORTF_USAGE_MASK; // No pull-ups

      // Unusable lines set to drive GND.
      // DANGER! Refer to your board's schematic!
      PORTB &= 0b01111111; // PB7
      PORTC &= 0b01111111; // PC7
      DDRB  |= 0b10000000;
      DDRC  |= 0b10000000;
      PORTD &= 0b10111111; // PD6
      DDRD  |= 0b01000000;
      PORTF &= 0b11111100; // PF1, PF0
      DDRF  |= 0b00000011;
   }
   /**< Initializes all GPIO lines as inputs (high-impedance)
        without pull-ups. All unterminated lines float around! */


   static inline void GPIO1_ChangeLines(const uint16_t GPIObits, const uint16_t GPIOmask)
   {
      uint8_t mask = GPIOmask & PORTB_USAGE_MASK;
      uint8_t bits = GPIObits & mask;
      PORTB = (PORTB & ~mask) | bits;

      mask = (GPIOmask >> 1) & PORTE_USAGE_MASK;
      bits = (GPIObits >> 1) & mask;
      PORTE = (PORTE & ~mask) | bits;

      mask = (GPIOmask >> 8) & PORTD_USAGE_MASK;
      bits = (GPIObits >> 8) & mask;
      PORTD = (PORTD & ~mask) | bits;

      mask = (GPIOmask >> 8) & PORTC_USAGE_MASK;
      bits = (GPIObits >> 8) & mask;
      PORTC = (PORTC & ~mask) | bits;
   }
   /**< Writes data to outputs of GPIO1.
        Controls pull-up circuit on inputs.
        @c GPIObits modifies the respective port lines only when
        corresponding bit position in @c GPIOmask is set ('1').
        This allows selection of bits to change.
        @param[in] GPIObits  Bit pattern to write to the port lines.
        @param[in] GPIOmask  Mask defining the bits to effectively change.
        @note Unavailable port lines are not touched. */


   static inline void GPIO1_ChangeDirections(const uint16_t GPIOdirs, const uint16_t GPIOmask)
   {
      uint8_t mask = GPIOmask & PORTB_USAGE_MASK;
      uint8_t dirs = GPIOdirs & mask;
      DDRB = (DDRB & ~mask) | dirs;

      mask = (GPIOmask >> 1) & PORTE_USAGE_MASK;
      dirs = (GPIOdirs >> 1) & mask;
      DDRE = (DDRE & ~mask) | dirs;

      mask = (GPIOmask >> 8) & PORTD_USAGE_MASK;
      dirs = (GPIOdirs >> 8) & mask;
      DDRD = (DDRD & ~mask) | dirs;

      mask = (GPIOmask >> 8) & PORTC_USAGE_MASK;
      dirs = (GPIOdirs >> 8) & mask;
      DDRC = (DDRC & ~mask) | dirs;
   }
   /**< Controls port line directions of GPIO1.
        @c GPIOdirs modifies the respective direction only when
        corresponding bit position in @c GPIOmask is set ('1').
        This allows selection of bits to change.
        @param[in] GPIObits  Bit pattern defining the direction.
                             Line gets output when bit is set.
        @param[in] GPIOmask  Mask defining the bits to effectively change.
        @note Unavailable port lines are not touched. */


   static inline uint16_t GPIO1_GetLines(void) ATTR_WARN_UNUSED_RESULT;
   static inline uint16_t GPIO1_GetLines(void)
   {
      uint8_t lo = (PINB & PORTB_USAGE_MASK) | ((PINE & PORTE_USAGE_MASK) << 1);
      uint8_t hi = (PIND & PORTD_USAGE_MASK) | (PINC & PORTC_USAGE_MASK);
      return ((hi << 8) | lo);
   }
   /**< Returns the logic level of all port lines (inputs as well
        as outputs) of GPIO1.
        @return  Bitpattern read from the GPIO1 port lines.
                 Unavailable port lines always return '0'. */


   static inline uint16_t GPIO1_GetDirections(void) ATTR_WARN_UNUSED_RESULT;
   static inline uint16_t GPIO1_GetDirections(void)
   {
      uint8_t lo = (DDRB & PORTB_USAGE_MASK) | ((DDRE & PORTE_USAGE_MASK) << 1);
      uint8_t hi = (DDRD & PORTD_USAGE_MASK) | (DDRC & PORTC_USAGE_MASK);
      return ((hi << 8) | lo);
   }
   /**< Returns the direction of all port lines of GPIO1.
        @return  Bitpattern from the GPIO1 direction control.
                 Unavailable port lines always return '0'. */

   
   static inline void GPIO2_ChangeLines(const uint16_t GPIObits, const uint16_t GPIOmask)
   {
      uint8_t mask = GPIOmask & PORTF_USAGE_MASK;
      uint8_t bits = GPIObits & mask;
      PORTF = (PORTF & ~mask) | bits;
   }
   /**< Writes data to outputs of GPIO2.
        Controls pull-up circuit on inputs.
        @c GPIObits modifies the respective port lines only when
        corresponding bit position in @c GPIOmask is set ('1').
        This allows selection of bits to change.
        @param[in] GPIObits  Bit pattern to write to the port lines.
        @param[in] GPIOmask  Mask defining the bits to effectively change.
        @note Unavailable port lines are not touched. */


   static inline void GPIO2_ChangeDirections(const uint16_t GPIOdirs, const uint16_t GPIOmask)
   {
      uint8_t mask = GPIOmask & PORTF_USAGE_MASK;
      uint8_t dirs = GPIOdirs & mask;
      DDRF = (DDRF & ~mask) | dirs;
   }
   /**< Controls port line directions of GPIO2.
        @c GPIOdirs modifies the respective direction only when
        corresponding bit position in @c GPIOmask is set ('1').
        This allows selection of bits to change.
        @param[in] GPIObits  Bit pattern defining the direction.
                             Line gets output when bit is set.
        @param[in] GPIOmask  Mask defining the bits to effectively change.
        @note Unavailable port lines are not touched. */


   static inline uint16_t GPIO2_GetLines(void) ATTR_WARN_UNUSED_RESULT;
   static inline uint16_t GPIO2_GetLines(void)
   {
      return (PINF & PORTF_USAGE_MASK);
   }
   /**< Returns the logic level of all port lines (inputs as well
        as outputs) of GPIO2.
        @return  Bitpattern read from the GPIO2 port lines.
                 Unavailable port lines always return '0'. */


   static inline uint16_t GPIO2_GetDirections(void) ATTR_WARN_UNUSED_RESULT;
   static inline uint16_t GPIO2_GetDirections(void)
   {
      return (DDRF & PORTF_USAGE_MASK);
   }
   /**< Returns the direction of all port lines of GPIO2.
        @return  Bitpattern from the GPIO2 direction control.
                 Unavailable port lines always return '0'. */


   /* Disable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      }
   #endif

#endif

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
 *  \~English
 *   @brief LED definitions for Sparkfun ProMicro.
 *
 *  API to control the 2 LEDs we have on a Sparkfun ProMicro board.
 *  Those are taken away from the USB-logic and made available to
 *  the user. Only the bootloader takes over when activated.
 *  The routines serve as a kind of hardware abstraction layer.
 *
 *  \~German
 *   @brief LED-Kontrolle für Sparkfun ProMicro.
 *
 *  API zur Kontrolle der 2 LEDs auf dem Sparkfun ProMicro.
 *  Der Benutzer bekommt die exklusive Kontrolle über diese beiden
 *  LEDs, die USB-Logik hat damit nichts mehr zu tun. Falls der
 *  Bootlader aktiv ist, übernimmt der die LEDs.
 *  Durch diese Routinen wird der Kern des USB-Userport ein wenig
 *  unabhängiger von der unterlagerten Hardware.
 *
 <table>
   <tr><th><b>ATmega32U4</b></th> <th><b>ProMicro</b>  </th></tr>
   <tr><td>       PB0       </td> <td>     RX-LED      </td></tr>
   <tr><td>       PD5       </td> <td>     TX-LED      </td></tr>
 </table>
 */


#ifndef __LEDS_USER_H__
#define __LEDS_USER_H__


   /* Includes: */
   #include <avr/io.h>


   /* Enable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      extern "C" {
   #endif // #if defined(__cplusplus)

// /* Preprocessor Checks: */
// #if !defined(__INCLUDE_FROM_LEDS_H)
//    #error Do not include this file directly. Include LUFA/Drivers/Board/LEDS.h instead.
// #endif // #if !defined(__INCLUDE_FROM_LEDS_H)
// Decoupled from LUFA definitions by intention.


   /* Public Interface - May be used in end-application: */
   /* Macros: */

   #define LEDS_LED1        (1 << 0)  // PB0
   /**<
    * \~English Bit mask for the first LED on the board.
    * \~German  Bitmaske für die erste LED auf der Platine.
    */


   #define LEDS_LED2        (1 << 5)  // PD5
   /**<
    * \~English Bit mask for the second LED on the board.
    * \~German  Bitmaske für die zweite LED auf der Platine.
    */


   #define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2)
   /**<
    * \~English Bit mask for all LEDs on the board.
    * \~German  Bitmaske für alle LEDs auf der Platine.
    */


   #define LEDS_NO_LEDS     0
   /**<
    * \~English Bit mask for no LEDs.
    * \~German  Bitmaske für keine LEDs.
    */


   #define RXLED            LEDS_LED1
   /**<
    * \~English Defines more convenient LED name based on schematic.
    * \~German  Ordnet den Namen der LED gemäß Schaltplan zu.
    */


   #define TXLED            LEDS_LED2
   /**<
    * \~English Defines more convenient LED name based on schematic.
    * \~German  Ordnet den Namen der LED gemäß Schaltplan zu.
    */


   /* Inline Functions: */

   static inline void LEDs_Init(void)
   {
      DDRB  |=  LEDS_LED1;
      PORTB |=  LEDS_LED1;
      DDRD  |=  LEDS_LED2;
      PORTD |=  LEDS_LED2;
   }
   /**<
    * \~English
    *  Initializes LED port lines as outputs, turns off LEDs (unlit).
    * \~German
    *  Initialisiert die IO-Leitungen der LEDs als Ausgänge, schaltet
    *  alle LEDs aus.
    */
   /**<  */


   static inline void LEDs_Disable(void)
   {
      DDRB  &= ~LEDS_LED1;
      PORTB &= ~LEDS_LED1;
      DDRD  &= ~LEDS_LED2;
      PORTD &= ~LEDS_LED2;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Retires from LED control. All port lines get inputs, no
        pull-ups to the lines. */


   static inline void LEDs_TurnLED1On(void)
   {
      PORTB &= ~LEDS_LED1;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Turns LED 1 on (lit). */
   // Note: LEDs are lit when the port drives out a logical '0'.
   // The logic thus is inverted.


   static inline void LEDs_TurnLED1Off(void)
   {
      PORTB |= LEDS_LED1;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Turns LED 1 off (unlit). */


   static inline void LEDs_TurnLED2On(void)
   {
      PORTD &= ~LEDS_LED2;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Turns LED 2 on (lit). */


   static inline void LEDs_TurnLED2Off(void)
   {
      PORTD |= LEDS_LED2;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Turns LED 1 off (unlit). */


   static inline void LEDs_TurnRxLEDOn(void)
   {
      PORTB &= ~RXLED;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Convenience macro, turns RX-LED on (lit). */


   static inline void LEDs_TurnRxLEDOff(void)
   {
      PORTB |= RXLED;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Convenience macro, turns RX-LED off (unlit). */


   static inline void LEDs_TurnTxLEDOn(void)
   {
      PORTD &= ~TXLED;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Convenience macro, turns TX-LED on (lit). */


   static inline void LEDs_TurnTxLEDOff(void)
   {
      PORTD |= TXLED;
   }
   /**<
    * \~English 
    * \~German  
    */
   /**< Convenience macro, turns TX-LED off (unlit). */


   static inline void LEDs_TurnOn(const uint8_t LEDMask)
   {
      PORTB &= ~(LEDMask & LEDS_LED1);
      PORTD &= ~(LEDMask & LEDS_LED2);
   }
   /**< Turns selected LEDs on (lit).
        @param[in] LEDMask defines which LEDs to turn on.
                   1 bit per LED: '1' = lit, '0' = leave as is.
                   Undefined bit positions are ignored. */


   static inline void LEDs_TurnOff(const uint8_t LEDMask)
   {
      PORTB |=  (LEDMask & LEDS_LED1);
      PORTD |=  (LEDMask & LEDS_LED2);
   }
   /**< Turns selected LEDs off (unlit).
        @param[in] LEDMask defines which LEDs to turn off.
                   1 bit per LED: '1' = unlit, '0' = leave as is.
                   Undefined bit positions are ignored. */


   static inline void LEDs_SetAll(const uint8_t LEDMask)
   {
      PORTB = ((PORTB | LEDS_LED1) & ~(LEDMask & LEDS_LED1));
      PORTD = ((PORTD | LEDS_LED2) & ~(LEDMask & LEDS_LED2));
   }
   /**< Sets all LEDs.
        @param[in] LEDMask defines each LED state.
                   1 bit per LED: '1' = lit, '0' = unlit.
                   Undefined bit positions are ignored. */


   static inline void LEDs_Change(const uint8_t LEDMask, const uint8_t ActiveMask)
   {
      PORTB = ((PORTB | (LEDMask & LEDS_LED1)) & ~(ActiveMask & LEDS_LED1));
      PORTD = ((PORTD | (LEDMask & LEDS_LED2)) & ~(ActiveMask & LEDS_LED2));
   }
   /**< Sets all selected LEDs.
        @param[in] LEDMask defines each LED state.
                   1 bit per LED: '1' = lit, '0' = unlit.
        @param[in] ActiveMask defines which LEDs to modify.
                   1 bit per LED: '1' = change, '0' = leave as is.
                   
                   Undefined bit positions are ignored. */


   static inline void LEDs_Toggle(const uint8_t LEDMask)
   {
      PORTB ^= (LEDMask & LEDS_LED1);
      PORTD ^= (LEDMask & LEDS_LED2);
   }
   /**< Toggles the logic states of selected LEDs.
        @param[in] LEDMask defines which LEDs to toggle.
                   1 bit per LED: '1' = toggle, '0' = leave as is.
                   Undefined bit positions are ignored. */


   static inline uint8_t LEDs_GetAll(void) ATTR_WARN_UNUSED_RESULT;
   static inline uint8_t LEDs_GetAll(void)
   {
      return (((PORTB & LEDS_LED1) | (PORTD & LEDS_LED2)) ^ LEDS_ALL_LEDS);
   }
   /**< Returns the logic state of all on-board LEDs.
        @return  LED states. '1' = lit, '0' = unlit.
                 Undefined bit positions always return '0'. */


   /* Disable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      }
   #endif // #if defined(__cplusplus)

#endif // #ifndef __LEDS_USER_H__

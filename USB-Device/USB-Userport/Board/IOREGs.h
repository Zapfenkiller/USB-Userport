/*
  * The USB-Userport *
  Copyright 2021  René Trapp (rene [dot] trapp (-at-) web [dot] de)
  License: "The MIT license" - see license.txt
*/


/** @file
 *  \~English
 *   @brief IO register access definitions for Sparkfun ProMicro.
 *
 *  API to control some (but not all) IO registers the Sparkfun
 *  ProMicro board provides.
 *  The routines serve as a kind of hardware abstraction layer.
 *  Reserved registers are excluded from writing, reserved bits
 *  always get written as'0'.
 *
 *  \~German
 *   @brief IO Registerzugriff für Sparkfun ProMicro.
 *
 *  API zur Kontrolle einiger (aber nicht aller) IO Register
 *  die ein Sparkfun ProMicro so hat.
 *  Durch diese Routinen wird der Kern des USB-Userport ein wenig
 *  unabhängiger von der unterlagerten Hardware.
 *  Reservierte Register werden nicht beschrieben, reservierte Bits
 *  werden immer zu '0' geschrieben.
 */


#ifndef __IOREGS_USER_H__
#define __IOREGS_USER_H__


   /* Includes: */
   #include <avr/io.h>


   /* Enable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      extern "C" {
   #endif // #if defined(__cplusplus)


   /* Public Interface - May be used in end-application: */

   uint16_t memAddress;


   /* Macros: */


   /* Inline Functions: */

   static inline void io_write(uint8_t *address, uint8_t data)
   {
      if ((address >= (uint8_t *)0x23) && (address <= (uint8_t *)0xD4))
         *address = data;
   }
   /**<
    * \~English
    *  writes a byte into a certain memory location.
    *  @param[in] address  selects the memory location to write to.
    *             The IO address range is memory mapped according
    *             to the ATmega32U4 datasheet. E.g. PINB = 0x23
    *  @param[in] data  is the byte to write. 
    *  @note Writing to a wrong location might crash the application!
    * \~German
    *  schreibt ein Byte auf eine Speicheradresse.
    *  @param[in] address  wählt die Speicherzelle aus. Die IO-Register
    *             sind Memory-Mapped gemäß dem Datenblatt des
    *             ATmega32U4. Z. B. PINB = 0x23
    *  @param[in] data  wird an diese Adresse geschrieben.
    *  @note Schreibzugriffe auf eine falsche Adresse können die
    *  Applikation abstürzen lassen!
    */


   static inline uint8_t io_read(uint8_t *address)
   {
      if ((address >= (uint8_t *)0x23) && (address <= (uint8_t *)0xD4))
         return(*address);
      else
         return(0);
   }
   /**<
    * \~English
    *  reads a byte from a certain memory location.
    *  @param[in] address  selects the memory location to write to.
    *             The IO address range is memory mapped according
    *             to the ATmega32U4 datasheet. E.g. PINB = 0x23
    *  @return    Byte read.
    *  @note Some read accesses might clear flags and thus crash
    *  the application.
    * \~German
    *  liest ein Byte von einer Speicheradresse.
    *  @param[in] address  wählt die Speicherzelle aus. Die IO-Register
    *             sind Memory-Mapped gemäß dem Datenblatt des
    *             ATmega32U4. Z. B. PINB = 0x23
    *  @return    Das Byte.
    *  @note Einige Lesezugriffe löschen Flags die die Applikation
    *  zu ihrer Funktion benötigt.
    */


   /* Disable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      }
   #endif // #if defined(__cplusplus)

#endif // #ifndef __IOREGS_USER_H__

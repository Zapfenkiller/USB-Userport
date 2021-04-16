/*
  * The USB-Userport *
  Copyright 2020  René Trapp (rene [dot] trapp (-at-) web [dot] de)
  License: "The MIT license" - see license.txt
*/


/** @file
 *  \~English
 *   @brief ADC definitions for Sparkfun ProMicro.
 *
 *  API to control some (but not all) of the analog inputs a
 *  Sparkfun ProMicro board provides. 4 GPIO lines get the ability
 *  to read analog voltages.
 *  The routines serve as a kind of hardware abstraction layer.
 *
 *  \~German
 *   @brief ADC-Kontrolle für Sparkfun ProMicro.
 *
 *  API zur Kontrolle einiger (aber nicht aller) analogen Eingänge
 *  die ein Sparkfun ProMicro so hat. Bei 4 GPIO-Leitungen kann
 *  die analoge Spannung abgelesen werden.
 *  Durch diese Routinen wird der Kern des USB-Userport ein wenig
 *  unabhängiger von der unterlagerten Hardware.
 *
 *  \~
 <table>
   <tr><th><b>ATmega32U4</b></th> <th><b>ProMicro</b>  </th></tr>
   <tr><td>       PF4       </td> <td>        21       </td></tr>
   <tr><td>       PF5       </td> <td>        20       </td></tr>
   <tr><td>       PF6       </td> <td>        19       </td></tr>
   <tr><td>       PF7       </td> <td>        18       </td></tr>
 </table>
 */


#ifndef __ADC_USER_H__
#define __ADC_USER_H__


   /* Includes: */
   #include <avr/io.h>


   /* Enable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      extern "C" {
   #endif // #if defined(__cplusplus)


   /* Public Interface - May be used in end-application: */
   /* Macros: */


   #define ADC_Chan_4       4         // ADC4
   /**<
    * \~English defines the ADMUX setting for ADC4.
    * \~German  definiert die ADC4-Kanaleinstellung für ADMUX.
    */


   #define ADC_Chan_5       5         // ADC5
   /**<
    * \~English defines the ADMUX setting for ADC5.
    * \~German  definiert die ADC5-Kanaleinstellung für ADMUX.
    */


   #define ADC_Chan_6       6         // ADC6
   /**<
    * \~English defines the ADMUX setting for ADC6.
    * \~German  definiert die ADC6-Kanaleinstellung für ADMUX.
    */


   #define ADC_Chan_7       7         // ADC7
   /**<
    * \~English defines the ADMUX setting for ADC7.
    * \~German  definiert die ADC7-Kanaleinstellung für ADMUX.
    */


   /* Inline Functions: */

   static inline void ADC_Configure(uint8_t Config, uint8_t Control)
   {
      DIDR0 = Config & 0xF0;
      uint8_t RefSel = (1 << REFS0) | (1 << ADLAR) | (0b00000 << MUX0);
      if (Control & 0x02)
         RefSel |= (1 << REFS1);
      ADMUX = RefSel;
      ADCSRB = 0;
      if (Control & 0x80)
         ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | (0b111 << ADPS0);
      else
         ADCSRA = (1 << ADIF) | (0b111 << ADPS0);
   }
   /**<
    * \~English
    *  turns ADC on or off, sets reference voltage and digital
    *  input disable register.
    *  @param[in] Config  disables the digital input circuitry
    *             when bit is set ('1'). Bits 4 to 7 correspond to
    *             analog inputs ADC4 to ADC7.
    *  @param[in] Control  selects reference voltage. Bit 1 = '1'
    *             selects the internal 2.56 V, bit 1 = '0' sets
    *             ratiometric AVcc. Bit 7 = '1' turns ADC on when
    *             set ('1').
    *  @note Undefined bit positions are ignored.
    * \~German
    *  schaltet den ADC an oder aus, wählt die Referenzspannung
    *  und kontrolliert die digitalen Eingangsschaltungen.
    *  @param[in] Config  schaltet die digitalen Eingangsstufen ab
    *             wenn das zugehöroge Bit gesetzt ist ('1'). Bit 4
    *             bis 7 gehören zu den analogen Eingängen ADC4 bis
    *             ADC7.
    *  @param[in] Control  wählt die Referenzquelle aus. Bit 1 =
    *             '1' schaltet die internen 2.56 V ein, Bit 1 = '0'
    *             schaltet die ratiometrische AVcc ein. Ein
    *             gesetztes ('1') Bit 7 schaltet den ADC ein.
    *  @note Nicht verwendete Bitpositionen werden ignoriert.
    */


   static inline void ADC_Get_Sample(uint8_t Channel, uint8_t* Sample)
   {
      ADMUX = (ADMUX & 0b11100000) | (Channel & 0b00011111);
      ADCSRA |= (1 << ADSC);
      while (ADCSRA & (1 << ADSC)) {}
      Sample[0] = ADCL;
      Sample[1] = ADCH;
   }
   /**<
    * \~English
    *  switches ADC to desired channel, starts a conversion
    *  (single shot), waits until done and stores the result to
    *  given location.
    *  @param[in] Channel  selects the desired channel to get the
    *             analog result from.
    *  @param[in] Sample  points to 2 consecutive RAM bytes to
    *             store the analog reading.
    *  @param[out] Sample  2 bytes in little endian format at
    *              location `Sample` points to.
    * \~German
    *  schaltet den ADC zum gewünschten analogen Eingang, startet
    *  eine einzelne Wandlung, wartet bis diese beendet ist und
    *  speichert das Ergebnis im referenzierten Speicherbereich.
    *  @param[in] Channel  wählt den analogen Kanals aus.
    *  @param[in] Sample  zeigt auf zwei zusammenhängende Bytes im
    *             RAM in denen das Wandlungsergebnis abzulegen ist.
    *  @param[out] Sample  2 bytes im Intel-Format ab der Adresse
    *              auf die `Sample` zeigt.
    */


   /* Disable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      }
   #endif // #if defined(__cplusplus)

#endif // #ifndef __LEDS_USER_H__

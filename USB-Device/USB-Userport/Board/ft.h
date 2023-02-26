/*
  * The USB-Userport *
  Copyright 2023  René Trapp (rene [dot] trapp (-at-) web [dot] de)
  License: "The MIT license" - see license.txt
*/


/** @file
 *  \~English
 *   @brief A Sparkfun ProMicro interfaces to a fischertechnik 66843.
 *
 *   API to exploit the USB-Userport as interface to an ancient
 *   fischertechnik CVK Computing Interface, item number 66843.
 *   Several GPIOs and the SPI module as well as Timer 1 of the
 *   ATmega32U4 are used for this purpose. A chain of one master and
 *   one slave interface is also supported.
 *
 *  \~German
 *   @brief Ein Sparkfun ProMicro "spricht" mit einem fischertechnik
 *   66843.
 *
 *   API um den USB-Userport als Schnittstelle zwischen USB und einem
 *   antiken fischertechnik CVK Computing Interface, Teilenummer 66843,
 *   zu verwenden.
 *   Für den Zweck werden einige GPIOs, das SPI-Modul und Timer 1 des
 *   ATmega32U4 benutzt.
 *   Eine Kette aus einem Master- und einem Slave-Interface wird auch
 *   unterstützt.
 *
 *  \~
 <table>
   <tr><th><b>ATmega32U4</b></th> <th><b>ProMicro</b>  </th> <th><b> 66843 </b>   </th></tr>
   <tr><td>       PB0       </td> <td>      -n/a-      </td> <td>      ---        </td></tr>
   <tr><td>       PB1       </td> <td>        15       </td> <td>      CLK        </td></tr>
   <tr><td>       PB2       </td> <td>        16       </td> <td>    DATA_OUT     </td></tr>
   <tr><td>       PB3       </td> <td>        14       </td> <td>   C64_DATA_IN   </td></tr>
   <tr><td>       PB4       </td> <td>         8       </td> <td>     LOAD_IN     </td></tr>
   <tr><td>       PB5       </td> <td>         9       </td> <td>                 </td></tr>
   <tr><td>       PB6       </td> <td>        10       </td> <td>                 </td></tr>
   <tr><td>       PB7       </td> <td>      -n/a-      </td> <td>      ---        </td></tr>
   <tr><td>       PC6       </td> <td>         5       </td> <td>   MMV_PULSES    </td></tr>
   <tr><td>       PC7       </td> <td>      -n/a-      </td> <td>      ---        </td></tr>
   <tr><td>       PD0       </td> <td>         3       </td> <td>                 </td></tr>
   <tr><td>       PD1       </td> <td>         2       </td> <td>                 </td></tr>
   <tr><td>       PD2       </td> <td>         0       </td> <td>                 </td></tr>
   <tr><td>       PD3       </td> <td>         1       </td> <td>                 </td></tr>
   <tr><td>       PD4       </td> <td>         4       </td> <td>                 </td></tr>
   <tr><td>       PD5       </td> <td>      -n/a-      </td> <td>      ---        </td></tr>
   <tr><td>       PD6       </td> <td>      -n/a-      </td> <td>      ---        </td></tr>
   <tr><td>       PD7       </td> <td>         6       </td> <td>    COUNT_IN     </td></tr>
   <tr><td>    PE2 / HWB    </td> <td>       GND       </td> <td>      ---        </td></tr>
   <tr><td>       PE6       </td> <td>         7       </td> <td>    LOAD_OUT     </td></tr>
   <tr><td>       PF0       </td> <td>      -n/a-      </td> <td>      ---        </td></tr>
   <tr><td>       PF1       </td> <td>      -n/a-      </td> <td>      ---        </td></tr>
   <tr><td>       PF4       </td> <td>        21       </td> <td>                 </td></tr>
   <tr><td>       PF5       </td> <td>        20       </td> <td>                 </td></tr>
   <tr><td>       PF6       </td> <td>        19       </td> <td>    TRIGGER_X    </td></tr>
   <tr><td>       PF7       </td> <td>        18       </td> <td>    TRIGGER_Y    </td></tr>

 </table>
 */


#ifndef __FT_H__
#define __FT_H__

   /* Includes: */
   #include <avr/io.h>


   /* Enable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      extern "C" {
   #endif


   #define ft66843_CON_TIMEOUT_s    0.25  // host connection timeout
   #define ft66843_REPETITION_s     0.005 // reasonable range: 1 ms to 10 ms

   #define ft66843_OUT_CHG          (1 << 0)
   #define ft66843_INP_CHG          (1 << 1)
   #define ft66843_EX_CHG           (1 << 2)
   #define ft66843_EY_CHG           (1 << 3)


/* -------------------------------------------------------------------------- */
/* Interface to application                                                   */
/* -------------------------------------------------------------------------- */


   void ft66843_OutputsSetMaster(uint8_t vector);
   void ft66843_OutputsSetSlave(uint8_t vector);
   uint8_t ft66843_OutputsGetMaster(void);
   uint8_t ft66843_OutputsGetSlave(void);
   uint8_t ft66843_InputsGetMaster(void);
   uint8_t ft66843_InputsGetSlave(void);
   void ft66843_StopEnableSet(uint8_t master, uint8_t slave);
   void ft66843_StopInversionSet(uint8_t master, uint8_t slave);
   void ft66843_StopResponseFwdSet(uint8_t master, uint8_t slave);
   void ft66843_StopResponseBwdSet(uint8_t master, uint8_t slave);
   uint8_t ft66843_PotXGet(void);
   uint8_t ft66843_PotYGet(void);
   uint8_t ft66843_HasChanges(void);


/* -------------------------------------------------------------------------- */
/* Entry and Exit                                                             */
/* -------------------------------------------------------------------------- */


   void ft66843_Init(void);
   void ft66843_Leave(void);
   uint8_t ft66843_IsEnabled(void);


/* -------------------------------------------------------------------------- */
/* Transfer and stop switch logic                                             */
/* -------------------------------------------------------------------------- */


   void ft66843_CoreTask(void);


   /* Disable C linkage for C++ Compilers: */
   #if defined(__cplusplus)
      }
   #endif

#endif

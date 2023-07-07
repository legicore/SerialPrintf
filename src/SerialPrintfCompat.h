////////////////////////////////////////////////////////////////////////////////
/**
 * @file        SerialPrintfCompat.h
 * 
 * @author      Martin Legleiter
 * 
 * @brief       TODO
 * 
 * @copyright   (c) 2023 Martin Legleiter
 * 
 * @license     Use of this source code is governed by an MIT-style
 *              license that can be found in the LICENSE file or at
 *              @see https://opensource.org/licenses/MIT.
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __SERIAL_PRINTF_COMPAT_H__
#define __SERIAL_PRINTF_COMPAT_H__

/*-----------------------------------------------------------*/

#include <Arduino.h>

/*-----------------------------------------------------------*/

/* This value for the Arduino default serial configuration is taken from the
source code (8 data bits, no parity, 1 stop bit). */
#define SERIAL_CONFIG_DEFAULT   SERIAL_8N1

/* Check the architecture, based on the architecture of the tested boards. */
#if defined( ARDUINO_ARCH_AVR ) || \
    defined( ARDUINO_ARCH_MEGAAVR ) || \
    defined( ARDUINO_ARCH_SAMD ) || \
    defined( ARDUINO_ARCH_RENESAS )

    /* Check for the known boards that are not (!) compatible with the Arduino
    SoftwareSerial library, and if none of them is selected, the compatibility
    gets activated. */
    #if !defined( ARDUINO_SAMD_MKRZERO ) && \
        !defined( ARDUINO_SAMD_NANO_33_IOT )

        #define COMPAT_SOFTWARE_SERIAL

    #endif

    /* Check for the known/tested boards and set the Serial_t type to the
    associated Arduino serial type. */
    #if defined( ARDUINO_AVR_UNO ) || \
        defined( ARDUINO_AVR_MEGA2560 ) || \
        defined( ARDUINO_AVR_PRO ) || \
        defined( ARDUINO_SAMD_MKRZERO )

        #define Serial_t    HardwareSerial

    #elif defined( ARDUINO_AVR_LEONARDO )

        #define Serial_t    Serial_
        #define HWSerial_t  HardwareSerial

    #elif defined( ARDUINO_AVR_NANO_EVERY ) || \
          defined( ARDUINO_AVR_UNO_WIFI_REV2 )

        #define Serial_t    UartClass

    #elif defined( ARDUINO_SAMD_NANO_33_IOT )

        #define Serial_t    Serial_
        #define HWSerial_t  Uart

    #elif defined( ARDUINO_MINIMA ) || \
          defined( ARDUINO_UNOWIFIR4 )

        #define Serial_t                _SerialUSB
        #define HWSerial_t              UART
        #define SERIAL_PORT_MONITOR     Serial
        #define SERIAL_PORT_HARDWARE    Serial1

    #else

        /* The selected board has a known architecture, but has not yet been
        tested. So the assumed default values for the Arduino serial port will
        be set, and the SoftwareSerial compatibility gets deactivated. */

        #if defined( COMPAT_SOFTWARE_SERIAL )
            #undef COMPAT_SOFTWARE_SERIAL
        #endif

        #warning The selected board has not been tested with this version of the library.

    #endif

#else

    /* The selected board and its architecture have not yet been tested. So the
    assumed default values for the Arduino serial port will be set. */

    #warning The selected board and architecture have not been tested with this version of the library.

#endif

#if !defined( Serial_t )
    #define Serial_t                HardwareSerial
#endif
#if !defined( SERIAL_PORT_MONITOR )
    #define SERIAL_PORT_MONITOR     Serial
#endif

/*-----------------------------------------------------------*/

#endif /* __SERIAL_PRINTF_COMPAT_H__ */
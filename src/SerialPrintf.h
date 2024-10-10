////////////////////////////////////////////////////////////////////////////////
/**
 * @file        SerialPrintf.h
 *
 * @author      Martin Legleiter
 *
 * @brief       TODO
 * 
 * @copyright   2024 Martin Legleiter
 * 
 * @license     Use of this source code is governed by an MIT-style
 *              license that can be found in the LICENSE file or at
 *              @see https://opensource.org/licenses/MIT.
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __SERIAL_PRINTF_H__
#define __SERIAL_PRINTF_H__

/*--------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "SerialPrintfCompat.h"

#include <Arduino.h>
#if defined( COMPAT_SOFTWARE_SERIAL )
    #include <SoftwareSerial.h>
#endif

/*--------------------------------------------------*/

#define configBUF_SIZE_DEFAULT  64

/*--------------------------------------------------*/

#define _d_1f( d, f ) "%"#d"ld.%ld", ( long ) f, abs( ( long ) ( f * 10 ) % 10 )
#define _d_2f( d, f ) "%"#d"ld.%ld", ( long ) f, abs( ( long ) ( f * 100 ) % 100 )
#define _d_3f( d, f ) "%"#d"ld.%ld", ( long ) f, abs( ( long ) ( f * 1000 ) % 1000 )
#define _d_4f( d, f ) "%"#d"ld.%ld", ( long ) f, abs( ( long ) ( f * 10000 ) % 10000 )
#define _d_5f( d, f ) "%"#d"ld.%ld", ( long ) f, abs( ( long ) ( f * 100000 ) % 100000 )
#define _d_6f( d, f ) "%"#d"ld.%ld", ( long ) f, abs( ( long ) ( f * 1000000 ) % 1000000 )

/*--------------------------------------------------*/

class SerialPrintf
{
public:

    SerialPrintf();
    ~SerialPrintf();
    int begin( MSerial_t * serial, int bufferSize = configBUF_SIZE_DEFAULT );
#if defined( HSerial_t )
    int begin( HSerial_t * serial, int bufferSize = configBUF_SIZE_DEFAULT );
#endif
#if defined( COMPAT_SOFTWARE_SERIAL )
    int begin( SoftwareSerial * serial, int bufferSize = configBUF_SIZE_DEFAULT );
#endif
    void end( void );
    int printf( const char * fmt, ... );
    void setMemMang( void * (*pvMalloc)( size_t ), void (*vFree)( void * ) );

private:

    char * pcBuffer;
    size_t xBufferSize;
    MSerial_t * pxMSerial;
#if defined( HSerial_t )
    HSerial_t * pxHSerial;
#endif
#if defined( COMPAT_SOFTWARE_SERIAL )
    SoftwareSerial * pxSSerial;
#endif
    bool bInitLock;
    void * (*pvMalloc)( size_t xSize );
    void (*vFree)( void * pvPtr );
    int8_t cSetBuffer( int16_t sBufferSize );
};
/*--------------------------------------------------*/

#endif /* __SERIAL_PRINTF_H__ */

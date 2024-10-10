////////////////////////////////////////////////////////////////////////////////
/**
 * @file        SerialPrintf.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "SerialPrintfCompat.h"
#include "SerialPrintf.h"

#include <Arduino.h>
#if defined( SSerial_t )
    #include <SoftwareSerial.h>
#endif

/*-----------------------------------------------------------*/

SerialPrintf::SerialPrintf()
{
    pcBuffer = NULL;
    xBufferSize = 0;

    pxMSerial = NULL;
#if defined( HSerial_t )
    pxHSerial = NULL;
#endif
#if defined( SSerial_t )
    pxSSerial = NULL;
#endif

    bInitLock = false;

    pvMalloc = malloc;
    vFree = free;
}
/*-----------------------------------------------------------*/

SerialPrintf::~SerialPrintf()
{
    vFree( pcBuffer );
}
/*-----------------------------------------------------------*/

int SerialPrintf::begin( MSerial_t * serial, int bufferSize )
{
    if( ( serial != NULL ) && ( bufferSize > 0 ) && ( bInitLock == false ) )
    {
        if( cSetBuffer( bufferSize ) == 0 )
        {
            pxMSerial = serial;
            bInitLock = true;

            return 0;
        }
    }
    
    return -1;
}
/*-----------------------------------------------------------*/

#if defined( HSerial_t )

    int SerialPrintf::begin( HSerial_t * serial, int bufferSize )
    {
        if( ( serial != NULL ) && ( bufferSize > 0 ) && ( bInitLock == false ) )
        {
            if( cSetBuffer( bufferSize ) == 0 )
            {
                pxHSerial = serial;
                bInitLock = true;

                return 0;
            }
        }
        
        return -1;
    }

#endif
/*-----------------------------------------------------------*/

#if defined( SSerial_t )

    int SerialPrintf::begin( SSerial_t * serial, int bufferSize )
    {
        if( ( serial != NULL ) && ( bufferSize > 0 ) && ( bInitLock == false ) )
        {
            if( cSetBuffer( bufferSize ) == 0 )
            {
                pxSSerial = serial;
                bInitLock = true;

                return 0;
            }
        }
        
        return -1;
    }

#endif
/*-----------------------------------------------------------*/

void SerialPrintf::end( void )
{
    pxMSerial = NULL;
#if defined( HSerial_t )
    pxHSerial = NULL;
#endif
#if defined( SSerial_t )
    pxSSerial = NULL;
#endif

    vFree( pcBuffer );
    pcBuffer = NULL;
    xBufferSize = 0;

    bInitLock = false;
}
/*-----------------------------------------------------------*/

int8_t SerialPrintf::cSetBuffer( int16_t sBufferSize )
{
    if( sBufferSize > 0 )
    {
        pcBuffer = ( char * ) pvMalloc( sizeof( char ) * ( size_t ) sBufferSize );
        if( pcBuffer != NULL )
        {
            xBufferSize = ( size_t ) sBufferSize;

            return 0;
        }
    }

    return -1;
}
/*-----------------------------------------------------------*/

int SerialPrintf::printf( const char * fmt, ... )
{
    int sStringLen = -1;
    va_list xArgs;

    if( fmt != NULL )
    {
        va_start( xArgs, fmt );

        sStringLen = vsnprintf( pcBuffer, xBufferSize, fmt, xArgs );
        if( ( sStringLen > 0 ) && ( sStringLen < ( int ) xBufferSize ) )
        {
            if( pxMSerial != NULL )
            {
                pxMSerial->print( pcBuffer );
                pxMSerial->flush();
            }
#if defined( HSerial_t )
            else if( pxHSerial != NULL )
            {
                pxHSerial->print( pcBuffer );
                pxHSerial->flush();
            }
#endif
#if defined( SSerial_t )
            else if( pxSSerial != NULL )
            {
                pxSSerial->print( pcBuffer );
                pxSSerial->flush();
            }
#endif
            else
            {
                sStringLen = -1;
            }
        }

        va_end( xArgs );
    }

    return sStringLen;
}
/*-----------------------------------------------------------*/

void SerialPrintf::setMemMang( void * (*pvMalloc)( size_t ), void (*vFree)( void * ) )
{
    if( ( pvMalloc != NULL ) && ( vFree != NULL ) )
    {
        this->pvMalloc = pvMalloc;
        this->vFree = vFree;
    }
}

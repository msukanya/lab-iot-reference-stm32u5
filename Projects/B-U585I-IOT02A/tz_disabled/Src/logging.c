/*
 * FreeRTOS STM32 Reference Integration
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Standard includes. */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Project Includes */
#include "main.h"

/* CommonIO Includes */
#include "iot_uart.h"

/*-----------------------------------------------------------*/

/* Dimensions the arrays into which print messages are created. */
#define dlMAX_PRINT_STRING_LENGTH    2048

/* Maximum amount of time to wait for the semaphore that protects the local
 * buffers and the serial output. */
#define dlMAX_SEMAPHORE_WAIT_TIME    ( pdMS_TO_TICKS( 2000UL ) )

int _write( int fd, const void * buffer, unsigned int count );

static char cPrintString[ dlMAX_PRINT_STRING_LENGTH ];
static SemaphoreHandle_t xLoggingMutex = NULL;

static IotUARTHandle_t xConsoleUart = NULL;

int _write( int fd, const void * buffer, unsigned int count )
{

	(void) fd;

	/* blocking write to UART */
    int32_t ret = iot_uart_write_sync( xConsoleUart, ( uint8_t * ) buffer, count );

	if( IOT_UART_SUCCESS != ret )
	{
		return 0;
	}
	else
	{
		return count;
	}

}

void vLoggingInit( void )
{
    int32_t status = IOT_UART_SUCCESS;

	xLoggingMutex = xSemaphoreCreateMutex();
	memset( &cPrintString, 0, dlMAX_PRINT_STRING_LENGTH );

    xConsoleUart = iot_uart_open( 0 );
    configASSERT( xConsoleUart != NULL );
    IotUARTConfig_t xConfig =
    {
        .ulBaudrate    = 115200,
        .xParity      = UART_PARITY_NONE,
        .ucWordlength  = UART_WORDLENGTH_8B,
        .xStopbits    = UART_STOPBITS_1,
        .ucFlowControl = UART_HWCONTROL_NONE
    };
    status = iot_uart_ioctl( xConsoleUart, eUartSetConfig, &xConfig );
    configASSERT( status == IOT_UART_SUCCESS );

	xSemaphoreGive(xLoggingMutex);
}


/*-----------------------------------------------------------*/

void vLoggingPrintf( const char * const pcFormat,
                     ... )
{
    int32_t iLength;
    va_list args;
    const char * const pcNewline = "\r\n";

    configASSERT( xLoggingMutex );

    /* Only proceed if the preceding call to xLoggingPrintMetadata() obtained
     * the mutex.
     */
    if( xSemaphoreGetMutexHolder( xLoggingMutex ) == xTaskGetCurrentTaskHandle() )
    {
        /* There are a variable number of parameters. */
        va_start( args, pcFormat );
        iLength = vsnprintf( cPrintString, dlMAX_PRINT_STRING_LENGTH, pcFormat, args );
        va_end( args );

        _write( 0, cPrintString, iLength );
        _write( 0, pcNewline, strlen( pcNewline ) );

        xSemaphoreGive( xLoggingMutex );
    }
}

/*-----------------------------------------------------------*/

/*
 * The prototype for this function, and the macros that call this function, are
 * both in demo_config.h.  Update the macros and prototype to pass in additional
 * meta data if required - for example the name of the function that called the
 * log message can be passed in by adding an additional parameter to the function
 * then updating the macro to pass __FUNCTION__ as the parameter value.  See the
 * comments in demo_config.h for more information.
 */
int32_t xLoggingPrintMetadata( const char * const pcLevel )
{
    const char * pcTaskName;
    const char * pcNoTask = "None";
    static BaseType_t xMessageNumber = 0;
    int32_t iLength = 0;

    configASSERT( xLoggingMutex );

    /* Get exclusive access to _write().  Note this mutex is not given back
     * until the vLoggingPrintf() function is called to ensure the meta data and
     * the log message are printed consecutively. */
    if( xSemaphoreTake( xLoggingMutex, dlMAX_SEMAPHORE_WAIT_TIME ) != pdFAIL )
    {
        /* Additional info to place at the start of the log. */
        if( xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED )
        {
            pcTaskName = pcTaskGetName( NULL );
        }
        else
        {
            pcTaskName = pcNoTask;
        }

        iLength = snprintf( cPrintString, dlMAX_PRINT_STRING_LENGTH, "%s: %s %lu %lu --- ",
                            pcLevel,
                            pcTaskName,
                            xMessageNumber++,
                            ( unsigned long ) xTaskGetTickCount() );

        _write( 0, cPrintString, iLength );
    }

    return iLength;
}

IotUARTHandle_t xLoggingGetIOHandle( void )
{
	return xConsoleUart;
}

//void vLoggingDeInit( void )
//{
//	/* Scheduler must be suspended */
//	if( xLoggingMutex != NULL &&
//	    xTaskGetSchedulerState() != taskSCHEDULER_RUNNING )
//	{
//		vSemaphoreDelete(xLoggingMutex);
//	}
//
//}

/******************************************************************************
* Software License Agreement
*
* Copyright (c) 2016, Infineon Technologies AG
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* Neither the name of the copyright holders nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission

* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* To improve the quality of the software, users are encouraged to share
* modifications, enhancements or bug fixes with Infineon Technologies AG
* (dave@infineon.com).
*
*****************************************************************************/

/****************************************************************
* HEADER FILES
***************************************************************/
#include "DAVE.h"
#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"

#if ((MB_RTU_ENABLED|MB_ASCII_ENABLED) > 0U)
/****************************************************************
* LOCAL DATA
***************************************************************/

static UART_t *ptr_uart = 0;

/****************************************************************
* API IMPLEMENTATION
***************************************************************/
void MB_register_UART(UART_t *uart)
{
	ptr_uart = uart;
}

void  vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
  if(xTxEnable)
  {
    (void)pxMBFrameCBTransmitterEmpty();
  }
}

void
vMBPortClose( void )
{
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
  BOOL return_value=false;

  if (ptr_uart != 0)
  {
    (void)UART_SetBaudrate (ptr_uart, ulBaudRate, 16U);
    return_value=true;
  }

  return return_value;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
  BOOL return_value=false;

  if (ptr_uart != 0)
  {
    UART_TransmitWord(ptr_uart,(uint8_t)ucByte);
    return_value=true;
  }

  return return_value;
}
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
  BOOL return_value=false;

  if (ptr_uart != 0)
  {
    *pucByte = (int8_t)UART_GetReceivedWord(ptr_uart);
    return_value=true;
  }

  return return_value;
}

void MB_RxHandler(void)
{
  (void)pxMBFrameCBByteReceived();
}
void MB_TxHandler(void)
{
  (void)pxMBFrameCBTransmitterEmpty();
}
#endif


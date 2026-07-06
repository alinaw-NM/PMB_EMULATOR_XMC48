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
#include <DAVE.h>
#include "port.h"
#include "mbutils.h"
#include "mb.h"

/****************************************************************
* MACROS AND DEFINES
***************************************************************/
/* Input Register Definition (16 bit; Read-Only) */
/* Input Register Start Address */
#define REG_INPUT_START_ADDR 1U
/* No of Input Registers*/
#define REG_INPUT_COUNT 4U

/* Holding Register Definition (16 bit; Read-Write) */
/* Holding Register Start Address */
#define REG_HOLDING_START_ADDR 10U
/* No of Holding Registers */
#define REG_HOLDING_COUNT 130U

/* Coil Register Definition (1 bit; Read-Write) */
/* Coil Register Start Address */
#define REG_COILS_START_ADDR 1000U
/* No of Coil Registers*/
#define REG_COILS_COUNT 16U

/* Discrete Inputs Definition (1 bit; Read-Only) */
/* Discrete Inputs Start Address */
#define REG_DISC_START_ADDR 2000U
/* No of Discrete Inputs */
#define REG_DISC_COUNT 16U

/****************************************************************
* LOCAL DATA
***************************************************************/
/* Allocate buffer for Input Register (16bit; Read-Only) */
static uint16_t reg_input_buffer[REG_INPUT_COUNT];
/* Allocate buffer for Holding Register (16bit; Read-Write) */
static uint16_t reg_holding_buffer[REG_HOLDING_COUNT];
/* Allocate buffer for discrete input register (1bit; Read-Only) */
static uint8_t reg_discrete_input_buffer[ (((uint16_t)REG_DISC_COUNT-1U) / 8U) + 1U];
/* Allocate buffer for coil register (1bit; Read-Write) */
static uint8_t reg_coils_buffers[ (((uint16_t)REG_COILS_COUNT-1U) / 8U) + 1U];

/****************************************************************
* API PROTOTYPES
***************************************************************/
void readbuttons(void);
void Rx_Cb(void);
void Tx_Cb(void);

/****************************************************************
* API IMPLEMENTATION
***************************************************************/
/**
 * @eMBRegInputCB
 *
 * Callback function called by the protocol stack for reading the 16bit value of input
 * register(s)
 *
 * @input :  - buffer  : Pointer to a buffer which is used to return the
 *                        current value of the modbus input registers to the stack.
 *           - address : Starting address of the registers.
 *           - count   : Number of registers to be returned.
 *
 * @output : - buffer  : Buffer which is updated with the modbus input registers.
 *
 * @return : MB_ENOERR if success
 *           MB_ENOREG if failure (illegal register access)
 *
 * */
eMBErrorCode eMBRegInputCB( uint8_t *buffer, uint16_t address, uint16_t count )
{
  eMBErrorCode status = MB_ENOERR;
  uint16_t     register_index;

  if (( address >= (uint16_t)REG_INPUT_START_ADDR )
     && ( (uint16_t)(address + count) <= (uint16_t)(REG_INPUT_START_ADDR + REG_INPUT_COUNT) ))
  {
    register_index = ( uint16_t )( address - REG_INPUT_START_ADDR );
    while( count > 0U )
    {
      /* Pass current register values to the protocol stack. */
      *buffer = ( uint8_t )( reg_input_buffer[register_index] >> 8 );
      buffer++;
      *buffer = ( uint8_t )( reg_input_buffer[register_index] & 0xFFU );
      buffer++;
      register_index++;
      count--;
    }
  }
  else
  {
    status = MB_ENOREG;
  }
  return status;
}

/**
 * @eMBRegHoldingCB
 *
 * Callback function called by the protocol stack for reading/writing the 16bit value of
 * holding register(s)
 *
 * @input :  - buffer  : Pointer to a buffer which is used to exchange (read/write)
 *                       current value of the modbus holding registers with the protocol stack.
 *           - address : Starting address of the register(s).
 *           - count   : Number of registers to be exchanged (read/write).
 *           - mode    : MB_REG_WRITE Protocol stack is writing input register(s).
 *                       MB_REG_READ  Protocol stack is reading input register(s).
 *
 * @output : - buffer  : Buffer which is updated with the modbus holding registers.
 *
 * @return:  MB_ENOERR if success
 *           MB_ENOREG if failure (illegal register access)
 *
 * */
eMBErrorCode eMBRegHoldingCB( uint8_t *buffer, uint16_t address, uint16_t count, eMBRegisterMode mode )
{
  eMBErrorCode status = MB_ENOERR;
  uint16_t register_index;

  if ( ( address >= REG_HOLDING_START_ADDR ) &&
     ( (uint16_t)(address + count) <= (uint16_t)(REG_HOLDING_START_ADDR + REG_HOLDING_COUNT)) )
  {
    register_index = ( uint16_t )( address - REG_HOLDING_START_ADDR );
    switch ( mode )
    {
      /* Pass current register values to the protocol stack. */
      case MB_REG_READ:
        while( count > 0U )
        {
          *buffer = (uint8_t)( reg_holding_buffer[register_index] >> 8 );
          buffer++;
          *buffer = (uint8_t)( reg_holding_buffer[register_index] & 0xFFU );
          buffer++;
          register_index++;
          count--;
        }
        break;
      /* Update current register values with new values from the
       * protocol stack. */
      case MB_REG_WRITE:
        while( count > 0U )
        {
          reg_holding_buffer[register_index] = (uint16_t)((uint16_t)*buffer << 8);
          buffer++;
          reg_holding_buffer[register_index] |= *buffer;
          buffer++;
          register_index++;
          count--;
        }
        break;
      default:
        status = MB_ENOREG;
        break;
    }
  }
  else
  {
      status = MB_ENOREG;
  }
  return status;
}

/**
 * @eMBRegCoilsCB
 *
 * Callback function called by the protocol stack for reading/writing the 1bit value of coil register(s)
 *
 * @input :  - buffer  : Pointer to a buffer which is used to exchange (read/write)
 *                       current value of the modbus coil regitser(s) with the protocol stack.
 *           - address : Starting address of the register(s).
 *           - count   : Number of registers bits to be exchanged (read/write).
 *           - mode    : MB_REG_WRITE Protocol stack is writing coil register(s).
 *                       MB_REG_READ  Protocol stack is reading coil register(s).
 *
 * @output : - buffer  : Buffer which is updated with the modbus coil register(s).
 *
 * @return:  MB_ENOERR if success
 *           MB_ENOREG if failure (illegal register access)
 *
 * */
eMBErrorCode eMBRegCoilsCB( uint8_t *buffer, uint16_t address, uint16_t count, eMBRegisterMode mode )
{
  eMBErrorCode status = MB_ENOERR;
  int16_t signed_count = (int16_t)count;
  uint16_t bit_offset;

  /* Check if we have registers mapped at this block. */
  if( ( address >= REG_COILS_START_ADDR ) &&
      ( (uint16_t)(address + count) <= (uint16_t)(REG_COILS_START_ADDR + REG_COILS_COUNT)) )
  {
    bit_offset = ( uint16_t )( address - REG_COILS_START_ADDR );
    switch ( mode )
    {
      /* Read current values and pass to protocol stack. */
      case MB_REG_READ:
        while( signed_count > 0 )
        {
          if (signed_count > 8)
          {
            *buffer = xMBUtilGetBits( reg_coils_buffers, bit_offset, 8U);
          }
          else
          {
            *buffer = xMBUtilGetBits( reg_coils_buffers, bit_offset, (uint8_t)signed_count);
          }
          buffer++;
          signed_count -= 8;
          bit_offset += 8U;
        }
        break;
      /* Update current register values. */
      case MB_REG_WRITE:
        while( signed_count > 0 )
        {
          if (signed_count > 8)
          {
            xMBUtilSetBits( reg_coils_buffers, bit_offset, 8U, *buffer );
          }
          else
          {
            xMBUtilSetBits( reg_coils_buffers, bit_offset, (uint8_t)signed_count, *buffer );
          }
          buffer++;
          signed_count -= 8;
          bit_offset += (uint8_t)8;
        }
        break;
      default:
        status = MB_ENOREG;
        break;
    }
  }
  else
  {
    status = MB_ENOREG;
  }
  return status;
}

/**
 * @eMBRegDiscreteCB
 *
 * Callback function called by the protocol stack for reading the 1bit value of discrete register(s)
 *
 * @input :  - buffer  : Pointer to a buffer which is used to return the
 *                       current value of the modbus discrete registers to the stack.
 *           - address : Starting address of the registers.
 *           - count   : Number of register bits to be returned.
 *
 * @output : - buffer  : Buffer which is updated with the modbus discrete registers.
 *
 * @return : MB_ENOERR if success
 *           MB_ENOREG if failure (illegal register access)
 *
 * */
eMBErrorCode eMBRegDiscreteCB( uint8_t *buffer, uint16_t address, uint16_t count )
{
  eMBErrorCode status = MB_ENOERR;
  int16_t signed_count = (int16_t)count;
  uint16_t bit_offset;

  /* Check if we have registers mapped at this block. */
  if( ( address >= REG_DISC_START_ADDR ) &&
    ( (uint16_t)(address + count) <= (uint16_t)(REG_DISC_START_ADDR + REG_DISC_COUNT)) )
  {
    bit_offset = (uint16_t)( address - REG_DISC_START_ADDR );
    while( signed_count > 0 )
    {
      if (signed_count > 8)
      {
         *buffer = xMBUtilGetBits( reg_discrete_input_buffer, bit_offset, 8U );
      }
      else
      {
         *buffer = xMBUtilGetBits( reg_discrete_input_buffer, bit_offset, (uint8_t)signed_count);
      }
      buffer++;
      signed_count -= 8;
      bit_offset += (uint8_t)8;
    }
  }
  else
  {
    status = MB_ENOREG;
  }
  return status;
}

/**
 * @readbuttons
 *
 * Checks state of button1 and button2 and
 * sets bit0 and bit1 of discrete input registers accordingly
 *
 * @input :  none
 *
 * @output : none
 *
 * @return : none
 *
 * */
void readbuttons(void)
{
    /* Read input of button 1 and store inside bit 0 of discrete input registers */
    if (XMC_GPIO_GetInput(P15_13))
    {
      xMBUtilSetBits( reg_discrete_input_buffer, 0U, 1U, 1U );    /* Set bit 0 to 1b. */
    }
    else
    {
      xMBUtilSetBits( reg_discrete_input_buffer, 0U, 1U, 0U );    /* Set bit 0 to 0b. */
    }

    /* Read input of button 2 and store inside bit 1 of discrete input registers */
    if (XMC_GPIO_GetInput(P15_12))
    {
      xMBUtilSetBits( reg_discrete_input_buffer, 1U, 1U, 1U );    /* Set bit 1 to 1b. */
    }
    else
    {
      xMBUtilSetBits( reg_discrete_input_buffer, 1U, 1U, 0U );    /* Set bit 1 to 0b. */
    }
}

/**
 * @brief
 *
 * Initialize Modbus RTU or ASCII mode, baudrate and device address. Enables the
 * modbus stack.
 *
 *
 * */
int32_t main(void)
{
  /*Initialize DAVE */
  if(DAVE_Init() == DAVE_STATUS_FAILURE)
  {
    /* Placeholder for error handler code.*/
    XMC_DEBUG(("DAVE APPs initialization failed\n"));
    while(1U)
    {
        /* do nothing */
    }
  }

  /* INITIALIZE BUTTON1 ON PORT 5.13 FOR INPUT */
  /* Set mode to input tristate */
  XMC_GPIO_SetMode(P15_13, XMC_GPIO_MODE_INPUT_TRISTATE);
  /* Enable digital input. Only needed because P15.13 is an analog port */
  XMC_GPIO_EnableDigitalInput(P15_13);
  /* INITIALIZE BUTTON2 ON PORT 5.12 FOR INPUT */
  /* Set mode to input tristate */
  XMC_GPIO_SetMode(P15_12, XMC_GPIO_MODE_INPUT_TRISTATE);
  /* Enable digital input. Only needed because P15.12 is an analog port */
  XMC_GPIO_EnableDigitalInput(P15_12);

  /* Set FIFO trigger limits */
  UART_SetRXFIFOTriggerLimit (&UART_0, (uint32_t)0);
  UART_SetTXFIFOTriggerLimit (&UART_0, (uint32_t)1);

  /* Register UART_0 interface for modbus usage */
  MB_register_UART(&UART_0);

  /* Initialization of modbus in RTU mode */
  (void)eMBInit( MB_RTU,            /*eMode (MB_ASCII or MB_RTU*/
		   (uint8_t)0x0A,     /*ucSlaveAddress*/
		   (uint8_t)0,        /*ignored*/
		   (uint8_t)19200,    /*ulBaudRate*/
		   (eMBParity)0         /*ignored*/
		 );
  /*Enable modbus protocol stack.*/
  (void)eMBEnable();

  /* Initialise the discrete input registers with zero's and one's
   * for demonstration purpose within this example. */
  xMBUtilSetBits( reg_discrete_input_buffer, (uint16_t)2, (uint8_t)2, (uint8_t)3 );    /* Set bit 2:3 to 11b. */
  xMBUtilSetBits( reg_discrete_input_buffer, (uint16_t)8, (uint8_t)1, (uint8_t)1 );    /* Set bit 8 to 1b. */

  /* Modbus polling loop waiting for an Event*/
  for( ;; )
  {
    /* Process modbus protocol stack */
    (void)eMBPoll();

    /* Change values inside local register banks for demonstration purpose*/
    reg_input_buffer[0]++;
    readbuttons();
  }
}

/* Callback handler of UART receiving */
void Rx_Cb(void)
{
  MB_RxHandler();
}

/* Callback handler of UART transmitting */
void Tx_Cb(void)
{
  MB_TxHandler();
}


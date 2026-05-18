/*******************************************************************************
 Copyright (c) 2016, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
	                                                                            **
 Redistribution and use in source and binary forms, with or without           **
 modification, are permitted provided that the following conditions are met:  **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
*******************************************************************************/

/**
 * @file
 * @date 21 Sept, 2016
 * @version 1.0.1
 *
 * @brief MULTICAN_CONFIG APP example
 *
 * For this demo either the XMC4700 or XMC4800 Relax kits are required.
 * The demo provides example of gateway and FIFO functionality combined with MULTICAN_CONFIG APP.
 *
 * History <br>
 *
 * Version 1.0.0 Initial version
 * Version 1.0.2 Modified for APP updates
 * Version 1.0.3 Update of MULTICAN_CONFIG APP
 */

#include <DAVE.h>                 /* Declarations from DAVE Code Generation (includes SFR declaration) */

uint8_t SW_TxData[2][8] = {{0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU, 0x66U, 0x77U},
						  {0x99U, 0x91U, 0x92U, 0x93U, 0x94U, 0x95U, 0x96U, 0x97U}};

uint8_t SW_TxDataDLC[2] = {5U, 8U};
uint8_t SW_RxData[2][8];
uint8_t SW_RxDataDLC[2];
uint8_t CAN_ISR_Count;
XMC_CAN_STATUS_t status;

void CAN_IRQHandler(void);


/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

int main(void)
{
  DAVE_STATUS_t APP_status;
  uint8_t i;
  uint8_t ubCount;

  APP_status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if(APP_status == DAVE_STATUS_FAILURE)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {
    }
  }

  /* prepare the data and transmit the MO3 twice */
  for(ubCount = 0U; ubCount < 2U; ubCount++)
  {
    /* prepare TxData for the M03 */
    MULTICAN_CONFIG_0.lmobj_ptr[3]->mo_ptr->can_data_length = SW_TxDataDLC[ubCount];
	  for (i = 0U; i < SW_TxDataDLC[ubCount]; i++)
	  {
	    MULTICAN_CONFIG_0.lmobj_ptr[3]->mo_ptr->can_data_byte[i] = SW_TxData[ubCount][i];
	  }

	  /* wait if MO3_MOSTAT.TXRQ is active */
	  while ((XMC_CAN_MO_GetStatus(MULTICAN_CONFIG_0.lmobj_ptr[3]->mo_ptr) & (uint32_t)XMC_CAN_MO_STATUS_TX_REQUEST))
	  {
	  }

	  status = XMC_CAN_MO_UpdateData(MULTICAN_CONFIG_0.lmobj_ptr[3]->mo_ptr);
	  if (status != XMC_CAN_STATUS_SUCCESS)
	  {
	    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
	    XMC_DEBUG("Transmission data has not been updated properly\n");

      while(1U)
	    {
	    }
	  }

	  /* request transmission of M03 */
	  status = XMC_CAN_MO_Transmit(MULTICAN_CONFIG_0.lmobj_ptr[3]->mo_ptr);
	  if (status != XMC_CAN_STATUS_SUCCESS)
	  {
	    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
	    XMC_DEBUG("Transmission of M03 is not successful\n");

      while(1U)
	    {
	    }
	  }
  }

  /* wait until all the data has been received in MO4 */
  while (!(CAN_ISR_Count == 2U))
  {
  }

  /* check if the CUR value matches with the expected data in the gateway source object and gateway destination base object */
  while(((MULTICAN_CONFIG_0.lmobj_ptr[0]->mo_ptr->can_mo_ptr->MOFGPR & 0x00FF0000U) >> 16U) != 1U)   /* gateway source object */
  {
  }
  while(((MULTICAN_CONFIG_0.lmobj_ptr[1]->mo_ptr->can_mo_ptr->MOFGPR & 0x00FF0000U) >> 16U) != 1U) 	/* gateway destination base object */
  {
  }

  /* compare TxData and RxData based on DLC and message data */
  for (ubCount = 0U; ubCount < 2U; ubCount++)
  {
	  if (SW_TxDataDLC[ubCount] != SW_RxDataDLC[ubCount])
	  {
	    while(1U)
	    {
	      /* should never be reached */
	    }
	  }
	  for (i = 0U; i < SW_TxDataDLC[ubCount]; i++)
	  {
	    if (SW_TxData[ubCount][i] != (uint8_t)SW_RxData[ubCount][i])
	    {
		    while(1U)
		    {
		      /* should never be reached */
		    }
	    }
	  }
  }

  /* turn on the LED to notify the user that the transmission has been successfully completed */
  XMC_GPIO_SetOutputHigh(DIGITAL_IO_0.gpio_port, DIGITAL_IO_0.gpio_pin);

  /* if everything was OK, code execution should end up here */
  while(1)
  {
  }
}

void CAN_IRQHandler(void)      /* Rx interrupt service routine */
{
  uint8_t i;
  /* read received data from MO4 into SW object */
  status = XMC_CAN_MO_Receive(MULTICAN_CONFIG_0.lmobj_ptr[4]->mo_ptr);

  if (status != XMC_CAN_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {
    }
  }

  SW_RxDataDLC[CAN_ISR_Count] = MULTICAN_CONFIG_0.lmobj_ptr[4]->mo_ptr->can_data_length;
  for (i = 0U; i < MULTICAN_CONFIG_0.lmobj_ptr[4]->mo_ptr->can_data_length; i++)
  {
    SW_RxData[CAN_ISR_Count][i] = MULTICAN_CONFIG_0.lmobj_ptr[4]->mo_ptr->can_data_byte[i];
  }

  CAN_ISR_Count++;
}

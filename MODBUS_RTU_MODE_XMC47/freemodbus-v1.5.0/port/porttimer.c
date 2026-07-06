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

/****************************************************************
* LOCAL DATA
***************************************************************/
#if MB_TCP_ENABLED
unsigned int timeout;
osTimerId TimerId = NULL;
unsigned int timeout;
#else
static uint32_t TimerId = 0U;
#endif

void wrap_pxMBPortCBTimerExpired(const void* ptr);
/****************************************************************
* API IMPLEMENTATION
***************************************************************/
void wrap_pxMBPortCBTimerExpired(const void* ptr)
{
  pxMBPortCBTimerExpired();
}

#if MB_TCP_ENABLED
void Oneshot_Timer_CallBack()
{
  pxMBPortCBTimerExpired();
}
osTimerDef(oneshot, Oneshot_Timer_CallBack);
#endif
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
#if MB_TCP_ENABLED
  timeout = usTim1Timerout50us;
#endif
  return true;
}

void
vMBPortTimersEnable(  )
{
#if MB_TCP_ENABLED
  osStatus Status = osErrorResource;
  if(TimerId != NULL)
  {
    Status = osTimerStop(TimerId);
    if(Status == osOK)
    {
      osTimerDelete(TimerId);
    }
	TimerId = osTimerCreate(osTimer(oneshot),osTimerOnce,NULL);
	Status = osTimerStart(TimerId,timeout);
  }
  else
  {
    TimerId = osTimerCreate(osTimer(oneshot),osTimerOnce,NULL);
    Status = osTimerStart(TimerId,timeout);
  }
#else
  SYSTIMER_STATUS_t Status = SYSTIMER_STATUS_FAILURE;
  if(TimerId != 0U)
  {
    Status = SYSTIMER_StopTimer(TimerId);
    if (Status == SYSTIMER_STATUS_SUCCESS)
    {
      (void)SYSTIMER_DeleteTimer(TimerId);
    }

    TimerId = SYSTIMER_CreateTimer(2000U,SYSTIMER_MODE_ONE_SHOT,wrap_pxMBPortCBTimerExpired,NULL);
    Status = SYSTIMER_StartTimer(TimerId);
  }
  else
  {
    TimerId = SYSTIMER_CreateTimer(2000U,SYSTIMER_MODE_ONE_SHOT,wrap_pxMBPortCBTimerExpired,NULL);
    Status = SYSTIMER_StartTimer(TimerId);
  }
#endif
}

void
vMBPortTimersDisable(  )
{
#if MB_TCP_ENABLED
  osStatus Status = osErrorResource;
  Status = osTimerStop(TimerId);
  if(Status ==osOK)
  {
    osTimerDelete(TimerId);
  }
  TimerId = NULL;
#else
  SYSTIMER_STATUS_t Status = SYSTIMER_STATUS_FAILURE;

  Status = SYSTIMER_StopTimer(TimerId);
  if(Status == SYSTIMER_STATUS_SUCCESS)
  {
    (void)SYSTIMER_DeleteTimer((uint32_t)TimerId);
  }
  TimerId =0U;
#endif
}

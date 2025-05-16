/*
 * spif_util.c
 *
 *  Created on: May 15, 2025
 *      Author: Megh Shah
 */

#include "spif_util.h"

/***********************************************************************************************************/

void SPIF_Delay(uint32_t Delay)
{
#if SPIF_RTOS == SPIF_RTOS_DISABLE
  HAL_Delay(Delay);
#elif (SPIF_RTOS == SPIF_RTOS_CMSIS_V1) || (SPIF_RTOS == SPIF_RTOS_CMSIS_V2)
  uint32_t d = (configTICK_RATE_HZ * Delay) / 1000;
  if (d == 0)
      d = 1;
  osDelay(d);
#elif SPIF_RTOS == SPIF_RTOS_THREADX
  uint32_t d = (TX_TIMER_TICKS_PER_SECOND * Delay) / 1000;
  if (d == 0)
    d = 1;
  tx_thread_sleep(d);
#endif
}

/***********************************************************************************************************/

void SPIF_Lock(SPIF_HandleTypeDef *Handle)
{
  while (Handle->Lock)
  {
    SPIF_Delay(1);
  }
  Handle->Lock = 1;
}

/***********************************************************************************************************/

void SPIF_UnLock(SPIF_HandleTypeDef *Handle)
{
  Handle->Lock = 0;
}

/***********************************************************************************************************/

void SPIF_CsPin(SPIF_HandleTypeDef *Handle, bool Select)
{
  HAL_GPIO_WritePin(Handle->Gpio, Handle->Pin, (GPIO_PinState)Select);
  for (int i = 0; i < 10; i++);
}




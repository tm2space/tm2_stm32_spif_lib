/*
 * spif_util.c
 *
 *  Created on: May 15, 2025
 *      Author: Megh Shah
 */

#include "spif_internal.h"

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

/***********************************************************************************************************/
/*
* @brief Sets the instruction phase parameters
*
* @param phases Pointer to the SPIF_EmulatedPhases structure
* @param enabled Whether the instruction phase is enabled
* @param size Size of the instruction in bytes
* @param instruction The instruction value
*/
void SPIF_SetCommandPhase(SPIF_HandleTypeDef* handle, bool enabled, uint8_t size, uint8_t instruction) {

   handle->phase_config.instructionEnabled= enabled;
   handle->phase_config.instructionSize = size;
   handle->phase_config.instruction = instruction;

}


/**
 * @brief Sets the address phase parameters
 *
 * @param phases Pointer to the SPIF_EmulatedPhases structure
 * @param enabled Whether the address phase is enabled
 * @param size Size of the address in bytes (typically 1-4)
 * @param address The address value
 */
void SPIF_SetAddressPhase(SPIF_HandleTypeDef *handle, bool enabled, uint8_t size, uint32_t address) {


    handle->phase_config.addressEnabled = enabled;
    handle->phase_config.addressSize = size;
    handle->phase_config.address = address;

}


/**
 * @brief Sets the dummy cycles parameter
 *
 * @param phases Pointer to the SPIF_EmulatedPhases structure
 * @param cycles Number of dummy cycles
 */
void SPIF_SetDummyCycles(SPIF_HandleTypeDef *handle, uint8_t cycles) {

    handle->phase_config.dummyCycles = cycles;

}


/**
 * @brief Sets the data phase parameters
 *
 * @param phases Pointer to the SPIF_EmulatedPhases structure
 * @param enabled Whether the data phase is enabled
 * @param length Length of data in bytes
 */
void SPIF_SetDataPhase(SPIF_HandleTypeDef *handle, bool enabled) {

    handle->phase_config.dataEnabled = enabled;
}


/************************************************************************************************************
**************    Include Headers
************************************************************************************************************/

#include "spif.h"
#include <stdlib.h>


#if SPIF_RTOS == SPIF_RTOS_DISABLE
#elif SPIF_RTOS == SPIF_RTOS_CMSIS_V1
#include "cmsis_os.h"
#include "freertos.h"
#elif SPIF_RTOS == SPIF_RTOS_CMSIS_V2
#include "cmsis_os2.h"
#include "freertos.h"
#elif SPIF_RTOS == SPIF_RTOS_THREADX
#include "app_threadx.h"
#endif

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




/***********************************************************************************************************/

/************************************************************************************************************
**************    Public Functions
************************************************************************************************************/

/***********************************************************************************************************/

/***********************************************************************************************************/

/**
  * @brief  Write data array to an Address
  * @note   Write a data array with specified size.
  * @note   All pages should be erased before write
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  Address: Start Address
  * @param  *Data: Pointer to Data
  * @param  Size: The length of data should be written. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_WriteAddress(SPIF_HandleTypeDef *Handle, uint32_t Address, uint8_t *Data, uint32_t Size)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  uint32_t page, add, offset, remaining, length, maximum, index = 0;
  add = Address;
  remaining = Size;
  do
  {
    page = SPIF_AddressToPage(add);
    offset = add % SPIF_PAGE_SIZE;
    maximum = SPIF_PAGE_SIZE - offset;
    if (remaining <= maximum)
    {
      length = remaining;
    }
    else
    {
      length = maximum;
    }
    if (SPIF_WriteFn(Handle, page, &Data[index], length, offset) == false)
    {
      break;
    }
    add += length;
    index += length;
    remaining -= length;
    if (remaining == 0)
    {
      retVal = true;
      break;
    }

  } while (remaining > 0);

  SPIF_UnLock(Handle);
  return retVal;
}

/***********************************************************************************************************/

/**
  * @brief  Write data array to a Page
  * @note   Write a data array with specified size.
  * @note   The Page should be erased before write
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  PageNumber: Page Number
  * @param  *Data: Pointer to Data
  * @param  Size: The length of data should be written. (in byte)
  * @param  Offset: The start point for writing data. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_WritePage(SPIF_HandleTypeDef *Handle, uint32_t PageNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  retVal = SPIF_WriteFn(Handle, PageNumber, Data, Size, Offset);
  SPIF_UnLock(Handle);
  return retVal;
}

/***********************************************************************************************************/

/**
  * @brief  Write data array to a Sector
  * @note   Write a data array with specified size.
  * @note   The Sector should be erased before write
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  SectorNumber: Sector Number
  * @param  *Data: Pointer to Data
  * @param  Size: The length of data should be written. (in byte)
  * @param  Offset: The start point for writing data. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_WriteSector(SPIF_HandleTypeDef *Handle, uint32_t SectorNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  SPIF_Lock(Handle);
  bool retVal = true;
  do
  {
    if (Offset >= SPIF_SECTOR_SIZE)
    {
      retVal = false;
      break;
    }
    if (Size > (SPIF_SECTOR_SIZE - Offset))
    {
      Size = SPIF_SECTOR_SIZE - Offset;
    }
    uint32_t bytesWritten = 0;
    uint32_t pageNumber = SectorNumber * (SPIF_SECTOR_SIZE / SPIF_PAGE_SIZE);
    pageNumber += Offset / SPIF_PAGE_SIZE;
    uint32_t remainingBytes = Size;
    uint32_t pageOffset = Offset % SPIF_PAGE_SIZE;
    while (remainingBytes > 0 && pageNumber < ((SectorNumber + 1) * (SPIF_SECTOR_SIZE / SPIF_PAGE_SIZE)))
    {
      uint32_t bytesToWrite = (remainingBytes > (SPIF_PAGE_SIZE - pageOffset)) ? (SPIF_PAGE_SIZE - pageOffset) : remainingBytes;
      if (SPIF_WriteFn(Handle, pageNumber, Data + bytesWritten, bytesToWrite, pageOffset) == false)
      {
        retVal = false;
        break;
      }
      bytesWritten += bytesToWrite;
      remainingBytes -= bytesToWrite;
      pageNumber++;
      pageOffset = 0;
    }
  } while (0);
  SPIF_UnLock(Handle);
  return retVal;
}

/***********************************************************************************************************/

/**
  * @brief  Write data array to a Block
  * @note   Write a data array with specified size.
  * @note   The Block should be erased before write
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  SectorNumber: Block Number
  * @param  *Data: Pointer to Data
  * @param  Size: The length of data should be written. (in byte)
  * @param  Offset: The start point for writing data. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_WriteBlock(SPIF_HandleTypeDef *Handle, uint32_t BlockNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  SPIF_Lock(Handle);
  bool retVal = true;
  do
  {
    if (Offset >= SPIF_BLOCK_SIZE)
    {
      retVal = false;
      break;
    }
    if (Size > (SPIF_BLOCK_SIZE - Offset))
    {
      Size = SPIF_BLOCK_SIZE - Offset;
    }
    uint32_t bytesWritten = 0;
    uint32_t pageNumber = BlockNumber * (SPIF_BLOCK_SIZE / SPIF_PAGE_SIZE);
    pageNumber += Offset / SPIF_PAGE_SIZE;
    uint32_t remainingBytes = Size;
    uint32_t pageOffset = Offset % SPIF_PAGE_SIZE;
    while (remainingBytes > 0 && pageNumber < ((BlockNumber + 1) * (SPIF_BLOCK_SIZE / SPIF_PAGE_SIZE)))
    {
      uint32_t bytesToWrite = (remainingBytes > (SPIF_PAGE_SIZE - pageOffset)) ? (SPIF_PAGE_SIZE - pageOffset) : remainingBytes;
      if (SPIF_WriteFn(Handle, pageNumber, Data + bytesWritten, bytesToWrite, pageOffset) == false)
      {
        retVal = false;
        break;
      }
      bytesWritten += bytesToWrite;
      remainingBytes -= bytesToWrite;
      pageNumber++;
      pageOffset = 0;
    }

  } while (0);

  SPIF_UnLock(Handle);
  return retVal;
}

/***********************************************************************************************************/

/**
  * @brief  Read From Address
  * @note   Read data from memory and copy to array
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  Address: Start Address
  * @param  *Data: Pointer to Data (output)
  * @param  Size: The length of data should be written. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_ReadAddress(SPIF_HandleTypeDef *Handle, uint32_t Address, uint8_t *Data, uint32_t Size)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  retVal = SPIF_ReadFn(Handle, Address, Data, Size);
  SPIF_UnLock(Handle);
  return retVal;
}

/***********************************************************************************************************/

/**
  * @brief  Read a Page
  * @note   Read a page and copy to array
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  PageNumber: Page Number
  * @param  *Data: Pointer to Data (output)
  * @param  Size: The length of data should be read. (in byte)
  * @param  Offset: The start point for Reading data. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_ReadPage(SPIF_HandleTypeDef *Handle, uint32_t PageNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  uint32_t address = SPIF_PageToAddress(PageNumber);
  uint32_t maximum = SPIF_PAGE_SIZE - Offset;
  if (Size > maximum)
  {
    Size = maximum;
  }
  retVal = SPIF_ReadFn(Handle, address, Data, Size);
  SPIF_UnLock(Handle);
  return retVal;
}

/***********************************************************************************************************/

/**
  * @brief  Read a Sector
  * @note   Read a Sector and copy to array
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  SectorNumber: Sector Number
  * @param  *Data: Pointer to Data (output)
  * @param  Size: The length of data should be read. (in byte)
  * @param  Offset: The start point for Reading data. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_ReadSector(SPIF_HandleTypeDef *Handle, uint32_t SectorNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  uint32_t address = SPIF_SectorToAddress(SectorNumber);
  uint32_t maximum = SPIF_SECTOR_SIZE - Offset;
  if (Size > maximum)
  {
    Size = maximum;
  }
  retVal = SPIF_ReadFn(Handle, address, Data, Size);
  SPIF_UnLock(Handle);
  return retVal;
}

/***********************************************************************************************************/

/**
  * @brief  Read a Block
  * @note   Read a Block and copy to array
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  BlockNumber: Block Number
  * @param  *Data: Pointer to Data (output)
  * @param  Size: The length of data should be read. (in byte)
  * @param  Offset: The start point for Reading data. (in byte)
  *
  * @retval bool: true or false
  */
bool SPIF_ReadBlock(SPIF_HandleTypeDef *Handle, uint32_t BlockNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  uint32_t address = SPIF_BlockToAddress(BlockNumber);
  uint32_t maximum = SPIF_BLOCK_SIZE - Offset;
  if (Size > maximum)
  {
    Size = maximum;
  }
  retVal = SPIF_ReadFn(Handle, address, Data, Size);
  SPIF_UnLock(Handle);
  return retVal;
}

/*
  * @brief  Send Command to SPIF
  * @note   Send command and data to SPIF
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  Cmd: Command to send
  * @param  *Data: Pointer to Data (input), Data can be null
  * @param  Size: The length of data should be written. (in byte), Size can be 0
  *
  * @retval bool: true or false
*/
bool SPIF_SendCmd(SPIF_HandleTypeDef *Handle, uint8_t Cmd, uint8_t *Data, uint32_t Size){
  bool retVal = false;
  uint8_t tx[1] = {Cmd};
  SPIF_CsPin(Handle, 0);
  if (SPIF_WriteEnable(Handle) == false)
  {
    SPIF_CsPin(Handle, 1);
    dprintf("SPIF_SendCmd() Error\r\n");
    return retVal;
  }

  if (SPIF_Transmit(Handle, tx, 1, 100) == false)
  {
    SPIF_CsPin(Handle, 1);
    dprintf("SPIF_SendCmd() Error\r\n");
    return retVal;
  }

  if (Data != NULL && Size > 0)
  {
    if (SPIF_Transmit(Handle, Data, Size, 100) == false)
    {
      SPIF_CsPin(Handle, 1);
      dprintf("SPIF_SendCmd() Error\r\n");
      return retVal;
    }
  }

  SPIF_CsPin(Handle, 1);
  retVal = true;
  return retVal;
}


bool SPIF_SendCmdReceive(SPIF_HandleTypeDef *Handle, uint8_t Cmd, uint8_t *RxData, uint32_t Size)
{
  bool retVal = false;
  uint8_t tx[1] = {Cmd};
  SPIF_CsPin(Handle, 0);
  if (SPIF_WriteEnable(Handle) == false)
  {
    SPIF_CsPin(Handle, 1);
    dprintf("SPIF_SendCmdReceive() Error\r\n");
    return retVal;
  }

  if (SPIF_TransmitReceive(Handle, tx, RxData, Size, 100) == false)
  {
    SPIF_CsPin(Handle, 1);
    dprintf("SPIF_SendCmdReceive() Error\r\n");
    return retVal;
  }

  SPIF_WriteDisable(Handle);
  SPIF_CsPin(Handle, 1);
  retVal = true;
  return retVal;
}

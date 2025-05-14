#include "spif_compat_is25xx.h"
#include "spif.h"
#include "NimaLTD.I-CUBE-SPIF_conf.h"

#if (SPIF_PLATFORM == SPIF_PLATFORM_OCTOSPI)
  #include "spif_interface_octospi.h"
#else
  #include "spif_interface_spi.h"
#endif





uint8_t SPIF_IS25_ReadReg1(SPIF_HandleTypeDef *Handle)
{
  uint8_t retVal = 0;
  uint8_t tx[2] = {SPIF_CMD_READSTATUS1, SPIF_DUMMY_BYTE};
  uint8_t rx[2];
  SPIF_CsPin(Handle, 0);
  if (SPIF_TransmitReceive(Handle, tx, rx, 2, 100) == true)
  {
    retVal = rx[1];
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
}

bool SPIF_IS25_WriteReg1(SPIF_HandleTypeDef *Handle, uint8_t Data)
{
  bool retVal = true;
  uint8_t tx[2] = {SPIF_CMD_WRITESTATUS1, Data};

  do
  {
    
    if (SPIF_WriteEnable(Handle) == false){
      retVal = false;
      break;
    }

    SPIF_CsPin(Handle, 0);
    if (SPIF_Transmit(Handle, tx, 2, 100) == false)
    {
      retVal = false;
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);
  } while (0);

  return retVal;
}


/*
  * @brief  Write QPI Enable command
  * @note   Send the QPI-Enable command
  * 
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @retval bool: true or false
  */
bool SPIF_IS25_QPI_Enable(SPIF_HandleTypeDef *Handle)
{
  bool retVal = true;
  uint8_t tx[1] = {SPIF_CMD_ENTERQPI};
  SPIF_CsPin(Handle, 0);
  if (SPIF_Transmit(Handle, tx, 1, 100) == false)
  {
    retVal = false;
    dprintf("SPIF_IS25_QPI_Enable() Error\r\n");
  }
  SPIF_CsPin(Handle, 1);
   return retVal;
 }



/**
  * @brief  Erase 32K Block.
  * @note   Send the Erase-Block command and wait for completion
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  Sector: Selected Block
  *
  * @retval bool: true or false
  */
bool SPIF_IS25XX_EraseBlock_32K(SPIF_HandleTypeDef *Handle, uint32_t Block)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  uint32_t address = Block * SPIF_BLOCK_SIZE;
  uint8_t tx[5];
  do
  {
#if SPIF_DEBUG != SPIF_DEBUG_DISABLE
    uint32_t dbgTime = HAL_GetTick();
#endif
    dprintf("SPIF_EraseBlock() START PAGE %ld\r\n", Block);
    if (Block >= Handle->BlockCnt)
    {
      dprintf("SPIF_EraseBlock() ERROR Block NUMBER\r\n");
      break;
    }
    if (SPIF_WriteEnable(Handle) == false)
    {
      break;
    }
    SPIF_CsPin(Handle, 0);
    if (Handle->BlockCnt >= 512)
    {
      tx[0] = SPIF_CMD_BLOCKERASE32K4ADD;
      tx[1] = (address & 0xFF000000) >> 24;
      tx[2] = (address & 0x00FF0000) >> 16;
      tx[3] = (address & 0x0000FF00) >> 8;
      tx[4] = (address & 0x000000FF);
      if (SPIF_Transmit(Handle, tx, 5, 100) == false)
      {
        SPIF_CsPin(Handle, 1);
        break;
      }
    }
    else
    {
      tx[0] = SPIF_CMD_BLOCKERASE32K3ADD;
      tx[1] = (address & 0x00FF0000) >> 16;
      tx[2] = (address & 0x0000FF00) >> 8;
      tx[3] = (address & 0x000000FF);
      if (SPIF_Transmit(Handle, tx, 4, 100) == false)
      {
        SPIF_CsPin(Handle, 1);
        break;
      }
    }
    SPIF_CsPin(Handle, 1);
    if (SPIF_WaitForWriting(Handle, 3000))
    {
      dprintf("SPIF_EraseBlock() DONE AFTER %ld ms\r\n", HAL_GetTick() - dbgTime);
      retVal = true;
    }

  } while (0);

  SPIF_WriteDisable(Handle);
  SPIF_UnLock(Handle);
  return retVal;
}

/**
  * @brief  Erase 64K Block.
  * @note   Send the Erase-Block command and wait for completion
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  Sector: Selected Block
  *
  * @retval bool: true or false
  */
bool SPIF_IS25XX_EraseBlock_64K(SPIF_HandleTypeDef *Handle, uint32_t Block){
	//to be implemented
	__NOP();
}

#include "spif_compat_w25xx.h"
#include "spif.h"
#include "NimaLTD.I-CUBE-SPIF_conf.h"


// This reads status register 1 for SPIF_COMPAT_W25XX
// This will also read the only status register for SPIF_COMPAT_IS25XX
uint8_t SPIF_W25XX_ReadReg1(SPIF_HandleTypeDef *Handle)
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

/***********************************************************************************************************/

uint8_t SPIF_W25XX_ReadReg2(SPIF_HandleTypeDef *Handle)
{

  uint8_t retVal = 0;
  uint8_t tx[2] = {SPIF_CMD_READSTATUS2, SPIF_DUMMY_BYTE};
  uint8_t rx[2];
  SPIF_CsPin(Handle, 0);
  if (SPIF_TransmitReceive(Handle, tx, rx, 2, 100) == true)
  {
    retVal = rx[1];
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
}


uint8_t SPIF_W25XX_ReadReg3(SPIF_HandleTypeDef *Handle)
{
  
  uint8_t retVal = 0;
  uint8_t tx[2] = {SPIF_CMD_READSTATUS3, SPIF_DUMMY_BYTE};
  uint8_t rx[2];
  SPIF_CsPin(Handle, 0);
  if (SPIF_TransmitReceive(Handle, tx, rx, 2, 100) == true)
  {
    retVal = rx[1];
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
  
}


bool SPIF_W25XX_WriteReg1(SPIF_HandleTypeDef *Handle, uint8_t Data)
{
  bool retVal = true;
  uint8_t tx[2] = {SPIF_CMD_WRITESTATUS1, Data};

  uint8_t cmd = SPIF_CMD_WRITESTATUSEN;

  do
  {
    
    SPIF_CsPin(Handle, 0);
    if (SPIF_Transmit(Handle, &cmd, 1, 100) == false)
    {
      retVal = false;
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);

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


bool SPIF_W25XX_WriteReg2(SPIF_HandleTypeDef *Handle, uint8_t Data)
{
  
  bool retVal = true;
  uint8_t tx[2] = {SPIF_CMD_WRITESTATUS2, Data};
  uint8_t cmd = SPIF_CMD_WRITESTATUSEN;
  do
  {
    SPIF_CsPin(Handle, 0);
    if (SPIF_Transmit(Handle, &cmd, 1, 100) == false)
    {
      retVal = false;
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);
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


bool SPIF_W25XX_WriteReg3(SPIF_HandleTypeDef *Handle, uint8_t Data)
{

  bool retVal = true;
  uint8_t tx[2] = {SPIF_CMD_WRITESTATUS3, Data};
  uint8_t cmd = SPIF_CMD_WRITESTATUSEN;
  do
  {
    SPIF_CsPin(Handle, 0);
    if (SPIF_Transmit(Handle, &cmd, 1, 100) == false)
    {
      retVal = false;
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);
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


/**
  * @brief  Erase Block.
  * @note   Send the Erase-Block command and wait for completion
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  Sector: Selected Block
  *
  * @retval bool: true or false
  */
bool SPIF_W25XX_EraseBlock(SPIF_HandleTypeDef *Handle, uint32_t Block)
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
      tx[0] = SPIF_CMD_BLOCKERASE4ADD;
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
      tx[0] = SPIF_CMD_BLOCKERASE3ADD;
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




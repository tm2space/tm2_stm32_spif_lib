
#include "spif_compat_w25xx.h"
#include "spif.h"

#if (SPIF_COMPAT == SPIF_COMPAT_W25XX)

/*Register Definition for W25XX*/
#define SPIF_DUMMY_BYTE 0xA5

#define SPIF_CMD_READSFDP 0x5A
#define SPIF_CMD_ID 0x90
#define SPIF_CMD_JEDECID 0x9F
#define SPIF_CMD_UNIQUEID 0x4B
#define SPIF_CMD_WRITEDISABLE 0x04
#define SPIF_CMD_READSTATUS1 0x05
#define SPIF_CMD_READSTATUS2 0x35
#define SPIF_CMD_READSTATUS3 0x15
#define SPIF_CMD_WRITESTATUSEN 0x50
#define SPIF_CMD_WRITESTATUS1 0x01
#define SPIF_CMD_WRITESTATUS2 0x31
#define SPIF_CMD_WRITESTATUS3 0x11
#define SPIF_CMD_WRITEENABLE 0x06
#define SPIF_CMD_ADDR4BYTE_EN 0xB7
#define SPIF_CMD_ADDR4BYTE_DIS 0xE9
#define SPIF_CMD_PAGEPROG3ADD 0x02
#define SPIF_CMD_PAGEPROG4ADD 0x12
#define SPIF_CMD_READDATA3ADD 0x03
#define SPIF_CMD_READDATA4ADD 0x13
#define SPIF_CMD_FASTREAD3ADD 0x0B
#define SPIF_CMD_FASTREAD4ADD 0x0C
#define SPIF_CMD_SECTORERASE3ADD 0x20
#define SPIF_CMD_SECTORERASE4ADD 0x21
#define SPIF_CMD_BLOCKERASE3ADD 0xD8
#define SPIF_CMD_BLOCKERASE4ADD 0xDC
#define SPIF_CMD_CHIPERASE1 0x60
#define SPIF_CMD_CHIPERASE2 0xC7
#define SPIF_CMD_SUSPEND 0x75
#define SPIF_CMD_RESUME 0x7A
#define SPIF_CMD_POWERDOWN 0xB9
#define SPIF_CMD_RELEASE 0xAB
#define SPIF_CMD_FRAMSERNO 0xC3

#define SPIF_STATUS1_BUSY (1 << 0)
#define SPIF_STATUS1_WEL (1 << 1)
#define SPIF_STATUS1_BP0 (1 << 2)
#define SPIF_STATUS1_BP1 (1 << 3)
#define SPIF_STATUS1_BP2 (1 << 4)
#define SPIF_STATUS1_TP (1 << 5)
#define SPIF_STATUS1_SEC (1 << 6)
#define SPIF_STATUS1_SRP0 (1 << 7)

#define SPIF_STATUS2_SRP1 (1 << 0)
#define SPIF_STATUS2_QE (1 << 1)
#define SPIF_STATUS2_RESERVE1 (1 << 2)
#define SPIF_STATUS2_LB0 (1 << 3)
#define SPIF_STATUS2_LB1 (1 << 4)
#define SPIF_STATUS2_LB2 (1 << 5)
#define SPIF_STATUS2_CMP (1 << 6)
#define SPIF_STATUS2_SUS (1 << 7)

#define SPIF_STATUS3_RESERVE1 (1 << 0)
#define SPIF_STATUS3_RESERVE2 (1 << 1)
#define SPIF_STATUS3_WPS (1 << 2)
#define SPIF_STATUS3_RESERVE3 (1 << 3)
#define SPIF_STATUS3_RESERVE4 (1 << 4)
#define SPIF_STATUS3_DRV0 (1 << 5)
#define SPIF_STATUS3_DRV1 (1 << 6)
#define SPIF_STATUS3_HOLD (1 << 7)



bool SPIF_WriteEnable(SPIF_HandleTypeDef *Handle)
{
  bool retVal = true;
  uint8_t tx[1] = {SPIF_CMD_WRITEENABLE};
	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITEENABLE);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 0);
	SPIF_SetDataPhase(Handle, false);

  SPIF_CsPin(Handle, 0);
  if (SPIF_Transmit(Handle, tx, 1, 100) == false)
  {
    retVal = false;
    dprintf("SPIF_WriteEnable() Error\r\n");
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
}


bool SPIF_WriteDisable(SPIF_HandleTypeDef *Handle)
{
  bool retVal = true;
  uint8_t tx[1] = {SPIF_CMD_WRITEDISABLE};
	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITEDISABLE);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 0);
	SPIF_SetDataPhase(Handle, false);
  SPIF_CsPin(Handle, 0);
  if (SPIF_Transmit(Handle, tx, 1, 100) == false)
  {
    retVal = false;
    dprintf("SPIF_WriteDisable() Error\r\n");
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
}


bool SPIF_WaitForWriting(SPIF_HandleTypeDef *Handle, uint32_t Timeout)
{
  bool retVal = false;
  uint32_t startTime = HAL_GetTick();
  while (1)
  {
    SPIF_Delay(1);
    if (HAL_GetTick() - startTime >= Timeout)
    {
      dprintf("SPIF_WaitForWriting() TIMEOUT\r\n");
      break;
    }
    if ((SPIF_ReadReg1(Handle) & SPIF_STATUS1_BUSY) == 0)

    {
      retVal = true;
      break;
    }
  }
  return retVal;
}



// This reads status register 1 for SPIF_COMPAT_W25XX
// This will also read the only status register for SPIF_COMPAT_IS25XX
uint8_t SPIF_ReadReg1(SPIF_HandleTypeDef *Handle)
{
  uint8_t retVal = 0;
//  uint8_t tx[2] = {SPIF_CMD_READSTATUS1, SPIF_DUMMY_BYTE};
  uint8_t rx[2];
	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_READSTATUS1);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 1);
	SPIF_SetDataPhase(Handle, false);
  SPIF_CsPin(Handle, 0);
  if (SPIF_Receive(Handle,  rx, 2, 100) == true)
  {
    retVal = rx[1];
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
}

/***********************************************************************************************************/

uint8_t SPIF_ReadReg2(SPIF_HandleTypeDef *Handle)
{

  uint8_t retVal = 0;
//  uint8_t tx[2] = {SPIF_CMD_READSTATUS2, SPIF_DUMMY_BYTE};
  uint8_t rx[2];
	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_READSTATUS2);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 1);
	SPIF_SetDataPhase(Handle, false);
  SPIF_CsPin(Handle, 0);
  if (SPIF_Receive(Handle, rx, 2, 100) == true)
  {
    retVal = rx[1];
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
}


uint8_t SPIF_ReadReg3(SPIF_HandleTypeDef *Handle)
{
  
  uint8_t retVal = 0;
//  uint8_t tx[2] = {SPIF_CMD_READSTATUS3, SPIF_DUMMY_BYTE};
  uint8_t rx[2];
	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_READSTATUS3);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 1);
	SPIF_SetDataPhase(Handle, false);
  SPIF_CsPin(Handle, 0);
  if (SPIF_Receive(Handle,  rx, 2, 100) == true)
  {
    retVal = rx[1];
  }
  SPIF_CsPin(Handle, 1);
  return retVal;
  
}


bool SPIF_WriteReg1(SPIF_HandleTypeDef *Handle, uint8_t Data)
{
  bool retVal = true;
  uint8_t tx[1] = { Data};

  uint8_t cmd = SPIF_CMD_WRITESTATUSEN;

  do
  {
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITESTATUSEN);
		SPIF_SetAddressPhase(Handle, false, 0, 0);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, false);

    SPIF_CsPin(Handle, 0);
    if (SPIF_Transmit(Handle, &cmd, 1, 100) == false)
    {
      retVal = false;
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);

	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITESTATUS1);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 0);
	SPIF_SetDataPhase(Handle, true);

    SPIF_CsPin(Handle, 0);
    if (SPIF_Transmit(Handle, tx, 1, 100) == false)
    {
      retVal = false;
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);
  } while (0);

  return retVal;
}


bool SPIF_WriteReg2(SPIF_HandleTypeDef *Handle, uint8_t Data)
{
  
  bool retVal = true;
  uint8_t tx[1] = { Data};
  uint8_t cmd = SPIF_CMD_WRITESTATUSEN;
  do
    {
  		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITESTATUSEN);
  		SPIF_SetAddressPhase(Handle, false, 0, 0);
  		SPIF_SetDummyCycles(Handle, 0);
  		SPIF_SetDataPhase(Handle, false);

      SPIF_CsPin(Handle, 0);
      if (SPIF_Transmit(Handle, &cmd, 1, 100) == false)
      {
        retVal = false;
        SPIF_CsPin(Handle, 1);
        break;
      }
      SPIF_CsPin(Handle, 1);

  	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITESTATUS2);
  	SPIF_SetAddressPhase(Handle, false, 0, 0);
  	SPIF_SetDummyCycles(Handle, 0);
  	SPIF_SetDataPhase(Handle, true);

      SPIF_CsPin(Handle, 0);
      if (SPIF_Transmit(Handle, tx, 1, 100) == false)
      {
        retVal = false;
        SPIF_CsPin(Handle, 1);
        break;
      }
      SPIF_CsPin(Handle, 1);
    } while (0);

  return retVal;
}


bool SPIF_WriteReg3(SPIF_HandleTypeDef *Handle, uint8_t Data)
{

  bool retVal = true;
  uint8_t tx[1] = { Data};
  uint8_t cmd = SPIF_CMD_WRITESTATUSEN;
  do
    {
  		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITESTATUSEN);
  		SPIF_SetAddressPhase(Handle, false, 0, 0);
  		SPIF_SetDummyCycles(Handle, 0);
  		SPIF_SetDataPhase(Handle, false);

      SPIF_CsPin(Handle, 0);
      if (SPIF_Transmit(Handle, &cmd, 1, 100) == false)
      {
        retVal = false;
        SPIF_CsPin(Handle, 1);
        break;
      }
      SPIF_CsPin(Handle, 1);

  	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_WRITESTATUS3);
  	SPIF_SetAddressPhase(Handle, false, 0, 0);
  	SPIF_SetDummyCycles(Handle, 0);
  	SPIF_SetDataPhase(Handle, true);

      SPIF_CsPin(Handle, 0);
      if (SPIF_Transmit(Handle, tx, 1, 100) == false)
      {
        retVal = false;
        SPIF_CsPin(Handle, 1);
        break;
      }
      SPIF_CsPin(Handle, 1);
    } while (0);

  return retVal;
 
}


bool SPIF_WriteFn(SPIF_HandleTypeDef *Handle, uint32_t PageNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  bool retVal = false;
  uint32_t address = 0, maximum = SPIF_PAGE_SIZE - Offset;
//  uint8_t tx[5];
  do
  {
#if SPIF_DEBUG != SPIF_DEBUG_DISABLE
    uint32_t dbgTime = HAL_GetTick();
#endif
    dprintf("SPIF_WritePage() START PAGE %ld\r\n", PageNumber);
    if (PageNumber >= Handle->PageCnt)
    {
      dprintf("SPIF_WritePage() ERROR PageNumber\r\n");
      break;
    }
    if (Offset >= SPIF_PAGE_SIZE)
    {
      dprintf("SPIF_WritePage() ERROR Offset\r\n");
      break;
    }
    if (Size > maximum)
    {
      Size = maximum;
    }
    address = SPIF_PageToAddress(PageNumber) + Offset;
#if SPIF_DEBUG == SPIF_DEBUG_FULL
      dprintf("SPIF WRITING {\r\n0x%02X", Data[0]);
      for (int i = 1; i < Size; i++)
      {
        if (i % 8 == 0)
        {
          dprintf("\r\n");
        }
        dprintf(", 0x%02X", Data[i]);
      }
      dprintf("\r\n}\r\n");
#endif
    if (SPIF_WriteEnable(Handle) == false)
    {
      break;
    }
    SPIF_CsPin(Handle, 0);
    if (Handle->BlockCnt >= 512)
    {
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_PAGEPROG4ADD);
		SPIF_SetAddressPhase(Handle, true, 4, address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, true);




    }
    else
    {
		SPIF_SetInstructionPhase(Handle, true, 1,SPIF_CMD_PAGEPROG3ADD );
		SPIF_SetAddressPhase(Handle, true, 3, address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, true);


    }
    if (SPIF_Transmit(Handle, Data, Size, 1000) == false)
    {
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);
    if (SPIF_WaitForWriting(Handle, 100))
    {
      dprintf("SPIF_WritePage() %d BYTES WITERN DONE AFTER %ld ms\r\n", (uint16_t)Size, HAL_GetTick() - dbgTime);
      retVal = true;
    }

  } while (0);

  SPIF_WriteDisable(Handle);
  return retVal;
}



bool SPIF_ReadFn(SPIF_HandleTypeDef *Handle, uint32_t Address, uint8_t *Data, uint32_t Size)
{
  bool retVal = false;
//  uint8_t tx[5];
  do
  {
#if SPIF_DEBUG != SPIF_DEBUG_DISABLE
    uint32_t dbgTime = HAL_GetTick();
#endif
    dprintf("SPIF_ReadAddress() START ADDRESS %ld\r\n", Address);
    SPIF_CsPin(Handle, 0);
    if (Handle->BlockCnt >= 512)
    {
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_READDATA4ADD);
		SPIF_SetAddressPhase(Handle, true, 4, Address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, false);


    }
    else
    {
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_READDATA3ADD);
		SPIF_SetAddressPhase(Handle, true, 3, Address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, false);


    }
    uint16_t max_uint16_size = 0xFFFF - 2;
    if(Size > max_uint16_size)
    {
    	uint32_t delta = Size;
    	uint32_t Data_offset = 0;
    	while(delta > 0)
    	{
    		if(delta <= max_uint16_size)
    		{
    			if (SPIF_Receive(Handle, (Data+Data_offset), delta, 2000) == false)
    			{
    				SPIF_CsPin(Handle, 1);
    				break;
    			}
    			delta = 0;
    		}
    		else
    		{
    			if (SPIF_Receive(Handle, (Data+Data_offset), max_uint16_size, 2000) == false)
    			{
    				SPIF_CsPin(Handle, 1);
    				break;
    			}
    			delta = abs(delta - max_uint16_size);
    			Data_offset += max_uint16_size;
    		}
    	}
    	if(delta > 0)
    	{
    		//break the main do-while loop, so as to return false
    		break;
    	}
    }
    else
    {
    	if(SPIF_Receive(Handle, Data, Size, 2000) == false)
    	{
    		SPIF_CsPin(Handle, 1);
    		break;
    	}
    }
    SPIF_CsPin(Handle, 1);
    dprintf("SPIF_ReadAddress() %d BYTES READ DONE AFTER %ld ms\r\n", (uint16_t)Size, HAL_GetTick() - dbgTime);
#if SPIF_DEBUG == SPIF_DEBUG_FULL
    dprintf("{\r\n0x%02X", Data[0]);
    for (int i = 1; i < Size; i++)
    {
      if (i % 8 == 0)
      {
        dprintf("\r\n");
      }
      dprintf(", 0x%02X", Data[i]);
    }
    dprintf("\r\n}\r\n");
#endif
    retVal = true;

  } while (0);

  return retVal;
}



/**
  * @brief  Full Erase chip.
  * @note   Send the Full-Erase-chip command and wait for completion
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  *
  * @retval bool: true or false
  */
bool SPIF_EraseChip(SPIF_HandleTypeDef *Handle)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  uint8_t tx[1] = {SPIF_CMD_CHIPERASE1};
  do
  {
#if SPIF_DEBUG != SPIF_DEBUG_DISABLE
    uint32_t dbgTime = HAL_GetTick();
#endif
    dprintf("SPIF_EraseChip() START\r\n");
    if (SPIF_WriteEnable(Handle) == false)
    {
      break;
    }
	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_CHIPERASE1);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 0);
	SPIF_SetDataPhase(Handle, false);

    SPIF_CsPin(Handle, 0);
    if (SPIF_Transmit(Handle, tx, 1, 100) == false)
    {
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);
    if (SPIF_WaitForWriting(Handle, Handle->BlockCnt * 1000))
    {
      dprintf("SPIF_EraseChip() DONE AFTER %ld ms\r\n", HAL_GetTick() - dbgTime);
      retVal = true;
    }

  } while (0);

  SPIF_WriteDisable(Handle);
  SPIF_UnLock(Handle);
  return retVal;
}


/**
  * @brief  Erase Sector.
  * @note   Send the Erase-Sector command and wait for completion
  *
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @param  Sector: Selected Sector
  *
  * @retval bool: true or false
  */
bool SPIF_EraseSector(SPIF_HandleTypeDef *Handle, uint32_t Sector)
{
  SPIF_Lock(Handle);
  bool retVal = false;
  uint32_t address = Sector * SPIF_SECTOR_SIZE;
  uint8_t tx[5];
  do
  {
#if SPIF_DEBUG != SPIF_DEBUG_DISABLE
    uint32_t dbgTime = HAL_GetTick();
#endif
    dprintf("SPIF_EraseSector() START SECTOR %ld\r\n", Sector);
    if (Sector >= Handle->SectorCnt)
    {
      dprintf("SPIF_EraseSector() ERROR Sector NUMBER\r\n");
      break;
    }
    if (SPIF_WriteEnable(Handle) == false)
    {
      break;
    }
    SPIF_CsPin(Handle, 0);
    if (Handle->BlockCnt >= 512)
    {
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_SECTORERASE4ADD);
		SPIF_SetAddressPhase(Handle, true, 4, address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, false);

      if (SPIF_Transmit(Handle, tx, 5, 100) == false)
      {
        SPIF_CsPin(Handle, 1);
        break;
      }
    }
    else
    {
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_SECTORERASE3ADD);
		SPIF_SetAddressPhase(Handle, true, 3, address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, false);
      tx[0] = SPIF_CMD_SECTORERASE3ADD;
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
    if (SPIF_WaitForWriting(Handle, 1000))
    {
      dprintf("SPIF_EraseSector() DONE AFTER %ld ms\r\n", HAL_GetTick() - dbgTime);
      retVal = true;
    }

  } while (0);

  SPIF_WriteDisable(Handle);
  SPIF_UnLock(Handle);
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
bool SPIF_EraseBlock(SPIF_HandleTypeDef *Handle, uint32_t Block)
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
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_BLOCKERASE4ADD);
		SPIF_SetAddressPhase(Handle, true, 4, address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, false);


      if (SPIF_Transmit(Handle, tx, 5, 100) == false)
      {
        SPIF_CsPin(Handle, 1);
        break;
      }
    }
    else
    {
		SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_BLOCKERASE3ADD);
		SPIF_SetAddressPhase(Handle, true, 3, address);
		SPIF_SetDummyCycles(Handle, 0);
		SPIF_SetDataPhase(Handle, false);

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


/***********************************************************************************************************/

bool SPIF_FindChip(SPIF_HandleTypeDef *Handle)
{
//  uint8_t tx[4] = {SPIF_CMD_JEDECID, 0xFF, 0xFF, 0xFF};
  uint8_t rx[4];
  bool retVal = false;
  do
  {
    dprintf("SPIF_FindChip()\r\n");
	SPIF_SetInstructionPhase(Handle, true, 1, SPIF_CMD_JEDECID);
	SPIF_SetAddressPhase(Handle, false, 0, 0);
	SPIF_SetDummyCycles(Handle, 3);
	SPIF_SetDataPhase(Handle, false);

    SPIF_CsPin(Handle, 0);
    if (SPIF_Receive(Handle, rx, 4, 100) == false)
    {
      SPIF_CsPin(Handle, 1);
      break;
    }
    SPIF_CsPin(Handle, 1);
    dprintf("CHIP ID: 0x%02X%02X%02X\r\n", rx[1], rx[2], rx[3]);
    Handle->Manufactor = rx[1];
    Handle->MemType = rx[2];
    Handle->Size = rx[3];

    dprintf("SPIF MANUFACTURE: ");
    switch (Handle->Manufactor)
    {
    case SPIF_MANUFACTOR_WINBOND:
      dprintf("WINBOND");
      break;
    case SPIF_MANUFACTOR_SPANSION:
      dprintf("SPANSION");
      break;
    case SPIF_MANUFACTOR_MICRON:
      dprintf("MICRON");
      break;
    case SPIF_MANUFACTOR_MACRONIX:
      dprintf("MACRONIX");
      break;
    case SPIF_MANUFACTOR_ISSI:
      dprintf("ISSI");
      break;
    case SPIF_MANUFACTOR_GIGADEVICE:
      dprintf("GIGADEVICE");
      break;
    case SPIF_MANUFACTOR_AMIC:
      dprintf("AMIC");
      break;
    case SPIF_MANUFACTOR_SST:
      dprintf("SST");
      break;
    case SPIF_MANUFACTOR_HYUNDAI:
      dprintf("HYUNDAI");
      break;
    case SPIF_MANUFACTOR_FUDAN:
      dprintf("FUDAN");
      break;
    case SPIF_MANUFACTOR_ESMT:
      dprintf("ESMT");
      break;
    case SPIF_MANUFACTOR_INTEL:
      dprintf("INTEL");
      break;
    case SPIF_MANUFACTOR_SANYO:
      dprintf("SANYO");
      break;
    case SPIF_MANUFACTOR_FUJITSU:
      dprintf("FUJITSU");
      break;
    case SPIF_MANUFACTOR_EON:
      dprintf("EON");
      break;
    case SPIF_MANUFACTOR_PUYA:
      dprintf("PUYA");
      break;
    default:
      Handle->Manufactor = SPIF_MANUFACTOR_ERROR;
      dprintf("ERROR");
      break;
    }
    dprintf(" - MEMTYPE: 0x%02X", Handle->MemType);
    dprintf(" - SIZE: ");
    switch (Handle->Size)
    {
    case SPIF_SIZE_1MBIT:
      Handle->BlockCnt = 2;
      dprintf("1 MBIT\r\n");
      break;
    case SPIF_SIZE_2MBIT:
      Handle->BlockCnt = 4;
      dprintf("2 MBIT\r\n");
      break;
    case SPIF_SIZE_4MBIT:
      Handle->BlockCnt = 8;
      dprintf("4 MBIT\r\n");
      break;
    case SPIF_SIZE_8MBIT:
      Handle->BlockCnt = 16;
      dprintf("8 MBIT\r\n");
      break;
    case SPIF_SIZE_16MBIT:
      Handle->BlockCnt = 32;
      dprintf("16 MBIT\r\n");
      break;
    case SPIF_SIZE_32MBIT:
      Handle->BlockCnt = 64;
      dprintf("32 MBIT\r\n");
      break;
    case SPIF_SIZE_64MBIT:
      Handle->BlockCnt = 128;
      dprintf("64 MBIT\r\n");
      break;
    case SPIF_SIZE_128MBIT:
      Handle->BlockCnt = 256;
      dprintf("128 MBIT\r\n");
      break;
    case SPIF_SIZE_256MBIT:
      Handle->BlockCnt = 512;
      dprintf("256 MBIT\r\n");
      break;
    case SPIF_SIZE_512MBIT:
      Handle->BlockCnt = 1024;
      dprintf("512 MBIT\r\n");
      break;
    default:
      Handle->Size = SPIF_SIZE_ERROR;
      dprintf("ERROR\r\n");
      break;
    }

    Handle->SectorCnt = Handle->BlockCnt * 16;
    Handle->PageCnt = (Handle->SectorCnt * SPIF_SECTOR_SIZE) / SPIF_PAGE_SIZE;
    dprintf("SPIF BLOCK CNT: %ld\r\n", Handle->BlockCnt);
    dprintf("SPIF SECTOR CNT: %ld\r\n", Handle->SectorCnt);
    dprintf("SPIF PAGE CNT: %ld\r\n", Handle->PageCnt);
    dprintf("SPIF STATUS1: 0x%02X\r\n", SPIF_ReadReg1(Handle));
    dprintf("SPIF STATUS2: 0x%02X\r\n", SPIF_ReadReg2(Handle));
    dprintf("SPIF STATUS3: 0x%02X\r\n", SPIF_ReadReg3(Handle));
    retVal = true;

  } while (0);

  return retVal;
}

#endif

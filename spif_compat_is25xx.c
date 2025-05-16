#include "spif_compat_is25xx.h"
#include "spif.h"


#if (SPIF_COMPAT == SPIF_COMPAT_IS25XX)
/*Register Definitions for IS25XX*/

#define SPIF_DUMMY_BYTE 0xA5 //TODO: confirm value

#define SPIF_CMD_READDATA3ADD 0x03
#define SPIF_CMD_READDATA4ADD 0x13
#define SPIF_CMD_FASTREAD3ADD 0x0B
#define SPIF_CMD_FASTREAD4ADD 0x0C
#define SPIF_CMD_FASTREADDUALIO3ADD 0xBB
#define SPIF_CMD_FASTREADDUALIO4ADD 0xBC
#define SPIF_CMD_FASTREADDUALOUTPUT3ADD 0x3B
#define SPIF_CMD_FASTREADDUALOUTPUT4ADD 0x3C
#define SPIF_CMD_FASTREADQUADIO3ADD 0xEB
#define SPIF_CMD_FASTREADQUADIO4ADD 0xEC
#define SPIF_CMD_FASTREADQUADOUTPUT3ADD 0x6B
#define SPIF_CMD_FASTREADQUADOUTPUT4ADD 0x6C
#define SPIF_CMD_FASTREADDTRIO3ADD 0x0D
#define SPIF_CMD_FASTREADDTRIO4ADD 0x0E
#define SPIF_CMD_FASTREADDTRDUALIO3ADD 0xBD
#define SPIF_CMD_FASTREADDTRDUALIO4ADD 0xBE
#define SPIF_CMD_FASTREADQUADDTRIO3ADD 0xED
#define SPIF_CMD_FASTREADQUADDTRIO4ADD 0xEE
#define SPIF_CMD_PAGEPROG3ADD 0x02
#define SPIF_CMD_PAGEPROG4ADD 0x12
#define SPIF_CMD_QUADPAGEPROGRAM3ADD 0x32
#define SPIF_CMD_QUADPAGEPROGRAM4ADD 0x34
#define SPIF_CMD_SECTORERASE3ADD 0x20
#define SPIF_CMD_SECTORERASE4ADD 0x21
#define SPIF_CMD_BLOCKERASE32K3ADD 0x52
#define SPIF_CMD_BLOCKERASE32K4ADD 0x5C
#define SPIF_CMD_BLOCKERASE64K3ADD 0xD8
#define SPIF_CMD_BLOCKERASE64K4ADD 0xDC
#define SPIF_CMD_CHIPERASE1 0xC7
#define SPIF_CMD_WRITEENABLE 0x06
#define SPIF_CMD_WRITEDISABLE 0x04
#define SPIF_CMD_READSTATUS1 0x05
#define SPIF_CMD_WRITESTATUS1 0x01
#define SPIF_CMD_READFNREG 0x48
#define SPIF_CMD_WRITEFNREG 0x42
#define SPIF_CMD_ENTERQPI 0x35
#define SPIF_CMD_EXITQPI 0xF5
#define SPIF_CMD_SUSPEND 0x75
#define SPIF_CMD_RESUME 0x7A
#define SPIF_CMD_DEEPPOWERDOWN 0xB9
#define SPIF_CMD_READID 0xAB
#define SPIF_CMD_RELEASEPOWERDOWN 0xAB
#define SPIF_CMD_SETREADPARAMNONVOLATILE 0x65
#define SPIF_CMD_SETREADPARAMVOLATILE 0xC0
#define SPIF_CMD_SETEXTENDEDREADPARAMNONVOLATILE 0x85
#define SPIF_CMD_SETEXTEDEDREADPARAMVOLATILE 0x83
#define SPIF_CMD_READREADPARAMVOLATILE 0x61
#define SPIF_CMD_READREADEXTENDEDPARAMVOLATILE 0x81
#define SPIF_CMD_CLEAREXTENDEDREADREG 0x82
#define SPIF_CMD_JEDECID 0x9F
#define SPIF_CMD_READMANUFACTURERID 0x90
#define SPIF_CMD_READJEDECIDQPI 0xAF
#define SPIF_CMD_READUNIQUEID 0x4B
#define SPIF_CMD_READSFDP 0x5A
#define SPIF_CMD_NOOP 0x00
#define SPIF_CMD_SOFTRESETENABLE 0x66
#define SPIF_CMD_SOFTRESET 0x99
#define SPIF_CMD_ERASEINFOROW 0x64
#define SPIF_CMD_PROGRAMINFOROW 0x62
#define SPIF_CMD_READINFOROW 0x68
#define SPIF_CMD_SECTORUNLOCK3ADD 0x26
#define SPIF_CMD_SECTORUNLOCK4ADD 0x25
#define SPIF_CMD_SECTORLOCK 0x24
#define SPIF_CMD_READAUTOBOOTREG 0x14
#define SPIF_CMD_WRITEAUTOBOOTREG 0x15
#define SPIF_CMD_READBANKADDRESSREG 0x16
#define SPIF_CMD_WRITEBANKADDRESSREGVOLATILE 0x17
#define SPIF_CMD_WRITEBANKADDRESSREGNONVOLATILE 0x18
#define SPIF_CMD_ENTER4ADD 0xB7
#define SPIF_CMD_EXIT4ADD 0x29
#define SPIF_CMD_READDYB3ADD 0xFA
#define SPIF_CMD_READDYB4ADD 0xE0
#define SPIF_CMD_WRITEDYB3ADD 0xFB
#define SPIF_CMD_WRITEDYB4ADD 0xE1
#define SPIF_CMD_READPPB3ADD 0xFC
#define SPIF_CMD_READPPB4ADD 0xE2
#define SPIF_CMD_PROGPPB3ADD 0xFD
#define SPIF_CMD_PROGPPB4ADD 0xE3
#define SPIF_CMD_ERASEPPB 0xE4
#define SPIF_CMD_READASP 0x2B
#define SPIF_CMD_PROGRMASP 0x2F
#define SPIF_CMD_READPPBLOCKBIT 0xA7
#define SPIF_CMD_WRITEPPBLOCKBIT 0xA6
#define SPIF_CMD_SETFREEZEBIT 0x91
#define SPIF_CMD_READPASSWORD 0xE7
#define SPIF_CMD_PROGRAMPASSWORD 0xE8
#define SPIF_CMD_UNLOCKPASSWORD 0xE9
#define SPIF_CMD_SETALLDYBBITS 0x7E
#define SPIF_CMD_CLEARALLDYBBITS 0x98

/*status register definitions*/
#define SPIF_STATUS_WIP (1 << 0)
#define SPIF_STATUS_WEL (1 << 1)
#define SPIF_STATUS_BP0 (1 << 2)
#define SPIF_STATUS_BP1 (1 << 3)
#define SPIF_STATUS_BP2 (1 << 4)
#define SPIF_STATUS_BP3 (1 << 5)
#define SPIF_STATUS_QE (1 << 6)
#define SPIF_STATUS_SRWD (1 << 7)

/*Function Register definitions*/
#define SPIF_FNREG_DEDICATEDRESET (1 << 0)
#define SPIF_FNREG_TBS (1 << 1)
#define SPIF_FNREG_PSUS (1 << 2)
#define SPIF_FNREG_ESUS (1 << 3)
#define SPIF_FNREG_IRLOCK0 (1 << 4)
#define SPIF_FNREG_IRLOCK1 (1 << 5)
#define SPIF_FNREG_IRLOCK2 (1 << 6)
#define SPIF_FNREG_IRLOCK3 (1 << 7)

/*Read Register definitons*/
#define SPIF_READREG_BURSTLEN0 (1 << 0)
#define SPIF_READREG_BURSTLEN1 (1 << 1)
#define SPIF_READREG_BURSTLENENABLE (1 << 2)
#define SPIF_READREG_DUMMYCYCLES0 (1 << 3)
#define SPIF_READREG_DUMMYCYCLES1 (1 << 4)
#define SPIF_READREG_DUMMYCYCLES2 (1 << 5)
#define SPIF_READREG_DUMMYCYCLES3 (1 << 6)
#define SPIF_READREG_HOLD_RESET (1 << 7)

/*Extended Read Register definitions*/
#define SPIF_EXTENDEDREADREG_WIP (1 << 0)
#define SPIF_EXTENDEDREADREG_PROT_E (1 << 1)
#define SPIF_EXTENDEDREADREG_P_ERR (1 << 2)
#define SPIF_EXTENDEDREADREG_E_ERR (1 << 3)
// bit 4 is reserved
#define SPIF_EXTENDEDREADREG_ODS0 (1 << 5)
#define SPIF_EXTENDEDREADREG_ODS1 (1 << 6)
#define SPIF_EXTENDEDREADREG_ODS2 (1 << 7)

/* AutoBoot Register definitions */
#define SPIF_AUTOBootREG_ABSA      (0xFFFFFFE0)  // Bits AB[31:5] - AutoBoot Start Address
#define SPIF_AUTOBootREG_ABSD      (0x0000001E)  // Bits AB[4:1]  - AutoBoot Start Delay
#define SPIF_AUTOBootREG_ABE       (1 << 0)      // Bit  AB[0]    - AutoBoot Enable

/* Bank Address Register*/
#define SPIF_BANKADDRESSREG_EXTADD (1 << 7) //3-byte or 4-byte addressing selection Bit
//other bits are all reserved

/* Advanced Sector/Block Protection Register (ASPR) definitions */
// bit 0 is reserved
#define SPIF_ASPR_PSTMLB   (1 << 1)  // Persistent Protection Mode Lock Bit
#define SPIF_ASPR_PWDMLB   (1 << 2)  // Password Protection Mode Lock Bit
//bit 3 to 14 are reserved
#define SPIF_ASPR_TBPARM   (1 << 15) // Top/Bottom Parameter Sector

/*Password Register*/
// TODO: define the password register bits

/*PPB Lock Register*/
#define SPIF_PPBLOCKREG_PPBLOCK (1 << 0) //PPB Lock Bit
//bit 1 to 6 are reserved
#define SPIF_PPBLOCKREG_FREEZE (1 << 7)

/*PPB Register*/
//TODO: confirm if need this definition

/*DYB register*/
//TODO: confirm if need this definition




bool SPIF_WriteEnable(SPIF_HandleTypeDef *Handle)
{
  bool retVal = true;
  uint8_t tx[1] = {SPIF_CMD_WRITEENABLE};
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
    if ((SPIF_ReadReg1(Handle) & SPIF_STATUS_WIP) == 0)

    {
      retVal = true;
      break;
    }
  }
  return retVal;
}


uint8_t SPIF_ReadReg1(SPIF_HandleTypeDef *Handle)
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

bool SPIF_WriteReg1(SPIF_HandleTypeDef *Handle, uint8_t Data)
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


bool SPIF_WriteFn(SPIF_HandleTypeDef *Handle, uint32_t PageNumber, uint8_t *Data, uint32_t Size, uint32_t Offset)
{
  bool retVal = false;
  uint32_t address = 0, maximum = SPIF_PAGE_SIZE - Offset;
  uint8_t tx[5];
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
      tx[0] = SPIF_CMD_PAGEPROG4ADD;
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
      tx[0] = SPIF_CMD_PAGEPROG3ADD;
      tx[1] = (address & 0x00FF0000) >> 16;
      tx[2] = (address & 0x0000FF00) >> 8;
      tx[3] = (address & 0x000000FF);
      if (SPIF_Transmit(Handle, tx, 4, 100) == false)
      {
        SPIF_CsPin(Handle, 1);
        break;
      }
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
  uint8_t tx[5];
  do
  {
#if SPIF_DEBUG != SPIF_DEBUG_DISABLE
    uint32_t dbgTime = HAL_GetTick();
#endif
    dprintf("SPIF_ReadAddress() START ADDRESS %ld\r\n", Address);
    SPIF_CsPin(Handle, 0);
    if (Handle->BlockCnt >= 512)
    {
      tx[0] = SPIF_CMD_READDATA4ADD;
      tx[1] = (Address & 0xFF000000) >> 24;
      tx[2] = (Address & 0x00FF0000) >> 16;
      tx[3] = (Address & 0x0000FF00) >> 8;
      tx[4] = (Address & 0x000000FF);
      if (SPIF_Transmit(Handle, tx, 5, 100) == false)
      {
        SPIF_CsPin(Handle, 1);
        break;
      }
    }
    else
    {
      tx[0] = SPIF_CMD_READDATA3ADD;
      tx[1] = (Address & 0x00FF0000) >> 16;
      tx[2] = (Address & 0x0000FF00) >> 8;
      tx[3] = (Address & 0x000000FF);
      if (SPIF_Transmit(Handle, tx, 4, 100) == false)
      {
        SPIF_CsPin(Handle, 1);
        break;
      }
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
      tx[0] = SPIF_CMD_SECTORERASE4ADD;
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


/*
  * @brief  Write QPI Enable command
  * @note   Send the QPI-Enable command
  * 
  * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
  * @retval bool: true or false
  */
bool SPIF_QPI_Enable(SPIF_HandleTypeDef *Handle)
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
bool SPIF_EraseBlock_64K(SPIF_HandleTypeDef *Handle, uint32_t Block){
	//to be implemented
	__NOP();
	return false;
}

#endif

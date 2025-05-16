#ifndef _SPIF_COMPAT_IS25_H_
#define _SPIF_COMPAT_IS25_H_

#include "spif_util.h"
#include <stdlib.h>


// IS25-specific function prototypes
bool SPIF_WriteEnable(SPIF_HandleTypeDef *Handle);
bool SPIF_WriteDisable(SPIF_HandleTypeDef *Handle);
bool SPIF_WaitForWriting(SPIF_HandleTypeDef *Handle, uint32_t Timeout);
uint8_t SPIF_ReadReg1(SPIF_HandleTypeDef *Handle);
bool SPIF_WriteReg1(SPIF_HandleTypeDef *Handle, uint8_t Data);
bool SPIF_WriteFn(SPIF_HandleTypeDef *Handle, uint32_t PageNumber, uint8_t *Data, uint32_t Size, uint32_t Offset);
bool SPIF_ReadFn(SPIF_HandleTypeDef *Handle, uint32_t Address, uint8_t *Data, uint32_t Size);
bool SPIF_EraseChip(SPIF_HandleTypeDef *Handle);
bool SPIF_EraseSector(SPIF_HandleTypeDef *Handle, uint32_t Sector);
bool SPIF_QPI_Enable(SPIF_HandleTypeDef *Handle);
bool SPIF_EraseBlock(SPIF_HandleTypeDef *Handle, uint32_t Block);
bool SPIF_EraseBlock_64K(SPIF_HandleTypeDef *Handle, uint32_t Block);

#endif

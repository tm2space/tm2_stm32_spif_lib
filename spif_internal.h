/*
 * spif_types.h
 *
 *  Created on: May 15, 2025
 *      Author: Megh Shah
 */

#ifndef INC_SPIF_INTERNAL_H_
#define INC_SPIF_INTERNAL_H_

#include <stdbool.h>
#include <string.h>
#include "stm32u5xx_hal.h"

#include "spif_common.h"


#if SPIF_DEBUG == SPIF_DEBUG_DISABLE
#define dprintf(...)
#else
#include <stdio.h>
#define dprintf(...) printf(__VA_ARGS__)
#endif




/************************************************************************************************************
 **************    Public Definitions
 ************************************************************************************************************/

#define SPIF_PAGE_SIZE                      0x100
#define SPIF_SECTOR_SIZE                    0x1000
#define SPIF_BLOCK_SIZE                     0x10000

#define SPIF_PageToSector(PageNumber)      (((PageNumber) * SPIF_PAGE_SIZE) / SPIF_SECTOR_SIZE)
#define SPIF_PageToBlock(PageNumber)       (((PageNumber) * SPIF_PAGE_SIZE) / SPIF_BLOCK_SIZE)
#define SPIF_SectorToBlock(SectorNumber)   (((SectorNumber) * SPIF_SECTOR_SIZE) / SPIF_BLOCK_SIZE)
#define SPIF_SectorToPage(SectorNumber)    (((SectorNumber) * SPIF_SECTOR_SIZE) / SPIF_PAGE_SIZE)
#define SPIF_BlockToPage(BlockNumber)      (((BlockNumber) * SPIF_BLOCK_SIZE) / SPIF_PAGE_SIZE)
#define SPIF_PageToAddress(PageNumber)     ((PageNumber) * SPIF_PAGE_SIZE)
#define SPIF_SectorToAddress(SectorNumber) ((SectorNumber) * SPIF_SECTOR_SIZE)
#define SPIF_BlockToAddress(BlockNumber)   ((BlockNumber) * SPIF_BLOCK_SIZE)
#define SPIF_AddressToPage(Address)        ((Address) / SPIF_PAGE_SIZE)
#define SPIF_AddressToSector(Address)      ((Address) / SPIF_SECTOR_SIZE)
#define SPIF_AddressToBlock(Address)       ((Address) / SPIF_BLOCK_SIZE)




/*Helper Functions*/
void SPIF_Delay(uint32_t Delay);
void SPIF_Lock(SPIF_HandleTypeDef *Handle);
void SPIF_UnLock(SPIF_HandleTypeDef *Handle);
void SPIF_CsPin(SPIF_HandleTypeDef *Handle, bool Select);


void SPIF_SetInstructionPhase(SPIF_HandleTypeDef* handle, bool enabled, uint8_t size, uint8_t instruction);
void SPIF_SetAddressPhase(SPIF_HandleTypeDef *handle, bool enabled, uint8_t size, uint32_t address);
void SPIF_SetDummyCycles(SPIF_HandleTypeDef *handle, uint8_t cycles);
void SPIF_SetDataPhase(SPIF_HandleTypeDef *handle, bool enabled);

bool SPIF_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx,
		uint8_t *Rx, size_t Size, uint32_t Timeout);
bool SPIF_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size,
		uint32_t Timeout);
bool SPIF_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size,
		uint32_t Timeout);


#endif /* INC_SPIF_INTERNAL_H_ */

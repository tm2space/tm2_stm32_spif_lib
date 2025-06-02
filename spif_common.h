/*
 * spif_common.h
 *
 *  Created on: Jun 2, 2025
 *      Author: Megh Shah
 */

#ifndef INC_SPIF_COMMON_H_
#define INC_SPIF_COMMON_H_

#include <stdbool.h>
#include "stm32u5xx_hal.h"
#include <string.h>




#if SPIF_DEBUG == SPIF_DEBUG_DISABLE
#define dprintf(...)
#else
#include <stdio.h>
#define dprintf(...) printf(__VA_ARGS__)
#endif



/************************************************************************************************************
 **************    Public struct/enum
 ************************************************************************************************************/

typedef enum {
	SPIF_MANUFACTOR_ERROR = 0,
	SPIF_MANUFACTOR_WINBOND = 0xEF,
	SPIF_MANUFACTOR_ISSI = 0xD5,
	SPIF_MANUFACTOR_MICRON = 0x20,
	SPIF_MANUFACTOR_GIGADEVICE = 0xC8,
	SPIF_MANUFACTOR_MACRONIX = 0xC2,
	SPIF_MANUFACTOR_SPANSION = 0x01,
	SPIF_MANUFACTOR_AMIC = 0x37,
	SPIF_MANUFACTOR_SST = 0xBF,
	SPIF_MANUFACTOR_HYUNDAI = 0xAD,
	SPIF_MANUFACTOR_ATMEL = 0x1F,
	SPIF_MANUFACTOR_FUDAN = 0xA1,
	SPIF_MANUFACTOR_ESMT = 0x8C,
	SPIF_MANUFACTOR_INTEL = 0x89,
	SPIF_MANUFACTOR_SANYO = 0x62,
	SPIF_MANUFACTOR_FUJITSU = 0x04,
	SPIF_MANUFACTOR_EON = 0x1C,
	SPIF_MANUFACTOR_PUYA = 0x85,

} SPIF_ManufactorTypeDef;

typedef enum {
	SPIF_SIZE_ERROR = 0,
	SPIF_SIZE_1MBIT = 0x11,
	SPIF_SIZE_2MBIT = 0x12,
	SPIF_SIZE_4MBIT = 0x13,
	SPIF_SIZE_8MBIT = 0x14,
	SPIF_SIZE_16MBIT = 0x15,
	SPIF_SIZE_32MBIT = 0x16,
	SPIF_SIZE_64MBIT = 0x17,
	SPIF_SIZE_128MBIT = 0x18,
	SPIF_SIZE_256MBIT = 0x19,
	SPIF_SIZE_512MBIT = 0x20,

} SPIF_SizeTypeDef;

typedef struct {
    // --- Instruction Phase ---
    bool    instructionEnabled;
    uint8_t instructionSize;
    uint8_t instruction;

    // --- Address Phase ---
    bool    addressEnabled;
    uint8_t addressSize;
    uint32_t address;

    // Dummy Cycles ---
    uint8_t dummyCycles;

    // --- Data Phase ---
    bool    dataEnabled;
} SPIF_EmulatedPhases;

typedef struct {
	void *interface;
	GPIO_TypeDef *Gpio;
	SPIF_ManufactorTypeDef Manufactor;
	SPIF_SizeTypeDef Size;
	uint8_t Inited;
	uint8_t MemType;
	uint8_t Lock;
	uint8_t Reserved;
	uint32_t Pin;
	uint32_t PageCnt;
	uint32_t SectorCnt;
	uint32_t BlockCnt;

	SPIF_EmulatedPhases phase_config;
} SPIF_HandleTypeDef;


#endif /* INC_SPIF_COMMON_H_ */

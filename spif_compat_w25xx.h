
#ifndef _SPIF_COMPAT_W25XX_H_
#define _SPIF_COMPAT_W25XX_H_

#include <stdint.h>
#include "spif.h"

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


// W25XX-specific function prototypes
uint8_t SPIF_W25XX_ReadReg1(SPIF_HandleTypeDef *Handle);
uint8_t SPIF_W25XX_ReadReg2(SPIF_HandleTypeDef *Handle);
uint8_t SPIF_W25XX_ReadReg3(SPIF_HandleTypeDef *Handle);
bool SPIF_W25XX_WriteReg1(SPIF_HandleTypeDef *Handle, uint8_t Data);
bool SPIF_W25XX_WriteReg2(SPIF_HandleTypeDef *Handle, uint8_t Data);
bool SPIF_W25XX_WriteReg3(SPIF_HandleTypeDef *Handle, uint8_t Data);


bool SPIF_W25XX_EraseBlock(SPIF_HandleTypeDef *Handle, uint32_t Block);

#endif

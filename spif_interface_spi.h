#ifndef _SPIF_INTERFACE_SPI_H_
#define _SPIF_INTERFACE_SPI_H_

#include "spif_util.h"
#include "stm32u5xx_hal_spi.h"


bool SPI_SendInstruction(SPIF_HandleTypeDef *Handle, uint32_t Timeout);
bool SPI_SendAddress(SPIF_HandleTypeDef *Handle, uint32_t Timeout);
bool SPI_SendDummyCycles(SPIF_HandleTypeDef *Handle, uint32_t Timeout);

//bool SPIF_Init(SPIF_HandleTypeDef *Handle, SPI_HandleTypeDef *HSpi, GPIO_TypeDef *Gpio, uint16_t Pin);
//bool SPIF_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx, uint8_t *Rx, size_t Size, uint32_t Timeout);
//bool SPIF_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size, uint32_t Timeout);
//bool SPIF_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size, uint32_t Timeout);

#endif

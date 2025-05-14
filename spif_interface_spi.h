#ifndef _SPIF_INTERFACE_SPI_H_
#define _SPIF_INTERFACE_SPI_H_

#include "spif.h"
#include "spi.h"

bool SPIF_SPI_Init(SPIF_HandleTypeDef *Handle, SPI_HandleTypeDef *HSpi, GPIO_TypeDef *Gpio, uint16_t Pin);
bool SPIF_SPI_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx, uint8_t *Rx, size_t Size, uint32_t Timeout);
bool SPIF_SPI_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size, uint32_t Timeout);
bool SPIF_SPI_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size, uint32_t Timeout);

#endif

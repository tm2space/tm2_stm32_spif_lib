#ifndef _SPIF_INTERFACE_OCTOSPI_H_
#define _SPIF_INTERFACE_OCTOSPI_H_

#include "spif.h"
#include <stdint.h>

bool SPIF_OCTOSPI_Init(SPIF_HandleTypeDef *Handle, OSPI_HandleTypeDef *HOspi, GPIO_TypeDef *Gpio, uint16_t Pin);
bool SPIF_OCTOSPI_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx, uint8_t *Rx, size_t Size, uint32_t Timeout);
bool SPIF_OCTOSPI_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size, uint32_t Timeout);
bool SPIF_OCTOSPI_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size, uint32_t Timeout);

#endif

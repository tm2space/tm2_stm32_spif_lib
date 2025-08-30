#include "spif_internal.h"
#include "spif.h"

#if(SPIF_INTERFACE == SPIF_INTERFACE_SPI)


bool SPI_SendInstruction(SPIF_HandleTypeDef *Handle, uint32_t Timeout);
bool SPI_SendAddress(SPIF_HandleTypeDef *Handle, uint32_t Timeout);
bool SPI_SendDummyCycles(SPIF_HandleTypeDef *Handle, uint32_t Timeout);


// Instruction Phase
bool SPI_SendInstruction(SPIF_HandleTypeDef *Handle, uint32_t Timeout) {
	bool retVal = false;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;

	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef*) Handle->interface;
	retVal = HAL_SPI_Transmit(hspi, &pc->instruction, pc->instructionSize,
			Timeout) == HAL_OK;

	return retVal;

}

// Address Phase
bool SPI_SendAddress(SPIF_HandleTypeDef *Handle, uint32_t Timeout) {
	bool retVal = false;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;
	uint32_t Address = pc->address;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef*) Handle->interface;
	uint8_t addrBytes[4] = { 0 };
	for (int i = 0; i < pc->addressSize; i++) {
		addrBytes[i] = (Address >> (8 * i)) & 0xFF;
	}
	retVal = HAL_SPI_Transmit(hspi, addrBytes, pc->addressSize, Timeout)
			== HAL_OK;
	return retVal;
}

// Dummy Cycles Phase
bool SPI_SendDummyCycles(SPIF_HandleTypeDef *Handle, uint32_t Timeout) {
	SPIF_EmulatedPhases *pc = &Handle->phase_config;
	if (pc->dummyCycles > 0) {
		SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef*) Handle->interface;
		uint8_t dummy = 0x00;
		for (int i = 0; i < pc->dummyCycles; i++) {
			if (HAL_SPI_Transmit(hspi, &dummy, 1, Timeout) != HAL_OK) {
				return false;
			}
		}
	}
	return true;
}


bool SPIF_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx, uint8_t *Rx,
		size_t Size, uint32_t Timeout) {
	bool retVal = false;

#if (SPIF_PLATFORM == SPIF_PLATFORM_HAL)
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef*) Handle->interface;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;

	if (pc->instructionEnabled) {
		retVal = SPI_SendInstruction(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->addressEnabled) {
		retVal = SPI_SendAddress(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->dummyCycles > 0) {
		retVal = SPI_SendDummyCycles(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->dataEnabled) {
		if (HAL_SPI_TransmitReceive(hspi, Tx, Rx, Size, Timeout)
				== HAL_OK) {
			retVal = true;
		} else {
			dprintf("SPIF TIMEOUT\r\n");
		}
	}
#elif (SPIF_PLATFORM == SPIF_PLATFORM_HAL_DMA)
    uint32_t startTime = HAL_GetTick();
    if (HAL_SPI_TransmitReceive_DMA(Handle->interface, Tx, Rx, Size) != HAL_OK)
    {
        dprintf("SPIF TRANSFER ERROR\r\n");
    }
    else
    {
        while (1)
        {
            SPIF_Delay(1);
            if (HAL_GetTick() - startTime >= Timeout)
            {
                dprintf("SPIF TIMEOUT\r\n");
                HAL_SPI_DMAStop(Handle->interface);
                break;
            }
            if (HAL_SPI_GetState(Handle->interface) == HAL_SPI_STATE_READY)
            {
                retVal = true;
                break;
            }
        }
    }
#endif
	return retVal;
}

bool SPIF_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size,
		uint32_t Timeout) {

	bool retVal = false;

#if (SPIF_PLATFORM == SPIF_PLATFORM_HAL)

	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef*) Handle->interface;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;

	if (pc->instructionEnabled) {
		retVal = SPI_SendInstruction(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->addressEnabled) {
		retVal = SPI_SendAddress(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->dummyCycles > 0) {
		retVal = SPI_SendDummyCycles(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->dataEnabled) {
		if (HAL_SPI_Transmit(hspi, Tx, Size, Timeout) == HAL_OK) {
			retVal = true;
		} else {
			dprintf("SPIF TIMEOUT\r\n");
		}
	}
#elif (SPIF_PLATFORM == SPIF_PLATFORM_HAL_DMA)
    uint32_t startTime = HAL_GetTick();
    if (HAL_SPI_Transmit_DMA(Handle->interface, Tx, Size) != HAL_OK)
    {
        dprintf("SPIF TRANSFER ERROR\r\n");
    }
    else
    {
        while (1)
        {
            SPIF_Delay(1);
            if (HAL_GetTick() - startTime >= Timeout)
            {
                dprintf("SPIF TIMEOUT\r\n");
                HAL_SPI_DMAStop(Handle->interface);
                break;
            }
            if (HAL_SPI_GetState(Handle->interface) == HAL_SPI_STATE_READY)
            {
                retVal = true;
                break;
            }
        }
    }
#endif
	return retVal;
}

bool SPIF_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size,
		uint32_t Timeout) {

	bool retVal = false;
#if (SPIF_PLATFORM == SPIF_PLATFORM_HAL)
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef*) Handle->interface;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;

	if (pc->instructionEnabled) {
		retVal = SPI_SendInstruction(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->addressEnabled) {
		retVal = SPI_SendAddress(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (pc->dummyCycles > 0) {
		retVal = SPI_SendDummyCycles(Handle, Timeout);
		if (!retVal) {
			return retVal;
		}
	}

	if (HAL_SPI_Receive(hspi, Rx, Size, Timeout) == HAL_OK) {
		retVal = true;
	} else {
		dprintf("SPIF TIMEOUT\r\n");
	}
#elif (SPIF_PLATFORM == SPIF_PLATFORM_HAL_DMA)
    uint32_t startTime = HAL_GetTick();
    if (HAL_SPI_Receive_DMA(Handle->interface, Rx, Size) != HAL_OK)
    {
        dprintf("SPIF TRANSFER ERROR\r\n");
    }
    else
    {
        while (1)
        {
            SPIF_Delay(1);
            if (HAL_GetTick() - startTime >= Timeout)
            {
                dprintf("SPIF TIMEOUT\r\n");
                HAL_SPI_DMAStop(Handle->interface);
                break;
            }
            if (HAL_SPI_GetState(Handle->interface) == HAL_SPI_STATE_READY)
            {
                retVal = true;
                break;
            }
        }
    }
#endif
	return retVal;
}

#endif

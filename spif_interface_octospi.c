#include "spif_interface_octospi.h"
#include "stm32u5xx_hal_ospi.h"
#include "spif.h"

#if (SPIF_INTERFACE == SPIF_INTERFACE_OCTOSPI)

bool SPIF_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx, uint8_t *Rx,
		size_t Size, uint32_t Timeout) {

	bool retVal = false;

	OSPI_RegularCmdTypeDef sCommand = { 0 };
	OSPI_HandleTypeDef *hospi = (OSPI_HandleTypeDef*) Handle->interface;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;

	sCommand.InstructionMode =
			pc->instructionEnabled ?
					HAL_OSPI_INSTRUCTION_4_LINES : HAL_OSPI_INSTRUCTION_NONE;
	sCommand.InstructionSize =
			pc->instructionSize == 2 ?
					HAL_OSPI_INSTRUCTION_16_BITS : HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.Instruction = pc->instruction;

	sCommand.AddressMode =
			pc->addressEnabled ?
					HAL_OSPI_ADDRESS_4_LINES : HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize =
			pc->addressSize == 4 ?
					HAL_OSPI_ADDRESS_32_BITS : HAL_OSPI_ADDRESS_24_BITS;
	sCommand.Address = pc->addressEnabled ? pc->address : 0;

	sCommand.DummyCycles = pc->dummyCycles;

	// Data configuration
	sCommand.DataMode =
			pc->dataEnabled ? HAL_OSPI_DATA_4_LINES : HAL_OSPI_DATA_NONE;
	sCommand.NbData = pc->dataEnabled ? Size : 0;

// Prepare the command (this could be any relevant command, like a read or write)
	if (HAL_OSPI_Command(hospi, &sCommand, Timeout) != HAL_OK) {
		dprintf("SPIF COMMAND ERROR\r\n");
		return false;
	}
	if (HAL_OSPI_TransmitReceive(hospi, Tx, Rx, Size, Timeout)
			== HAL_OK) {
		retVal = true;
	} else {
		dprintf("SPIF TIMEOUT\r\n");
	}

	return retVal;
}

bool SPIF_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size,
		uint32_t Timeout) {

	bool retVal = false;
	OSPI_RegularCmdTypeDef sCommand = { 0 };
	OSPI_HandleTypeDef *hospi = (OSPI_HandleTypeDef*) Handle->interface;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;

	sCommand.InstructionMode =
			pc->instructionEnabled ?
					HAL_OSPI_INSTRUCTION_4_LINES : HAL_OSPI_INSTRUCTION_NONE;
	sCommand.InstructionSize =
			pc->instructionSize == 2 ?
					HAL_OSPI_INSTRUCTION_16_BITS : HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.Instruction = pc->instruction;

	sCommand.AddressMode =
			pc->addressEnabled ?
					HAL_OSPI_ADDRESS_4_LINES : HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize =
			pc->addressSize == 4 ?
					HAL_OSPI_ADDRESS_32_BITS : HAL_OSPI_ADDRESS_24_BITS;
	sCommand.Address = pc->addressEnabled ? pc->address : 0;

	sCommand.DummyCycles = pc->dummyCycles;

	// Data configuration
	sCommand.DataMode =
			pc->dataEnabled ? HAL_OSPI_DATA_4_LINES : HAL_OSPI_DATA_NONE;
	sCommand.NbData = pc->dataEnabled ? Size : 0;

// Send the command to prepare for the transmission
	if (HAL_OSPI_Command(hospi, &sCommand, Timeout) != HAL_OK) {
		dprintf("SPIF COMMAND ERROR\r\n");
		return false;
	}

	if (HAL_OSPI_Transmit(hospi, Tx, Timeout) == HAL_OK) {
		retVal = true;
	} else {
		dprintf("SPIF TIMEOUT\r\n");
	}
	return retVal;

}

bool SPIF_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size,
		uint32_t Timeout) {

	bool retVal = false;
	OSPI_RegularCmdTypeDef sCommand = { 0 };
	OSPI_HandleTypeDef *hospi = (OSPI_HandleTypeDef*) Handle->interface;
	SPIF_EmulatedPhases *pc = &Handle->phase_config;

	sCommand.InstructionMode =
			pc->instructionEnabled ?
					HAL_OSPI_INSTRUCTION_4_LINES : HAL_OSPI_INSTRUCTION_NONE;
	sCommand.InstructionSize =
			pc->instructionSize == 2 ?
					HAL_OSPI_INSTRUCTION_16_BITS : HAL_OSPI_INSTRUCTION_8_BITS;
	sCommand.Instruction = pc->instruction;

	sCommand.AddressMode =
			pc->addressEnabled ?
					HAL_OSPI_ADDRESS_4_LINES : HAL_OSPI_ADDRESS_NONE;
	sCommand.AddressSize =
			pc->addressSize == 4 ?
					HAL_OSPI_ADDRESS_32_BITS : HAL_OSPI_ADDRESS_24_BITS;
	sCommand.Address = pc->addressEnabled ? pc->address : 0;

	sCommand.DummyCycles = pc->dummyCycles;

	// Data configuration
	sCommand.DataMode =
			pc->dataEnabled ? HAL_OSPI_DATA_4_LINES : HAL_OSPI_DATA_NONE;
	sCommand.NbData = pc->dataEnabled ? Size : 0;
	if (HAL_OSPI_Command(hospi, &sCommand, Timeout) != HAL_OK) {
		dprintf("SPIF COMMAND ERROR\r\n");
		return false;
	}

	if (HAL_OSPI_Receive(hospi, Rx, Timeout) == HAL_OK) {
		retVal = true;
	} else {
		dprintf("SPIF TIMEOUT\r\n");
	}
	return retVal;

}

#endif

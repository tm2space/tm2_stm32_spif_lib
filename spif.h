#ifndef _SPIF_H_
#define _SPIF_H_

/***********************************************************************************************************

 Author:     Nima Askari
 Github:     https://www.github.com/NimaLTD
 LinkedIn:   https://www.linkedin.com/in/nimaltd
 Youtube:    https://www.youtube.com/@nimaltd
 Instagram:  https://instagram.com/github.NimaLTD

 Version:    2.3.1

 History:

 2.3.1
 - Fixed SPIF_WriteSector() and SPIF_WriteBlock()

 2.3.0
 - Added ThreadX Configuration

 2.2.2
 - Compile error

 2.2.1
 - Updated SPIF_WriteAddress()


 2.2.0
 - Added SPI_Trasmit and SPI_Receive again :)

 2.1.0
 - Added Support HAL-DMA
 - Removed SPI_Trasmit function

 2.0.1
 - Removed SPI_Receive function

 2.0.0
 - Rewrite again
 - Supported STM32CubeMx Packet installer

 ***********************************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

	/************************************************************************************************************
	 **************    Include Headers
	 ************************************************************************************************************/

#include <stdbool.h>
#include <string.h>
#include "NimaLTD.I-CUBE-SPIF_conf.h"
#include "spif_common.h"

#if (SPIF_INTERFACE == SPIF_INTERFACE_OCTOSPI)
#include "stm32u5xx_hal.h"
#include "stm32u5xx_hal_ospi.h"
#else
#include "stm32u5xx_hal_spi.h"
#endif



#if (SPIF_COMPAT == SPIF_COMPAT_W25XX)
#include "spif_compat_w25xx.h"

#elif (SPIF_COMPAT == SPIF_COMPAT_IS25XX)
#include "spif_compat_is25xx.h"

#endif

	#if (SPIF_PLATFORM == SPIF_INTERFACE_OCTOSPI)
	#include "spif_interface_octospi.h"
#else

#include "spif_interface_spi.h"

#endif



	/************************************************************************************************************
	 **************    Public Functions
	 ************************************************************************************************************/
//#if (SPIF_PLATFORM == SPIF_PLATFORM_OCTOSPI)
//	bool SPIF_OCTOSPI_Init(SPIF_HandleTypeDef *Handle,
//			OSPI_HandleTypeDef *HOspi, GPIO_TypeDef *Gpio, uint16_t Pin);
//#else
//	bool SPIF_Init(SPIF_HandleTypeDef *Handle, SPI_HandleTypeDef *HSpi, GPIO_TypeDef *Gpio, uint16_t Pin);
//#endif

	bool SPIF_WriteAddress(SPIF_HandleTypeDef *Handle, uint32_t Address,
			uint8_t *Data, uint32_t Size);
	bool SPIF_WritePage(SPIF_HandleTypeDef *Handle, uint32_t PageNumber,
			uint8_t *Data, uint32_t Size, uint32_t Offset);
	bool SPIF_WriteSector(SPIF_HandleTypeDef *Handle, uint32_t SectorNumber,
			uint8_t *Data, uint32_t Size, uint32_t Offset);
	bool SPIF_WriteBlock(SPIF_HandleTypeDef *Handle, uint32_t BlockNumber,
			uint8_t *Data, uint32_t Size, uint32_t Offset);

	bool SPIF_ReadAddress(SPIF_HandleTypeDef *Handle, uint32_t Address,
			uint8_t *Data, uint32_t Size);
	bool SPIF_ReadPage(SPIF_HandleTypeDef *Handle, uint32_t PageNumber,
			uint8_t *Data, uint32_t Size, uint32_t Offset);
	bool SPIF_ReadSector(SPIF_HandleTypeDef *Handle, uint32_t SectorNumber,
			uint8_t *Data, uint32_t Size, uint32_t Offset);
	bool SPIF_ReadBlock(SPIF_HandleTypeDef *Handle, uint32_t BlockNumber,
			uint8_t *Data, uint32_t Size, uint32_t Offset);

	bool SPIF_SendCmd(SPIF_HandleTypeDef *Handle, uint8_t Cmd, uint8_t *Data,
			uint32_t Size);
	bool SPIF_SendCmdReceive(SPIF_HandleTypeDef *Handle, uint8_t Cmd,
			uint8_t *RxData, uint32_t Size);

#if (SPIF_COMPAT == SPIF_COMPAT_IS25XX)
	bool SPIF_QPI_Enable(SPIF_HandleTypeDef *Handle);
#endif

#ifdef __cplusplus
}
#endif
#endif

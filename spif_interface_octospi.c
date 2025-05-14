#include "spif_interface_octospi.h"
#include "spif.h"
#include "ospi.h"
#include "NimaLTD.I-CUBE-SPIF_conf.h"

/**
 * @brief  Initialize the SPIF.
 * @note   Enable and configure the SPI and Set GPIO as output for CS pin on the CubeMX
 *
 * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
 * @param  *HOSpi: Pointer to a SPI_HandleTypeDef structure
 * @param  *Gpio: Pointer to a GPIO_TypeDef structure for CS
 * @param  Pin: Pin of CS
 *
 * @retval bool: true or false
 */
bool SPIF_OCTOSPI_Init(SPIF_HandleTypeDef *Handle, OSPI_HandleTypeDef *HOspi, GPIO_TypeDef *Gpio, uint16_t Pin){
    
   bool retVal = false;
   do
   {
     if ((Handle == NULL) || (HOspi == NULL) || (Gpio == NULL) || (Handle->Inited == 1))
     {
       dprintf("SPIF_Init() Error, Wrong Parameter\r\n");
       break;
     }
     memset(Handle, 0, sizeof(SPIF_HandleTypeDef));
     Handle->interface = HOspi;
     Handle->Gpio = Gpio;
     Handle->Pin = Pin;
     SPIF_CsPin(Handle, 1);
     /* wait for stable VCC */
     while (HAL_GetTick() < 20)
     {
       SPIF_Delay(1);
     }
     if (SPIF_WriteDisable(Handle) == false)
     {
       break;
     }
     retVal = SPIF_FindChip(Handle);
     if (retVal)
     {
       Handle->Inited = 1;
       dprintf("SPIF_Init() Done\r\n");
     }
 
   } while (0);
 
   return retVal;
}


bool SPIF_OCTOSPI_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx, uint8_t *Rx, size_t Size, uint32_t Timeout){
    
  bool retVal = false;
OSPI_RegularCmdTypeDef sCommand = {0};

// Set up the command to use 4 data lines
sCommand.OperationType   = HAL_OSPI_OPTYPE_COMMON_CFG;
sCommand.DataMode        = HAL_OSPI_DATA_4_LINES;  // 4-line data mode
sCommand.NbData          = Size;  // Set the number of data bytes to transmit/receive

// Prepare the command (this could be any relevant command, like a read or write)
if (HAL_OSPI_Command(Handle->interface, &sCommand, Timeout) != HAL_OK)
{
    dprintf("SPIF COMMAND ERROR\r\n");
    return false;
}
  if (HAL_OSPI_TransmitReceive(Handle->interface, Tx, Rx, Size, Timeout) == HAL_OK)
  {
    retVal = true;
  }
  else
  {
    dprintf("SPIF TIMEOUT\r\n");
  }

  return retVal;
}


bool SPIF_OCTOSPI_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size, uint32_t Timeout){
    
  bool retVal = false;
OSPI_RegularCmdTypeDef sCommand = {0};
sCommand.OperationType = HAL_OSPI_OPTYPE_COMMON_CFG;
sCommand.DataMode = HAL_OSPI_DATA_4_LINES;  // Set to 4 data lines
sCommand.NbData = Size;

// Send the command to prepare for the transmission
if (HAL_OSPI_Command(Handle->interface, &sCommand, Timeout) != HAL_OK)
{
    dprintf("SPIF COMMAND ERROR\r\n");
    return false;
}

  if (HAL_OSPI_Transmit(Handle->interface, Tx, Timeout) == HAL_OK)
  {
    retVal = true;
  }
  else
  {
    dprintf("SPIF TIMEOUT\r\n");
  }
  return retVal;

}

bool SPIF_OCTOSPI_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size, uint32_t Timeout){
    
  bool retVal = false;
  OSPI_RegularCmdTypeDef sCommand = {0};

  // Set up the command for receiving data with 4 data lines
  sCommand.OperationType   = HAL_OSPI_OPTYPE_COMMON_CFG;
  sCommand.DataMode        = HAL_OSPI_DATA_4_LINES;  // Set to 4-line data mode
  sCommand.NbData          = Size;  // Number of data bytes to receive

  if (HAL_OSPI_Command(Handle->interface, &sCommand, Timeout) != HAL_OK)
  {
    dprintf("SPIF COMMAND ERROR\r\n");
    return false;
  }

  if (HAL_OSPI_Receive(Handle->interface, Rx,  Timeout) == HAL_OK)
  {
    retVal = true;
  }
  else
  {
    dprintf("SPIF TIMEOUT\r\n");
  } 
  return retVal;

}

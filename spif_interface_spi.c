
#include "spif_interface_spi.h"
#include "spif.h"

#if(SPIF_INTERFACE == SPIF_INTERFACE_SPI)

/**
 * @brief  Initialize the SPIF.
 * @note   Enable and configure the SPI and Set GPIO as output for CS pin on the CubeMX
 *
 * @param  *Handle: Pointer to SPIF_HandleTypeDef structure
 * @param  *HSpi: Pointer to a SPI_HandleTypeDef structure
 * @param  *Gpio: Pointer to a GPIO_TypeDef structure for CS
 * @param  Pin: Pin of CS
 *
 * @retval bool: true or false
 */
//bool SPIF_Init(SPIF_HandleTypeDef *Handle, SPI_HandleTypeDef *HSpi, GPIO_TypeDef *Gpio, uint16_t Pin)
//{
//    bool retVal = false;
//    do
//    {
//        if ((Handle == NULL) || (HSpi == NULL) || (Gpio == NULL) || (Handle->Inited == 1))
//        {
//            dprintf("SPIF_Init() Error, Wrong Parameter\r\n");
//            break;
//        }
//        memset(Handle, 0, sizeof(SPIF_HandleTypeDef));
//        Handle->interface = HSpi;
//        Handle->Gpio = Gpio;
//        Handle->Pin = Pin;
//        SPIF_CsPin(Handle, 1);
//        /* wait for stable VCC */
//        while (HAL_GetTick() < 20)
//        {
//            SPIF_Delay(1);
//        }
//        if (SPIF_WriteDisable(Handle) == false)
//        {
//            break;
//        }
//        retVal = SPIF_FindChip(Handle);
//        if (retVal)
//        {
//            Handle->Inited = 1;
//            dprintf("SPIF_Init() Done\r\n");
//        }
//
//    } while (0);
//
//    return retVal;
//}

bool SPIF_TransmitReceive(SPIF_HandleTypeDef *Handle, uint8_t *Tx, uint8_t *Rx, size_t Size, uint32_t Timeout)
{
    bool retVal = false;
#if (SPIF_PLATFORM == SPIF_PLATFORM_HAL)
    if (HAL_SPI_TransmitReceive(Handle->interface, Tx, Rx, Size, Timeout) == HAL_OK)
    {
        retVal = true;
    }
    else
    {
        dprintf("SPIF TIMEOUT\r\n");
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

bool SPIF_Transmit(SPIF_HandleTypeDef *Handle, uint8_t *Tx, size_t Size, uint32_t Timeout)
{

    bool retVal = false;
#if (SPIF_PLATFORM == SPIF_PLATFORM_HAL)
    if (HAL_SPI_Transmit(Handle->interface, Tx, Size, Timeout) == HAL_OK)
    {
        retVal = true;
    }
    else
    {
        dprintf("SPIF TIMEOUT\r\n");
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

bool SPIF_Receive(SPIF_HandleTypeDef *Handle, uint8_t *Rx, size_t Size, uint32_t Timeout)
{

    bool retVal = false;
#if (SPIF_PLATFORM == SPIF_PLATFORM_HAL)
    if (HAL_SPI_Receive(Handle->interface, Rx, Size, Timeout) == HAL_OK)
    {
        retVal = true;
    }
    else
    {
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

#ifndef PHBALREG_RPI_SPI_H_
#define PHBALREG_RPI_SPI_H_
#include <ph_Status.h>

phStatus_t phbalReg_RPi_spi_Init(
		phbalReg_RPi_spi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
		uint16_t wSizeOfDataParams                /**< [In] Specifies the size of the data parameter structure. */
        );

phStatus_t phbalReg_RPi_spi_GetPortList(
	phbalReg_RPi_spi_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    );

phStatus_t phbalReg_RPi_spi_SetPort(
							phbalReg_RPi_spi_DataParams_t * pDataParams,
                            uint8_t * pPortName
                            );

phStatus_t phbalReg_RPi_spi_OpenPort(
								phbalReg_RPi_spi_DataParams_t * pDataParams
                                  );

phStatus_t phbalReg_RPi_spi_ClosePort(
								phbalReg_RPi_spi_DataParams_t * pDataParams
                                   );

phStatus_t phbalReg_RPi_spi_Exchange(
								phbalReg_RPi_spi_DataParams_t * pDataParams,
                                uint16_t wOption,
                                uint8_t * pTxBuffer,
                                uint16_t wTxLength,
                                uint16_t wRxBufSize,
                                uint8_t * pRxBuffer,
                                uint16_t * pRxLength
                                  );

phStatus_t phbalReg_RPi_spi_SetConfig(
									phbalReg_RPi_spi_DataParams_t * pDataParams,
									uint16_t wConfig,
									uint16_t wValue
                                   );

phStatus_t phbalReg_RPi_spi_GetConfig(
									phbalReg_RPi_spi_DataParams_t * pDataParams,
									uint16_t wConfig,
									uint16_t * pValue
                                   );

#endif /* PHBALREG_R_PI_SPI_H_ */

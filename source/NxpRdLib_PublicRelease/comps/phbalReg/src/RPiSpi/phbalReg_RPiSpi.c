#include <ph_Status.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHBAL_REG_RPISPI

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

#include "phbalReg_RPiSpi.h"

/*
 * piBoardRev:
 *	Return a number representing the hardware revision of the board.
 *	Revision is currently 1 or 2. -1 is returned on error.
 *
 *	Much confusion here )-:
 *	Seems there ar esome boards with 0000 in them (mistake in manufacture)
 *	and some board with 0005 in them (another mistake in manufacture).
 *	So the distinction between boards that I can see is:
 *	0000 - Error
 *	0001 - Not used
 *	0002 - Rev 1
 *	0003 - Rev 1
 *	0004 - Rev 2
 *	0005 - Rev 2
 *	0006 - Rev 2
 *	000f - Rev 2 + 512MB
 *
 *	A small thorn is the olde style overvolting - that will add in
 *		1000000
 *
 *this function was taken from https://github.com/WiringPi/WiringPi
 *********************************************************************************
 */

/*============================================================================================*/
static int piBoardRev (void)
    {
    static int  boardRev = -1 ;
    FILE *cpuFd ;
    char line [120] ;
    char *c, lastChar ;

    // No point checking twice...
#ifdef    NXPBUILD__PH_DEBUG
    printf("Enter: piBoardRev\n");
#endif // NXPBUILD__PH_DEBUG

    if (boardRev != -1)
        {
#ifdef    NXPBUILD__PH_DEBUG
        printf("if(boardRev != -1)\n");
#endif   // NXPBUILD__PH_DEBUG
        return boardRev ;
        }

    if ((cpuFd = fopen ("/proc/cpuinfo", "r")) == NULL)
        {
        printf("if(cpuFd = fopen)\n");
        return -1 ;
        }

    while (fgets (line, 120, cpuFd) != NULL)
        if (strncmp (line, "Revision", 8) == 0)
            break ;

    fclose (cpuFd) ;

    if (line == NULL)
        {
        errno = 0 ;
        printf("if(line == NULL)\n");
        return -1 ;
        }

    for (c = line ; *c ; ++c)
        {
        if (isdigit (*c))
            {
            break ;
            }
        }

    if (!isdigit (*c))
        {
        errno = 0 ;
        printf("!isdigit (*c))\n");
        return -1 ;
        }

    lastChar = c [strlen (c) - 2] ;

    if ((lastChar == '2') || (lastChar == '3'))
        {
        boardRev = 1 ;
        }
    else
        {
        boardRev = 2 ;
        }
#ifdef    NXPBUILD__PH_DEBUG
    printf ("Board Revision:\n"
            " - string:     %s"
            " - board rev.: %d\n", c, boardRev) ;
#endif   // NXPBUILD__PH_DEBUG
    return boardRev ;
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_Init(
        phbalReg_RPi_spi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wSizeOfDataParams                /**< [In] Specifies the size of the data parameter structure. */
)
    {
    int ifsel_fd;
    uint8_t data[5];
    int boardRev;

    if (sizeof(phbalReg_RPi_spi_DataParams_t) != wSizeOfDataParams)
        {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }
    PH_ASSERT_NULL (pDataParams);

    pDataParams->wId = PH_COMP_BAL | PHBAL_REG_RPISPI_ID;

    boardRev = piBoardRev();

    ifsel_fd = open("/sys/class/gpio/export",O_WRONLY);
    if (ifsel_fd == 0)
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);

    if (boardRev == 1)
        {
        sprintf(data,"0");	/* */
        }
    else
        {
        sprintf(data,"2");	/* */
        }
    write(ifsel_fd,data,1);
    close(ifsel_fd);

    if (boardRev == 1)
        {
        ifsel_fd = open("/sys/class/gpio/gpio0/direction",O_WRONLY);
        }
    else
        {
        ifsel_fd = open("/sys/class/gpio/gpio2/direction",O_WRONLY);
        }

    if (ifsel_fd == 0)
        {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }

    sprintf(data,"in");
    write(ifsel_fd,data,2);
    close(ifsel_fd);

#ifdef NXPBUILD__PHHAL_HW_PN512
    ifsel_fd = open("/sys/class/gpio/export",O_WRONLY);
    if (ifsel_fd == 0)
        {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }

    /*** SET the RESET pin ***/

    sprintf(data,"7");  /* 512_NRSTPD */
    write(ifsel_fd,data,1);
    close(ifsel_fd);


    /* 512_NRSTPD - output */
    ifsel_fd = open("/sys/class/gpio/gpio7/direction",O_WRONLY);
    if (ifsel_fd == 0)
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
    sprintf(data,"out");
    write(ifsel_fd,data,3);
    close(ifsel_fd);

    /*** generate the RESET sequence "1"-(wait1.2-"0"-"1" ***/

    /* 512_NRSTPD - 1 */
    ifsel_fd = open("/sys/class/gpio/gpio7/value",O_WRONLY);

    if (ifsel_fd == 0)
        {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }
    sprintf(data,"1");
    write(ifsel_fd,data,1);

    usleep(1500);

    /* 512_NRSTPD - 0 */
    //   ifsel_fd = open("/sys/class/gpio/gpio7/value",O_WRONLY);
    if (ifsel_fd == 0)
        {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }
    sprintf(data,"0");
    write(ifsel_fd,data,1);

    usleep(1500);

    /* 512_NRSTPD - 1 */
    //   ifsel_fd = open("/sys/class/gpio/gpio7/value",O_WRONLY);
    if (ifsel_fd == 0)
        {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }
    sprintf(data,"1");
    write(ifsel_fd,data,1);
    close(ifsel_fd);

#endif /* NXPBUILD__PHHAL_HW_PN512 */

    pDataParams->spiMode	= 0 ;
    pDataParams->spiBPW		= 8 ;
    pDataParams->spiDelay	= 0;
    pDataParams->spiSpeed	= 2000000;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_GetPortList(
        phbalReg_RPi_spi_DataParams_t * pDataParams,
        uint16_t wPortBufSize,
        uint8_t * pPortNames,
        uint16_t * pNumOfPorts
)
    {
    wPortBufSize = 0;
    pPortNames = NULL;
    *pNumOfPorts = 0;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_SetPort(
        phbalReg_RPi_spi_DataParams_t * pDataParams,
        uint8_t * pPortName
)
    {
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_OpenPort(
        phbalReg_RPi_spi_DataParams_t * pDataParams
)
    {
#ifdef    NXPBUILD__PH_DEBUG
    printf("Enter: phbalReg_R_Pi_spi_OpenPort\n");
#endif // NXPBUILD__PH_DEBUG

    pDataParams->spiFD = open ("/dev/spidev0.0", O_RDWR);
    if (pDataParams->spiFD == 0)
        {
        printf("phbalReg_R_Pi_spi_OpenPort: if (pDataParams->spiFD == 0)\n");
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }

    if (ioctl (pDataParams->spiFD, SPI_IOC_WR_MODE, &pDataParams->spiMode)         < 0)
        {
        close(pDataParams->spiFD);
        printf("phbalReg_R_Pi_spi_OpenPort: if (ioctl (pDataParams->spiFD, SPI_IOC_WR_MODE, &pDataParams->spiMode)\n");
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }
    if (ioctl (pDataParams->spiFD, SPI_IOC_WR_BITS_PER_WORD, &pDataParams->spiBPW) < 0)
        {
        close(pDataParams->spiFD);
        printf("phbalReg_R_Pi_spi_OpenPort: if (ioctl (pDataParams->spiFD, SPI_IOC_WR_BITS_PER_WORD, &pDataParams->spiBPW)\n");
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }
    if (ioctl (pDataParams->spiFD, SPI_IOC_WR_MAX_SPEED_HZ, &pDataParams->spiSpeed)   < 0)
        {
        close(pDataParams->spiFD);
        printf("phbalReg_R_Pi_spi_OpenPort: if (ioctl (pDataParams->spiFD, SPI_IOC_WR_MAX_SPEED_HZ, &pDataParams->spiSpeed)\n");
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
        }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_ClosePort(
        phbalReg_RPi_spi_DataParams_t * pDataParams
)
    {
    close(pDataParams->spiFD);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_Exchange(
        phbalReg_RPi_spi_DataParams_t * pDataParams,
        uint16_t wOption,
        uint8_t * pTxBuffer,
        uint16_t wTxLength,
        uint16_t wRxBufSize,
        uint8_t * pRxBuffer,
        uint16_t * pRxLength
)
    {
    struct spi_ioc_transfer spi ;


    spi.tx_buf        = (unsigned long)pTxBuffer ;
    spi.rx_buf        = (unsigned long)pTxBuffer ;
    spi.len           = wTxLength ;
    spi.delay_usecs   = pDataParams->spiDelay ;
    spi.speed_hz      = pDataParams->spiSpeed;
    spi.bits_per_word = pDataParams->spiBPW ;

    int i;

    *pRxLength = ioctl (pDataParams->spiFD, SPI_IOC_MESSAGE(1), &spi) ;
    if (*pRxLength <= 0 )
        {
        printf("read write error\n");
        return PH_ADD_COMPCODE(PH_ERR_READ_WRITE_ERROR, PH_COMP_BAL);
        }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_SetConfig(
        phbalReg_RPi_spi_DataParams_t * pDataParams,
        uint16_t wConfig,
        uint16_t wValue
)
    {
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

/*============================================================================================*/
phStatus_t phbalReg_RPi_spi_GetConfig(
        phbalReg_RPi_spi_DataParams_t * pDataParams,
        uint16_t wConfig,
        uint16_t * pValue
)
    {
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
    }

#endif // NXPBUILD__PHBAL_REG_RPISPI

/*==============================================================================================
 *         Copyright (c), NXP Semiconductors Gratkorn / Austria
 *
 *                     (C)NXP Semiconductors
 *       All rights are reserved. Reproduction in whole or in part is
 *      prohibited without the written consent of the copyright owner.
 *  NXP reserves the right to make changes without notice at any time.
 * NXP makes no warranty, expressed, implied or statutory, including but
 * not limited to any implied warranty of merchantability or fitness for any
 *particular purpose, or that the use will not infringe any third party patent,
 * copyright or trademark. NXP must not be liable for any loss or damage
 *                          arising from its use.
 */

/*==============================================================================================
 * file
 *
 * This is the code example of the application which demonstrates the possibility to write
 * a message from an Android device to emulated tag via NFC technology and consequence
 * to write this message to the Facebook account via the Java application and everything
 * is performed on Linux operating system.
 * The application connects the NXP reader library with supported T2T card emulation,
 * the embedded hardware presented by Raspberry Pi board, Linux operation system and java
 * application which handles the Facebook account access.
 * Like card reader is used the NXP PNEV512R card reader evaluation board in versions: v1.4 or
 * v1.5
 * The maximum length of posted message is 2016 bytes or characters.
 *
 * Author: Gratkorn development team
 * Revision: 1.0
 * Date: September 2013
 *
 * History:
 *  v1.0    initial version
 *
 */

/*----------------------------------------------------------------------------------------------
 * Includes
 ---------------------------------------------------------------------------------------------*/
#include <ph_Status.h>
#include <phhalHw.h>
#include <phOsal.h>
#include <phCardEmu.h>
#include <phCardEmu_Options.h>
#include <phbalReg_RPiSpi.h>

#ifdef  PHCARDEMU_T2T_MSG
#include <CheckNdef.h>
#endif  // PHCARDEMU_T2T_MSG

#include <stdio.h>

/*----------------------------------------------------------------------------------------------
 * Local macros and definitions
 ---------------------------------------------------------------------------------------------*/
/* size of communication buffers (required for HAL)*/
#define INPUT_BUFF_MAX     256      // for T4T according to FSCI in phpalI14443p4C.h
#define OUTPUT_BUFF_MAX    256
#define FB_BUF_MAX         2048

#define CHECK_STATUS(status) if (PH_ERR_SUCCESS != status) error (status, __LINE__);

/*----------------------------------------------------------------------------------------------
 * Local variables
 ---------------------------------------------------------------------------------------------*/
/* communication buffers for HAL */
uint8_t bTxBuff[OUTPUT_BUFF_MAX+1];
uint8_t bRxBuff[INPUT_BUFF_MAX+1];
uint8_t bTimerExpired;

phbalReg_RPi_spi_DataParams_t   bal;            /* BAL component holder */
phhalHw_Pn512_DataParams_t      hal;            /* HAL component holder */
phOsal_RPi_DataParams_t         osal;           /* OSAL component holder for RaspberryPi */

const uint8_t JAVA_CMD[]="java -jar fbPoster.jar \""; /* calling the java application */
uint8_t bTempBuff[FB_BUF_MAX];

/*----------------------------------------------------------------------------------------------
/* Functions
 ---------------------------------------------------------------------------------------------*/
/*============================================================================================*/
/* gets called in case of error */
static void error(phStatus_t status, int line)
    {
    // there was an error

    volatile phStatus_t s;  /* to prevent optimizations */
    volatile int l;         /* to prevent optimizations */

    s = status;
    l = line;

    while (1) {};
    }

/*============================================================================================*/
void RFAntiColl_TimerCallback(uint32_t timer_id, void * context)
    {
    bTimerExpired = 1;
    return;
    }


/*==============================================================================================
 *   M A I N
 ---------------------------------------------------------------------------------------------*/
int main( int argc, char * argv[] )
    {
    uint8_t    * pRxBuffer;
    uint8_t    * chp;
    uint8_t    statusT2T;
    uint8_t    sak;
    uint16_t   wRxLength;
    uint32_t   status_1;
    uint32_t   dwJavaMsgLen;
    uint8_t    bValue;
    phStatus_t status;

#ifdef    NXPBUILD__PH_DEBUG
    printf("\n\nSTART\n");
    printf("--------------------------\n");
#endif   // NXPBUILD__PH_DEBUG

    chp = (uint8_t *)dirname(argv[0]);
    chdir(chp);

    /* BEGIN: HW INITIALIZATION */

    /* Initialize the Reader BAL (Bus Abstraction Layer) component */
#ifdef    NXPBUILD__PH_DEBUG
    printf("SPI init\n");
    printf("--------------------------\n");
    printf("Enter: phbalReg_R_Pi_spi_Init\n");
#endif // NXPBUILD__PH_DEBUG

    status = phbalReg_RPi_spi_Init(&bal, sizeof(phbalReg_RPi_spi_DataParams_t));


    if (PH_ERR_SUCCESS != status)
	{
		printf("Failed to initialize SPI\n");
		return 1;
	}
#ifdef    NXPBUILD__PH_DEBUG
    else
	{
		printf("OK - phbalReg_R_Pi_spi_Init\n");
		printf("wId:        %0x\n",bal.wId);
		printf("spiFD:      %0x\n",bal.spiFD);
		printf("spiMode:    %0x\n",bal.spiMode);
		printf("spiBPW:     %0x\n",bal.spiBPW);
		printf("spiDelay:   %0x\n",bal.spiDelay);
		printf("spiSpeed:   %0x\n",bal.spiSpeed);
		printf("   (MHz):   %0d\n",bal.spiSpeed/1000000);
	}
    printf("--------------------------\n");
#endif // NXPBUILD__PH_DEBUG

    /* set up GPIO and SSP port - see underlying functions for more information */
    status = phbalReg_OpenPort(&bal);
    if (PH_ERR_SUCCESS != status)
        {
        printf("Failed to open bal\n");
        return 2;
        }

    /* init HAL for PN512 (PN512 is derivative of PN512) */
    status = phhalHw_Pn512_Init(&hal,
                                sizeof(phhalHw_Pn512_DataParams_t),
                                &bal,
                                NULL,
                                &osal,
                                &bTxBuff[0],
                                OUTPUT_BUFF_MAX+1,
                                &bRxBuff[0],
                                INPUT_BUFF_MAX+1);
    CHECK_STATUS(status);

    /* Set the HAL configuration to SPI */
    status = phhalHw_SetConfig(&hal, PHHAL_HW_CONFIG_BAL_CONNECTION, PHHAL_HW_BAL_CONNECTION_SPI);
    if (PH_ERR_SUCCESS != status)
        {
        printf("Failed to set hal connection SPI\n");
        return 4;
        }

    /* It is necessary to soft-reset the PN512 here. */
#ifdef    NXPBUILD__PH_DEBUG
    printf("--------------------------\n");
    printf("field reset\n");
    printf("--------------------------\n");
#endif   // NXPBUILD__PH_DEBUG
    status = phhalHw_Pn512_Cmd_SoftReset(&hal);
    CHECK_STATUS(status);

#ifdef    NXPBUILD__PH_DEBUG
    /* Read the version of the reader IC */
    status = phhalHw_ReadRegister(&hal, PHHAL_HW_PN512_REG_VERSION, &bValue);
    printf("Chip version: %0x\n",bValue);
    printf("--------------------------\n");
#endif   // NXPBUILD__PH_DEBUG

    /* Initialize the timer component */
    status = phOsal_RPi_Timer_Init(&osal);
    CHECK_STATUS(status);

    /* Initialize OSAL layer */
    status = phOsal_RPi_Init(&osal);
    CHECK_STATUS(status);

#ifdef    NXPBUILD__PH_DEBUG
    printf("HW INIT end\n");
    printf("--------------------------\n");
#endif   // NXPBUILD__PH_DEBUG

    /* END: HW INITIALIZATION */

    /* Card emu. initialization - card emulation is presented like MIFARE Ultralight card */
    sak = 0x00;
    phCardEmu_Init(&hal, &osal, sak);

    /* Initialize the T2T (Type 2-Tag) */
    statusT2T = phCardEmu_T2T_Init();

    /* Set the communication protocol */
    status = phhalHw_ApplyProtocolSettings(&hal, PHHAL_HW_CARDTYPE_ISO14443A_CE);
    CHECK_STATUS(status);

    printf("Waiting for the message to post.\n");


    uint8_t bndefstatus;

    while(1)
        {
        /* Card emulation activation is required */
        if(phCardEmu_Activate(&pRxBuffer, &wRxLength) != PH_ERR_SUCCESS)
            {
            continue;
            }

        if(statusT2T)
            {
            /* T2T proper operations*/
            phCardEmu_T2T_Start(pRxBuffer, wRxLength);

#ifdef  PHCARDEMU_T2T_MSG
            /* Processing the NDEF message and writing message to the Facebook */
            if( CheckNdef( t2tMemory, &NdefDesc ) == NDEF_TEXT_WRITE )
                {
                printf("Passing the message \"%s\" to the Java application.\n", NdefDesc.textstart);
                fflush(stdout);
                /* complete whole command line and NDEF extracts */
                // take length of command string
                dwJavaMsgLen = strlen(JAVA_CMD);
                // coppy to temp buffer
                memcpy(bTempBuff, JAVA_CMD, dwJavaMsgLen);
                // copy the NDEF message behind the command string to the temp buffer
                memcpy(&bTempBuff[dwJavaMsgLen], NdefDesc.textstart, NdefDesc.textlen);
                // add two termination characters
                bTempBuff[dwJavaMsgLen+NdefDesc.textlen] = '"';
                bTempBuff[dwJavaMsgLen+NdefDesc.textlen+1] = 0;
                // call the Java application from linux operation system
                system(bTempBuff);
                printf("\nNext message please!\n\n");
                }
#endif  // PHCARDEMU_T2T_MSG
            }
        }
    return 0;
    }

/*==============================================================================================
 *   End of File
 ---------------------------------------------------------------------------------------------*/


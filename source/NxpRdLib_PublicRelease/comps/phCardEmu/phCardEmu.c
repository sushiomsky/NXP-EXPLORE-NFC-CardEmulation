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
 * $Author: prabakaran.c $
 * $Revision: 1.7 $
 * $Date: Tue Dec  4 13:45:35 2012 $
 *
 * History:
 *  PC: Generated 25. Nov 2012
 *
 */

/*----------------------------------------------------------------------------------------------
 * Includes
 ---------------------------------------------------------------------------------------------*/
#include <phhalHw.h>
#include <phOsal.h>
#include <phCardEmu.h>
#include <phCardEmu_Options.h>
#include <phceT2TCmdHdl.h>

#if defined (PHCARDEMU_T2T_PHDC)
#include <PHDC.h>
#include <PHDC_NDEFhelper.h>
#endif

#include <stdlib.h>

/*----------------------------------------------------------------------------------------------
 * Local macros and definitions
 ---------------------------------------------------------------------------------------------*/
#define CHECK_STATUS(status) if (PH_ERR_SUCCESS != status) error (status, __LINE__);

/*----------------------------------------------------------------------------------------------
 * Local constants and variables
 ---------------------------------------------------------------------------------------------*/
static uint8_t sens_res[2]  = { 0x04, 0x00 };        /* ATQ bytes - needed for anti-collision */
static uint8_t nfc_id1[3]   = { 0xA1, 0xA2, 0xA3 };  /* user defined bytes of the UID (one is hardcoded) - needed for anti-collision */
static uint8_t nfc_id3      = 0xFA;                  /* NFC3 byte - needed for anti-collision */
static uint8_t poll_res[18] = {
                                0x01, 0xFE, 0xB2, 0xB3,
                                0xB4, 0xB5, 0xB6, 0xB7,
                                0xC0, 0xC1, 0xC2, 0xC3,
                                0xC4, 0xC5, 0xC6, 0xC7,
                                0x23, 0x45 };        /* felica - needed for anti-collision */

void * pHal;
void * pOsal;
phceT2T_Sw_DataParams_t		cet2t;
uint8_t * t2tMemory = NULL;
uint16_t	num_wr = 0;		// num of write
uint16_t	num_rd = 0;		// num of read
uint16_t	num_de = 0;		// num of reactivation

/*----------------------------------------------------------------------------------------------
/* Functions
 ---------------------------------------------------------------------------------------------*/

/*============================================================================================*/
/* gets called in case of error */
static void error(phStatus_t status, int line)
{
	// there was an error

	volatile phStatus_t s;	/* to prevent optimizations */
	volatile int l;			/* to prevent optimizations */

	s = status;
	l = line;

	while (1) {};
}

/*============================================================================================*/
void phCardEmu_Init(void * hal, void * osal, uint8_t sak)
{
	phStatus_t status;

	pHal = hal;
	pOsal = osal;

	/* configure the parameters for anti-collision */
	status = phhalHw_SetListenParameters(pHal, &sens_res[0], &nfc_id1[0], sak, &poll_res[0], nfc_id3);
	CHECK_STATUS(status);

	// generate random ID
	status = phhalHw_WriteRegister(pHal, PHHAL_HW_PN512_REG_COMMAND, PHHAL_HW_PN512_CMD_RANDOMIDS);
	CHECK_STATUS(status);

	#if defined (PHCARDEMU_T2T_PHDC)
	RTC_Init(RTC_CLKSRC_1HZ,0);
	RTC_Cmd(ENABLE);
	#endif
}

/*============================================================================================*/
phStatus_t phCardEmu_Activate(uint8_t ** ppRxBuffer, uint16_t * pRxLength)
    {
    phStatus_t status;
    uint8_t bRegister;

#ifdef PHCARDEMU_REFBOARD
    phOsal_LEDOn(pOsal, PHCARDEMU_REFBOARD_LED_RED);
    phOsal_LEDOff(pOsal, PHCARDEMU_REFBOARD_LED_GREEN);
    phOsal_LEDOff(pOsal, PHCARDEMU_REFBOARD_LED_BLUE);
#endif

    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_TXDATARATE, PHHAL_HW_RF_DATARATE_106));
    PH_CHECK_SUCCESS_FCT(status, phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_RXDATARATE, PHHAL_HW_RF_DATARATE_106));

    // check for RF loss and generate random ID
    PH_CHECK_SUCCESS_FCT(status, phhalHw_ReadRegister(pHal, PHHAL_HW_PN512_REG_DIVIRQ, &bRegister));
    if(bRegister & PHHAL_HW_PN512_BIT_RFOFFI)
        {
        PH_CHECK_SUCCESS_FCT(status, phhalHw_WriteRegister(pHal, PHHAL_HW_PN512_REG_DIVIRQ, (bRegister & (~PHHAL_HW_PN512_BIT_RFOFFI))));
        // generate random ID
        PH_CHECK_SUCCESS_FCT(status, phhalHw_WriteRegister(pHal, PHHAL_HW_PN512_REG_COMMAND, PHHAL_HW_PN512_CMD_RANDOMIDS));
        }

    /* there is no initiator - we listen for a new one */
    return phhalHw_Listen(pHal, ppRxBuffer, pRxLength, 0, 0); //TODO: make it a compile option
    }

/*============================================================================================*/
uint8_t phCardEmu_T2T_Init(void)
    {
    phStatus_t status;
    uint16_t dataLength;
    uint16_t tagLength;

    // start with 16 bytes for the internal, static lock and CC bytes
    tagLength = 16;

    // round up the data memory size to the next multiple of 8
    // because of the size encoding on the CC
    if(PHCARDEMU_T2T_DATAMEM_SIZE > 2040)
        {
        dataLength = 2040;
        }
    else
        {
        dataLength = ((PHCARDEMU_T2T_DATAMEM_SIZE+7)/8)*8;
        }
    tagLength += dataLength;
    // when data size is larger than 48 bytes, dynamic lock bytes
    // are added after this data space
    if(dataLength > 48)
        {
        // NumberOfDynamicLockBytes = roundup((DataAreaSize - 48) / 64)
        tagLength += (dataLength-48+63)/64;
        }

    if(t2tMemory == NULL)
        {
#if defined (PHCARDEMU_T2T_ON_RAM) || defined (PHCARDEMU_T2T_PHDC)
        phOsal_Mem_Malloc(pOsal, tagLength, sizeof(byte), 0, (void ** )&t2tMemory);
#else
        phOsal_Mem_Falloc(pOsal, tagLength, sizeof(byte), &t2tMemory);
#endif
        phOsal_Mem_PVR_Commit(pOsal);
        }

    if(t2tMemory != NULL)
        {
        /* init the NFC Tag Type 2 Component */
        status = phceT2T_Sw_Init(&cet2t, sizeof(phceT2T_Sw_DataParams_t), pHal, pOsal, t2tMemory, tagLength, dataLength);
        if(status != PH_ERR_SUCCESS)
            {
            return 0;
            }
        }
    else
        {
        return 0;
        }
    return 1;
    }

/*============================================================================================*/
void phCardEmu_T2T_Start(uint8_t * pRxBuffer, uint16_t wRxLength)
{
	phStatus_t status;

#ifdef PHCARDEMU_T2T_PHDC
	// init with pointer at the first length byte of TLV
	// and size available after T of TLV
	PHDC_NDEFhelper_Init(&(cet2t.pTagMemory[17]), cet2t.wTagLength - 17, NDEF_T2T);
	PHDC_Init();
#endif

	do
	    {
	    /* Enter into listen mode. Performed action depends on internal
	     * state of the NFC Tag Type 2 (phceT2T) Component which can be:
	     *
	     * -PHCET2T_STATE_NONE:        Performs activation and receive first command.
	     * -PHCET2T_STATE_ACTIVATED:   Process command received
	     * -PHCET2T_STATE_SEND:   	   Sends answer to last accepted command and receives a new one
	     * -PHCET2T_STATE_ERROR:       Tag is in error state and will return PH_ERR_USE_CONDITION (it needs to be reset)
	     *  */
	    status = phceT2TCmdHdl_Listen(&cet2t, 0, pRxBuffer, wRxLength);
	    /* Listen returns PHCE_T2T_ERR_SUCCESS_READ or PHCE_T2T_ERR_SUCCESS_WRITE
	     * if user has to provide data for NDEF tag.
	     * User should provide the data as fast as possible and send it back by calling
	     * phceT2TCmdHdl_Listen with correct parameters */
	    if (PHCE_T2T_ERR_SUCCESS_WRITE == (status & PH_ERR_MASK))
	        {
#ifdef PHCARDEMU_T2T_PHDC
	        byte * pNDEF;
	        unsigned short wLength;
	        PHDC_NDEFhelper_Read(&pNDEF,&wLength);
	        if(wLength > 0)
	            PHDC_Process(pNDEF, wLength);
#endif

	        if(t2tMemory != NULL)
	            {
	            num_wr++;
	            }
	        }

	    if (PHCE_T2T_ERR_SUCCESS_READ == (status & PH_ERR_MASK))
	        {
	        num_rd++;
	        }

	    if (PHCE_T2T_ERR_DEACTIVATED == (status & PH_ERR_MASK))
	        {
	        num_de++;
	        }

	    } while (
	            ((status & PH_ERR_MASK) == PHCE_T2T_ERR_SUCCESS_READ) ||
	            ((status & PH_ERR_MASK) == PHCE_T2T_ERR_SUCCESS_WRITE)
	    );

	/* reset the tag internal state -> now we can we start from the top */
	phceT2TCmdHdl_ResetTag(&cet2t);
}

/*==============================================================================================
 *   End of File
 ---------------------------------------------------------------------------------------------*/

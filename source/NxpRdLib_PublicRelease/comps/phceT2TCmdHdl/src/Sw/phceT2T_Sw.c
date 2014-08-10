/*
wa *             Copyright (c) NXP, 2012
 *                  http://www.nxp.com
 *
 *         Developed by CETRTA POT, d.o.o. Kranj, 2012
 *                 http://www.cetrtapot.si
 *
 * All rights are reserved. Reproduction in whole or in part is
 * prohibited without the written consent of the copyright owner.
 * NXP reserves the right to make changes without notice at
 * any time. NXP makes no warranty, expressed, implied or
 * statutory, including but not limited to any implied warranty of
 * merchantability or fitness for any particular purpose, or that
 * the use will not infringe any third party patent, copyright or
 * trademark. NXP must not be liable for any loss or damage
 * arising from its use.
 */

#include <phCardEmu.h>
#include <phCardEmu_Options.h>
#include <phOsal.h>
#include <ph_RefDefs.h>
#include <ph_Status.h>

#ifdef NXPBUILD__PHCET2T_SW
#include "phceT2T_Sw.h"
#include "phceT2T_Sw_Int.h"

uint8_t auxBuffer[16];

/*============================================================================================*/
phStatus_t phceT2T_Sw_Init
(
      phceT2T_Sw_DataParams_t * pDataParams,
      uint16_t wSizeOfDataParams,
      void * pHalDataParams,
      void * pOsalDataParams,
      uint8_t * pTagMemory,
      uint16_t wTagLength,
      uint16_t wDataLength
)
   {
   /* parameter check */
   if (sizeof(phceT2T_Sw_DataParams_t) != wSizeOfDataParams)
      {
      return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CE_T2T);
      }
   PH_ASSERT_NULL (pDataParams);
   PH_ASSERT_NULL (pHalDataParams);

   /* set initial data */
   pDataParams->wId = PH_COMP_CE_T2T | PHNFC_TAGTYPE2_SW_ID;

   pDataParams->bState = PHCET2T_STATE_ACTIVATED;

   // T2T

   pDataParams->bSelSector = 0;

   pDataParams->pTagMemory = pTagMemory;
   pDataParams->wTagLength = wTagLength;

   pDataParams->pTxBuffer = NULL;
   pDataParams->wTxLength = 0;

   pDataParams->bWaitForSector = 0;

   pDataParams->bLastCmd = 0;

   pDataParams->pHalDataParams = pHalDataParams;
   pDataParams->pOsalDataParams = pOsalDataParams;

   memcpy(auxBuffer,T2T_LOCK_CC_DEF,16);
   auxBuffer[14] = wDataLength / 8;

   phOsal_Mem_Copy(pDataParams->pOsalDataParams,auxBuffer,0,pDataParams->pTagMemory,0, 16);
   phOsal_Mem_Copy(pDataParams->pOsalDataParams,(void*)T2T_DATA_DEF,0,pDataParams->pTagMemory,16, T2T_DATA_DEF_SIZE);

   return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CE_T2T);
   }

/*============================================================================================*/
phStatus_t phceT2T_Sw_Listen
(
      phceT2T_Sw_DataParams_t * pDataParams,
      uint16_t                  wOption,
      uint8_t 				* pRxBufferPar,
      uint16_t 				wRxLengthPar
)
    {
    phStatus_t status;

    uint8_t * pRxBuffer = pRxBufferPar;
    uint16_t wRxLength = wRxLengthPar;

    uint8_t sendBits = PH_OFF;

    uint16_t wListenOptions = PH_EXCHANGE_DEFAULT;

    /* main entry point in to tag state machine */
    do
        {
        switch(pDataParams->bState)
            {
            case PHCET2T_STATE_NONE:
            //				pDataParams->bLastCmd = 0;
            //				pDataParams->bSelSector = 0;
            //				pDataParams->bWaitForSector = PH_OFF;
            //
            //				/* there is no initiator - we listen for a new one */
            //				//status = phhalHw_Listen(pDataParams->pHalDataParams, &pRxBuffer, &wRxLength);
            //
            //				if (PH_ERR_SUCCESS == (status & PH_ERR_MASK))
            //				{
            //					/* we have a new initiator, lets see what he wants */
            //					pDataParams->bState = PHCET2T_STATE_ACTIVATED;
            //					status = PHCE_T2T_ERR_NEW_STATE;
            //				}
            status = PH_ADD_COMPCODE(PHCE_T2T_ERR_DEACTIVATED, PH_COMP_CE_T2T);
            break;

            case PHCET2T_STATE_ACTIVATED:
                /* process received data */
                status = phceT2T_Sw_Int_Listen(pDataParams,
                        pRxBuffer,
                        &wRxLength,
                        &wListenOptions);
                break;

            case PHCET2T_STATE_SEND:
                if((pDataParams->pTxBuffer == &(pDataParams->bAckNackResponse)) && (pDataParams->wTxLength == 1))
                    {
                    phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_OFF);
                    phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0x04);
                    sendBits = PH_ON;
                    }

                status = phhalHw_Exchange(pDataParams->pHalDataParams,
                        wListenOptions,
                        pDataParams->pTxBuffer,
                        pDataParams->wTxLength,
                        &pRxBuffer,
                        &wRxLength);
                if(sendBits == PH_ON)
                    {
                    sendBits = PH_OFF;
                    phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXLASTBITS, 0x00);
                    phhalHw_SetConfig(pDataParams->pHalDataParams, PHHAL_HW_CONFIG_TXCRC, PH_ON);
                    }

                if (PH_ERR_SUCCESS != (status & PH_ERR_MASK))
                    {
                    return status;
                    }
                else
                    {
                    pDataParams->bState = PHCET2T_STATE_ACTIVATED;
                    status = PHCE_T2T_ERR_NEW_STATE;
                    }
                break;

            case PHCET2T_STATE_ERROR:
                /* return error */
                status = PH_ADD_COMPCODE(PH_ERR_USE_CONDITION, PH_COMP_CE_T2T);
                break;

            default:
                /* unknown state => should not happen */
                pDataParams->bState = PHCET2T_STATE_ERROR;

                status = PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_CE_T2T);
                break;
            }
        }
    while (PHCE_T2T_ERR_NEW_STATE == (status & PH_ERR_MASK));

   /* error happened => tag is in invalid state, requires reset */
   if(
         (PH_ERR_SUCCESS != (status & PH_ERR_MASK)) &&
         (
               (PHCE_T2T_ERR_SUCCESS_READ != (status & PH_ERR_MASK)) &&
               (PHCE_T2T_ERR_SUCCESS_WRITE != (status & PH_ERR_MASK)) &&
               (PHCE_T2T_ERR_DEACTIVATED != (status & PH_ERR_MASK)) &&
               (PH_COMP_CE_T2T == (status & PH_COMP_MASK))
         )
   )
      {
       pDataParams->bState = PHCET2T_STATE_ERROR;
      }

   return status;
   }

/*============================================================================================*/
phStatus_t phceT2T_Sw_Int_Listen
(
      phceT2T_Sw_DataParams_t * pDataParams,
      uint8_t *                 pRxBuffer,
      uint16_t *                pRxLength,
      uint16_t *				  pListenOptions
)
   {

#ifdef PHCARDEMU_REFBOARD
   phOsal_LEDOn(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif

   /* check command length */
   if (*pRxLength < 2)
      {
#ifdef PHCARDEMU_REFBOARD
      phOsal_LEDOff(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif
      // go to idle/sleep
      pDataParams->bState = PHCET2T_STATE_NONE;
      return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
      }

   if(pDataParams->bWaitForSector == PH_ON)
      {
      return phceT2T_Sw_Int_SectorSelect_Part2(pDataParams,
            pListenOptions,
            pRxBuffer,
            pRxLength);
      }

   /* check first byte for supported commands */
   switch(pRxBuffer[0])
      {
      case PHCET2T_CMD_READ:   //0x30
         return phceT2T_Sw_Int_Read(pDataParams,
               pListenOptions,
               pRxBuffer,
               pRxLength);
         break;
      case PHCET2T_CMD_WRITE:    // 0xA2
         return phceT2T_Sw_Int_Write(pDataParams,
               pListenOptions,
               pRxBuffer,
               pRxLength);
         break;
      case PHCET2T_CMD_SECTOR_SELECT: // 0xC2
         return phceT2T_Sw_Int_SectorSelect(pDataParams,
               pListenOptions,
               pRxBuffer,
               pRxLength);
         break;
      case PHCET2T_CMD_SLP_REQ:  // 0x50
         //go to idle/sleep state
         pDataParams->bState = PHCET2T_STATE_NONE;
         return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
         break;
      default:
         break;
      }

   /* instruction is unsupported */
   //go to idle/sleep state
   pDataParams->bState = PHCET2T_STATE_NONE;
   return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
   }

/*============================================================================================*/
phStatus_t phceT2T_Sw_Int_Read
(
        phceT2T_Sw_DataParams_t * pDataParams,
        uint16_t * pListenOptions,
        uint8_t * pRxBuffer,
        uint16_t * pRxLength
)
    {
    uint8_t bBlockNr;
    uint16_t wIndex;

#ifdef PHCARDEMU_REFBOARD
    phOsal_LEDToggle(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_GREEN);
#endif

    /* check if command length is valid */
    if (0x02 != *pRxLength)
        {
        // go to idle/sleep
        pDataParams->bState = PHCET2T_STATE_NONE;
        return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
        }

    /* extract block number */
    bBlockNr = pRxBuffer[1];
    wIndex = (pDataParams->bSelSector * 1024) + (bBlockNr * 4);

    if(wIndex > (pDataParams->wTagLength - 1))
        {
        //return NACK
        pDataParams->bAckNackResponse = PHCET2T_RESP_NACK;

        pDataParams->pTxBuffer = &(pDataParams->bAckNackResponse);
        pDataParams->wTxLength = 1;

        pDataParams->bState = PHCET2T_STATE_SEND;
        return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
        }

    if((wIndex+16) > pDataParams->wTagLength)
        {
        uint16_t diff = pDataParams->wTagLength - wIndex;

        memcpy(&auxBuffer[0], &(pDataParams->pTagMemory[wIndex]), diff);
        memcpy(&auxBuffer[diff], &(pDataParams->pTagMemory[0]), 16-diff);

        // transmit 16 bytes starting at requested block and wrapping around
        // to the beginning of the memory for the rest
        pDataParams->pTxBuffer = auxBuffer;
        }
    else
        {
        // transmit 16 bytes starting at the chosen block + the next 3
        pDataParams->pTxBuffer = &(pDataParams->pTagMemory[wIndex]);
        }
    pDataParams->wTxLength = 16;

    pDataParams->bLastCmd = PHCET2T_CMD_READ;

    pDataParams->bState = PHCET2T_STATE_SEND;
    return PH_ADD_COMPCODE(PHCE_T2T_ERR_SUCCESS_READ, PH_COMP_CE_T2T);
    }

/*============================================================================================*/
phStatus_t phceT2T_Sw_Int_Write
(
      phceT2T_Sw_DataParams_t * pDataParams,
      uint16_t * pListenOptions,
      uint8_t * pRxBuffer,
      uint16_t * pRxLength
)
    {
    uint8_t bBlockNr;
    uint8_t i;
    uint16_t wIndex;

#ifdef PHCARDEMU_REFBOARD
    phOsal_LEDToggle(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_GREEN);
#endif

    /* check if command length is valid */
    if (0x06 != *pRxLength )
        {
#ifdef PHCARDEMU_REFBOARD
        phOsal_LEDOff(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif
        // go to idle
        pDataParams->bState = PHCET2T_STATE_NONE;
        return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
        }

    /* extract block number */
    bBlockNr = pRxBuffer[1];
    wIndex = (pDataParams->bSelSector * 1024) + (bBlockNr * 4);

    if((wIndex <= 4) || (wIndex > (pDataParams->wTagLength - 1)))
        {
#ifdef PHCARDEMU_REFBOARD
        phOsal_LEDOff(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif
        //return NACK
        pDataParams->bAckNackResponse = PHCET2T_RESP_NACK;

        pDataParams->pTxBuffer = &(pDataParams->bAckNackResponse);
        pDataParams->wTxLength = 1;

        pDataParams->bState = PHCET2T_STATE_SEND;
        return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);

        }
    else
        {
        if(bBlockNr == 2 && pDataParams->bSelSector == 0)
            {
            auxBuffer[0] = pDataParams->pTagMemory[wIndex+2] | pRxBuffer[2];
            auxBuffer[1] = pDataParams->pTagMemory[wIndex+3] | pRxBuffer[3];
            phOsal_Mem_Copy(pDataParams->pOsalDataParams,auxBuffer,0,pDataParams->pTagMemory,wIndex+2,2);
            }
        else
            {
            for(i=0; i < 4; i++)
                {
                if(bBlockNr == 3 && pDataParams->bSelSector == 0)
                    {
                    auxBuffer[i] = (pDataParams->pTagMemory[wIndex+i] | pRxBuffer[2+i]);
                    }
                else
                    {
                    auxBuffer[i] = pRxBuffer[2+i];
                    }
                }
            phOsal_Mem_Copy(pDataParams->pOsalDataParams,auxBuffer,0,pDataParams->pTagMemory,wIndex,4);
            }

        pDataParams->bAckNackResponse = PHCET2T_RESP_ACK;

        pDataParams->pTxBuffer = &(pDataParams->bAckNackResponse);
        pDataParams->wTxLength = 1;

        pDataParams->bLastCmd = PHCET2T_CMD_WRITE;

        pDataParams->bState = PHCET2T_STATE_SEND;
        return PH_ADD_COMPCODE(PHCE_T2T_ERR_SUCCESS_WRITE, PH_COMP_CE_T2T);
        }
    }

/*============================================================================================*/
phStatus_t phceT2T_Sw_Int_SectorSelect
(
      phceT2T_Sw_DataParams_t * pDataParams,
      uint16_t * pListenOptions,
      uint8_t * pRxBuffer,
      uint16_t * pRxLength
)
   {
#ifdef PHCARDEMU_REFBOARD
   phOsal_LEDToggle(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_GREEN);
#endif

   /* check for command length and command integrity*/
	if (0x02 != *pRxLength || pRxBuffer[1] != 0xFF)
   {
#ifdef PHCARDEMU_REFBOARD
      phOsal_LEDOff(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif
      // go to idle/sleep state
      pDataParams->bState = PHCET2T_STATE_NONE;
      return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
   }

   // check if tag has more than one sector (>1k), set answer accordingly
   if(pDataParams->wTagLength > 1024)
      {
      pDataParams->bAckNackResponse = PHCET2T_RESP_ACK;
      pDataParams->bWaitForSector = PH_ON;
      }
   else
      {
#ifdef PHCARDEMU_REFBOARD
      phOsal_LEDOff(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif
      pDataParams->bAckNackResponse = PHCET2T_RESP_NACK;
      pDataParams->bWaitForSector = PH_OFF;
      }
   pDataParams->pTxBuffer = &(pDataParams->bAckNackResponse);
   pDataParams->wTxLength = 1;

   pDataParams->bState = PHCET2T_STATE_SEND;
   return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
   }

// Sector Select Command Packet 2
/*============================================================================================*/
phStatus_t phceT2T_Sw_Int_SectorSelect_Part2
(
      phceT2T_Sw_DataParams_t * pDataParams,
      uint16_t * pListenOptions,
      uint8_t * pRxBuffer,
      uint16_t * pRxLength
)
   {
   uint8_t bSectorNr;

   pDataParams->bWaitForSector = PH_OFF;

#ifdef PHCARDEMU_REFBOARD
   phOsal_LEDToggle(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_GREEN);
#endif

   // check for command length and integrity
   if (0x04 != *pRxLength || pRxBuffer[1] != 0x00 || pRxBuffer[2] != 0x00 || pRxBuffer[3] != 0x00)
      {
#ifdef PHCARDEMU_REFBOARD
      phOsal_LEDOff(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif
      // go to idle/sleep state
      pDataParams->bState = PHCET2T_STATE_NONE;
      return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
      }

   /* extract sector number */
   bSectorNr = pRxBuffer[0];

   // check if sector number is valid and exists(< 0xFF)
   if(bSectorNr == 0xFF || bSectorNr > (pDataParams->wTagLength / 1024))
      {
#ifdef PHCARDEMU_REFBOARD
      phOsal_LEDOff(pDataParams->pOsalDataParams, PHCARDEMU_REFBOARD_LED_BLUE);
#endif
      //return NACK
      pDataParams->bAckNackResponse = PHCET2T_RESP_NACK;

      pDataParams->pTxBuffer = &(pDataParams->bAckNackResponse);
      pDataParams->wTxLength = 1;
      } else {
         // change selected sector on internal data structure
         pDataParams->bSelSector = bSectorNr;

         // send passive ack (no answer)
         pDataParams->pTxBuffer = NULL;
         pDataParams->wTxLength = 0;
      }

   pDataParams->bLastCmd = PHCET2T_CMD_SECTOR_SELECT;

   pDataParams->bState = PHCET2T_STATE_SEND;
   return PH_ADD_COMPCODE(PHCE_T2T_ERR_NEW_STATE, PH_COMP_CE_T2T);
   }

/*============================================================================================*/
phStatus_t phceT2T_Sw_ResetTag
(
      phceT2T_Sw_DataParams_t * pDataParams
)
   {
   /* reset tag state to initial data */
   pDataParams->bState = PHCET2T_STATE_ACTIVATED;

   pDataParams->bSelSector = 0;
   pDataParams->bWaitForSector = PH_OFF;

   pDataParams->bLastCmd = 0;

   return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_CE_T2T);
   }

#endif /* NXPBUILD__PHCET2T_SW */

/*==============================================================================================
 *   End of File
 ---------------------------------------------------------------------------------------------*/

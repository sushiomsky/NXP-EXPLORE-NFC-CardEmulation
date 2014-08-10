/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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

/** \file
* Generic OSAL Component of Reader Library Framework.
* $Author: prabakaran.c $
* $Revision: 1.11 $
* $Date: Mon Dec  3 09:40:10 2012 $
*
* History:
*  PC: Generated 23. Aug 2012
*
*/


#include <ph_Status.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PH_OSAL
#include <phOsal.h>

#ifdef NXPBUILD__PH_OSAL_LPC12XX
//#include "Lpc12xx/phOsal_Lpc12xx.h"
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
#include "RaspberryPi/phOsal_R_Pi.h"
#include "stdio.h"
#endif /* NXPBUILD__PH_OSAL_RPi */

// globals

phOsal_TransVars_s phOsal_transVars[PH_OSAL_MAX_TRANS_VARS];
uint8_t phOsal_transVarsCount;
uint16_t phOsal_transVarsTotalBytes;

phOsal_FlashVars_s phOsal_flashVars[PH_OSAL_MAX_FLASH_VARS];
uint8_t phOsal_flashVarsCount;
uint16_t phOsal_flashVarsTotalBytes;

phOsal_PVR_s phOsal_PVR[PH_OSAL_MAX_PVR_VARS];
uint8_t phOsal_PVRCount;

phOsal_PVR_s * phOsal_PVR_inFlash;
phOsal_FlashVars_s * phOsal_flashVars_inFlash;
phOsal_TransVars_s * phOsal_transVars_inFlash;

const char phOsal_PVR_MagicNumber[3] = {0x4e,0x58,0x50};

char phOsal_allocingApp;

/*============================================================================================*/
phStatus_t phOsal_Timer_Init(void * pDataParams)
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phOsal_Timer_Create");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTimerId);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Timer_Init((phOsal_Lpc12xx_DataParams_t*)pDataParams);
        break;

#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
    	status = phOsal_RPi_Timer_Init((phOsal_RPi_DataParams_t*)pDataParams);
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
	{
		PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, pTimerId_log, pTimerId);
    }
#endif
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Timer_Create(void *pDataParams, uint32_t *pTimerId)
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phOsal_Timer_Create");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTimerId);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pTimerId);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Timer_Create((phOsal_Lpc12xx_DataParams_t*)pDataParams, pTimerId);
        break;

#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
    	status = phOsal_RPi_Timer_Create((phOsal_RPi_DataParams_t*)pDataParams, pTimerId);
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
	{
		PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, pTimerId_log, pTimerId);
    }
#endif
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Timer_Start(
    void * pDataParams,
    uint32_t dwTimerId,
    uint8_t  bTimerDelayUnit,
    uint32_t dwRegTimeCnt,
    ppCallBck_t pApplication_callback,
    void  * pContext
    )
{
    phStatus_t PH_MEMLOC_REM status = PH_ERR_SUCCESS;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phOsal_TImer_Start");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(dwTimerId);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(dwRegTimeCnt);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, dwTimerId_log, &dwTimerId);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, dwRegTimeCnt_log, &dwRegTimeCnt);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Timer_Start((phOsal_Lpc12xx_DataParams_t*)pDataParams,
                                dwTimerId,
                                bTimerDelayUnit,
                                dwRegTimeCnt,
                                (ppCallBck_t)pApplication_callback,
                                pContext);
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
    	status = phOsal_RPi_Timer_Start((phOsal_RPi_DataParams_t*)pDataParams,
                dwTimerId,
                dwRegTimeCnt,
                (ppCallBck_t)pApplication_callback,
                pContext);
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Timer_Stop(void * pDataParams, uint32_t dwTimerId)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phOsal_Timer_Stop");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(dwTimerId);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, dwTimerId_log, &dwTimerId);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }
    status = PH_ERR_SUCCESS;

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Timer_Stop((phOsal_Lpc12xx_DataParams_t*)pDataParams,
                                dwTimerId);
        break;
#endif /* NXPBUILD__PH_OSAL_WINOS */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
    	status = phOsal_RPi_Timer_Stop((phOsal_RPi_DataParams_t*)pDataParams, dwTimerId);
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Timer_Wait(
                             void * pDataParams,
                             uint8_t bTimerDelayUnit,
                             uint16_t wDelay
                             )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phOsal_Timer_Wait");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bTimerDelayUnit);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wDelay);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bTimerDelayUnit_log, &bTimerDelayUnit);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wDelay_log, &wDelay);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }
    status = PH_ERR_SUCCESS;

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        phOsal_Lpc12xx_Timer_Wait(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bTimerDelayUnit,
            wDelay
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
    	status = phOsal_RPi_Timer_Wait((phOsal_RPi_DataParams_t*)pDataParams,  bTimerDelayUnit, wDelay);
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }
    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Timer_Delete(void * pDataParams, uint32_t dwTimerId)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phOsal_Timer_Delete");
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(dwTimerId);
    PH_LOG_HELPER_ADDPARAM_UINT32(PH_LOG_LOGTYPE_DEBUG, dwTimerId_log, &dwTimerId);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Timer_Delete((phOsal_Lpc12xx_DataParams_t*)pDataParams,
                                dwTimerId);
        break;
#endif /* NXPBUILD__PH_OSAL_WINOS */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
    	status = phOsal_RPi_Timer_Delete((phOsal_RPi_DataParams_t*)pDataParams, dwTimerId);
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_Falloc(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        uint16_t wNum, 		/**< [In] Number of elements to allocate */
                        uint8_t bSize, 		/**< [In] Size of each element */
                        void ** pMem        /**< [Out] Pointer to Memory allocated */
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_Falloc(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            wNum, bSize, pMem
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_Malloc(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        uint16_t wNum, 		/**< [In] Number of elements to allocate */
                        uint8_t bSize, 		/**< [In] Size of each element */
                        uint8_t bEvent, 	/**< [In] Event for transient variable */
                        void ** pMem        /**< [Out] Pointer to Memory allocated */
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_Malloc(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            wNum, bSize, bEvent, pMem
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = phOsal_RPi_Mem_Malloc(
                (phOsal_RPi_DataParams_t*)pDataParams,
                wNum,
                bSize,
                bEvent,
                pMem
            );

        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_Copy(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        void * pSrc, 		/**< [In] Pointer to source */
                        uint16_t wSrcOff, 	/**< [In] Source pointer offset */
                        void * pDst, 		/**< [In] Pointer to destination */
                        uint16_t wDstOff, 	/**< [In] Destination pointer offset*/
                        uint16_t wSize 	/**< [In] Size of the memory to copy */
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
        {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
        }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_Copy(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            pSrc, wSrcOff, pDst, wDstOff, wSize
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = phOsal_RPi_Mem_Copy(
            (phOsal_RPi_DataParams_t*)pDataParams,
            pSrc,
            wSrcOff,
            pDst,
            wDstOff,
            wSize
            );
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_PVR(
	void * pDataParams, 	/**< [In] Pointer to this layers parameter structure. */
	void * pPointer, 							/**< [In] Pointer to memory to be registered */
	uint8_t bSize, 								/**< [In] Size of the variable */
	uint64_t wContent,    						/**< [In] Content of the variable */
	uint8_t bCommit	    						/**< [In] Commit changes on tables to non-volatile */
)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_PVR(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            pPointer, bSize, wContent, bCommit
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_PVR_Commit(
		void * pDataParams 	/**< [In] Pointer to this layers parameter structure. */
)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_PVR_Commit(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_PVR_Restore(
		void * pDataParams 	/**< [In] Pointer to this layers parameter structure. */
)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_PVR_Restore(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_FreeAppVars(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        char bAppId 		/**< [In] ID of the App that allocated the memory */
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_FreeAppVars(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bAppId
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Mem_ClearAppVars(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        char bAppId, 		/**< [In] ID of the App that allocated the memory */
                        uint8_t bEvent 	/**< [In] Event for transient variable */
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Mem_ClearAppVars(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bAppId, bEvent
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_Sleep(
                        void * pDataParams  /**< [In] Pointer to this layers parameter structure. */
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_Sleep(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_LEDOn(
						void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
						uint8_t bLED		/**< [In] Number of the LED*/
						)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_LEDOn(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bLED
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_LEDOff(
						void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
						uint8_t bLED		/**< [In] Number of the LED*/
						)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_LEDOff(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bLED
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_LEDToggle(
						void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
						uint8_t bLED		/**< [In] Number of the LED*/
						)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_LEDToggle(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bLED
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_DIPSW_GetValue(
		void * pDataParams, 	/**< [In] Pointer to this layers parameter structure. */
		uint8_t bDIPSW,			/**< [In] Number of the DIPSW */
		uint8_t * bValue		/**< [Out] Value of the DIPSW */
)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_DIPSW_GetValue(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bDIPSW, bValue
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_EnableIRQ(
                        void * pDataParams /**< [In] Pointer to this layers parameter structure. */
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_EnableIRQ(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_DisableIRQ(
	                    void * pDataParams /**< [In] Pointer to this layers parameter structure. */
	                    )
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_DisableIRQ(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}

/*============================================================================================*/
phStatus_t phOsal_CommDev_Control(
	void * pDataParams, 	/**< [In] Pointer to this layers parameter structure. */
	uint8_t bAction		/**< [In] Action to be made: Power Up, Down or Reset */
)
{
    phStatus_t PH_MEMLOC_REM status;

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_OSAL)
    {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);

        return status;
    }

    /* Select the active layer to perform the action */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PH_OSAL_LPC12XX
    case PH_OSAL_LPC12XX_ID:
        status = phOsal_Lpc12xx_CommDev_Control(
            (phOsal_Lpc12xx_DataParams_t*)pDataParams,
            bAction
            );
        break;
#endif /* NXPBUILD__PH_OSAL_LPC12XX */

#ifdef NXPBUILD__PH_OSAL_RPi
    case PH_OSAL_RPi_ID:
        status = PH_ERR_SUCCESS;
        break;
#endif /* NXPBUILD__PH_OSAL_RPi */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_OSAL);
        break;
    }

    return status;
}
#endif  /* NXPBUILD__PH_OSAL */

/*==============================================================================================
 *   End of File
 ---------------------------------------------------------------------------------------------*/

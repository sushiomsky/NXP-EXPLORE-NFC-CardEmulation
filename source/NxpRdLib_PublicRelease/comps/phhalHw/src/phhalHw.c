/*
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

/** \file
 * Generic HAL Component of Reader Library Framework.
 * $Author: santosh.d.araballi $
 * $Revision: 1.4 $
 * $Date: Fri Apr 15 09:27:29 2011 $
 *
 * History:
 *  CHu: Generated 19. May 2009
 *
 */

#include <phhalHw.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_PN512
#include "Pn512/phhalHw_Pn512.h"
#endif /* NXPBUILD__PHHAL_HW_PN512 */

#ifdef NXPBUILD__PHHAL_HW_CALLBACK
#include "Callback/phhalHw_Callback.h"
#endif /* NXPBUILD__PHHAL_HW_CALLBACK */

#ifdef NXPBUILD__PHHAL_HW

phStatus_t phhalHw_DontUseIRQ(
        void * pDataParams,
        uint8_t bOption
)
    {
    phStatus_t PH_MEMLOC_REM status;

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            if(bOption)
                ((phhalHw_Pn512_DataParams_t *) pDataParams)->bDontUseIrq = 1;
            else
                ((phhalHw_Pn512_DataParams_t *) pDataParams)->bDontUseIrq = 0;
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    return status;
    }

phStatus_t phhalHw_Exchange(
        void * pDataParams,
        uint16_t wOption,
        uint8_t * pTxBuffer,
        uint16_t wTxLength,
        uint8_t ** ppRxBuffer,
        uint16_t * pRxLength
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Exchange");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pTxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pTxBuffer_log, pTxBuffer, wTxLength);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    if (wTxLength) PH_ASSERT_NULL (pTxBuffer);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
        {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
        return status;
        }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_Exchange((phhalHw_Pn512_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((((status & PH_ERR_MASK) == PH_ERR_SUCCESS) ||
            ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_CHAINING) ||
            ((status & PH_ERR_MASK) == PH_ERR_SUCCESS_INCOMPLETE_BYTE)) &&
            !(wOption & PH_EXCHANGE_BUFFERED_BIT) &&
            (ppRxBuffer != NULL))
        {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
        }
#endif
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_WriteRegister(
        void * pDataParams,
        uint8_t bAddress,
        uint8_t bValue
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_WriteRegister");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAddress);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAddress_log, &bAddress);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bValue_log, &bValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_WriteRegister((phhalHw_Pn512_DataParams_t *)pDataParams, bAddress, bValue);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_ReadRegister(
        void * pDataParams,
        uint8_t bAddress,
        uint8_t * pValue
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_ReadRegister");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bAddress);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bAddress_log, &bAddress);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_ReadRegister((phhalHw_Pn512_DataParams_t *)pDataParams, bAddress, pValue);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);
    return status;
}

phStatus_t phhalHw_ApplyProtocolSettings(
        void * pDataParams,
        uint8_t bCardType
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_ApplyProtocolSettings");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bCardType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bCardType_log, &bCardType);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_ApplyProtocolSettings((phhalHw_Pn512_DataParams_t *)pDataParams, bCardType);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        break;
    }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_SetConfig(
        void * pDataParams,
        uint16_t wConfig,
        uint16_t wValue
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_SetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wValue_log, &wValue);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_SetConfig((phhalHw_Pn512_DataParams_t *)pDataParams, wConfig, wValue);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_MfcAuthenticateKeyNo(
        void * pDataParams,
        uint8_t bBlockNo,
        uint8_t bKeyType,
        uint16_t wKeyNo,
        uint16_t wKeyVersion,
        uint8_t * pUid
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_MfcAuthenticateKeyNo");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wKeyVersion);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyType_log, &bKeyType);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyNo_log, &wKeyNo);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wKeyVersion_log, &wKeyVersion);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pUid);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_MfcAuthenticateKeyNo(
                    (phhalHw_Pn512_DataParams_t *)pDataParams,
                    bBlockNo,
                    bKeyType,
                    wKeyNo,
                    wKeyVersion,
                    pUid);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_MfcAuthenticate(
        void * pDataParams,
        uint8_t bBlockNo,
        uint8_t bKeyType,
        uint8_t * pKey,
        uint8_t * pUid
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_MfcAuthenticate");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bBlockNo);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bKeyType);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pKey);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pUid);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bBlockNo_log, &bBlockNo);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bKeyType_log, &bKeyType);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pKey_log, pKey, 6);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pUid_log, pUid, 4);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pKey);
    PH_ASSERT_NULL (pUid);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_MfcAuthenticate(
                    (phhalHw_Pn512_DataParams_t *)pDataParams,
                    bBlockNo,
                    bKeyType,
                    pKey,
                    pUid);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_GetConfig(
        void * pDataParams,
        uint16_t wConfig,
        uint16_t * pValue
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_GetConfig");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wConfig);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pValue);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wConfig_log, &wConfig);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pValue);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_GetConfig((phhalHw_Pn512_DataParams_t *)pDataParams, wConfig, pValue);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, pValue_log, pValue);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_FieldOn(
        void * pDataParams
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_FieldOn");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_FieldOn((phhalHw_Pn512_DataParams_t *)pDataParams);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_FieldOff(
        void * pDataParams
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_FieldOff");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_FieldOff((phhalHw_Pn512_DataParams_t *)pDataParams);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_FieldReset(
        void * pDataParams
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_FieldReset");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_FieldReset((phhalHw_Pn512_DataParams_t *)pDataParams);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_Wait(
        void * pDataParams,
        uint8_t bUnit,
        uint16_t wTimeout
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Wait");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bUnit);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wTimeout);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bUnit_log, &bUnit);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wTimeout_log, &wTimeout);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
    {
		status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_Wait((phhalHw_Pn512_DataParams_t *)pDataParams, bUnit, wTimeout);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_Listen(
                        void * pDataParams,
                        uint8_t ** ppRxBuffer,
                        uint16_t * pRxLength,
                        uint8_t bTimeoutUnit,
                        uint16_t wTimeout
                        )
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_Listen");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(ppRxBuffer);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
        {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
        }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
        status = phhalHw_Pn512_Listen((phhalHw_Pn512_DataParams_t *)pDataParams, ppRxBuffer, pRxLength, bTimeoutUnit, wTimeout);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }

    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
#ifdef NXPBUILD__PH_LOG
    if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS &&
        (ppRxBuffer != NULL))
    {
        PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, ppRxBuffer_log, *ppRxBuffer, *pRxLength);
        }
#endif

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

phStatus_t phhalHw_SetListenParameters
(
        void * pDataParams,
        uint8_t * pSensRes,
        uint8_t * pNfcId1,
        uint8_t  bSelRes,
        uint8_t * pPollRes,
        uint8_t  bNfcId3
)
    {
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phhalHw_SetListenParameters");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(status);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pSensRes);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pNfcId1);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bSelRes);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(pPollRes);
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(bNfcId3);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pSensRes_log, pSensRes, 2);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pNfcId1_log, pNfcId1, 3);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bSelRes_log, &bSelRes);
    PH_LOG_HELPER_ADDPARAM_BUFFER(PH_LOG_LOGTYPE_DEBUG, pPollRes_log, pPollRes, 18);
    PH_LOG_HELPER_ADDPARAM_UINT8(PH_LOG_LOGTYPE_DEBUG, bNfcId3_log, &bNfcId3);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pSensRes);
    PH_ASSERT_NULL (pNfcId1);
    PH_ASSERT_NULL (pPollRes);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_HAL)
        {
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);

        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return status;
        }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
        {
#ifdef NXPBUILD__PHHAL_HW_PN512
        case PHHAL_HW_PN512_ID:
            status = phhalHw_Pn512_SetListenParameters((phhalHw_Pn512_DataParams_t *)pDataParams, pSensRes, pNfcId1, bSelRes, pPollRes, bNfcId3);
            break;
#endif /* NXPBUILD__PHHAL_HW_PN512 */

        default:
            status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_HAL);
            break;
        }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);

    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
    }

#endif /* NXPBUILD__PHHAL_HW */

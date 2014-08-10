/*
 *             Copyright (c) NXP, 2012
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
#include <ph_Status.h>
#include <phceT2TCmdHdl.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHCET2T_SW
#include "Sw/phceT2T_Sw.h"
#endif /* NXPBUILD__PHCET2T_SW */

#ifdef NXPBUILD__PHCET2T_SW

phStatus_t phceT2TCmdHdl_Listen
(
	void *     pDataParams,
	uint16_t   wOption,
    uint8_t    * pRxBuffer,
    uint16_t   wRxLength
)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phceT2T_Listen");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ALLOCATE_PARAMNAME(wOption);
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_DEBUG, wOption_log, &wOption);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CE_T2T)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CE_T2T);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHCET2T_SW
    case PHNFC_TAGTYPE2_SW_ID:
        status = phceT2T_Sw_Listen((phceT2T_Sw_DataParams_t *)pDataParams, wOption, pRxBuffer, wRxLength);
        break;
#endif /* NXPBUILD__PHPAL_I14443P4C_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CE_T2T);
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

phStatus_t phceT2TCmdHdl_ResetTag
(
	void * pDataParams
)
{
    phStatus_t PH_MEMLOC_REM status;

    PH_LOG_HELPER_ALLOCATE_TEXT(bFunctionName, "phceT2T_ResetTag");
    /*PH_LOG_HELPER_ALLOCATE_PARAMNAME(pDataParams);*/
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_ENTER);
	PH_ASSERT_NULL (pDataParams);

    /* Check data parameters */
    if (PH_GET_COMPCODE(pDataParams) != PH_COMP_CE_T2T)
    {
        PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);
        PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
        PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CE_T2T);
    }

    /* perform operation on active layer */
    switch (PH_GET_COMPID(pDataParams))
    {
#ifdef NXPBUILD__PHCET2T_SW
    case PHNFC_TAGTYPE2_SW_ID:
        status = phceT2T_Sw_ResetTag((phceT2T_Sw_DataParams_t *)pDataParams);
        break;
#endif /* NXPBUILD__PHPAL_I14443P4C_SW */

    default:
        status = PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_CE_T2T);
        break;
    }
    PH_LOG_HELPER_ADDSTRING(PH_LOG_LOGTYPE_INFO, bFunctionName);


    PH_LOG_HELPER_ADDPARAM_UINT16(PH_LOG_LOGTYPE_INFO, status_log, &status);
    PH_LOG_HELPER_EXECUTE(PH_LOG_OPTION_CATEGORY_LEAVE);

    return status;
}

#endif /* NXPBUILD__PHCET2T_SW */

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
* Wait Routines for PN512 specific HAL-Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.4 $
* $Date: Fri Apr 15 09:27:50 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include <ph_Status.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>

#ifdef NXPBUILD__PHHAL_HW_PN512

#include "phhalHw_Pn512.h"
#include "phhalHw_Pn512_Int.h"
#include <phhalHw_Pn512_Reg.h>

#include <phOsal.h>

phStatus_t phhalHw_Pn512_WaitIrq(
                                 phhalHw_Pn512_DataParams_t * pDataParams,
                                 uint8_t bIrq0WaitFor,
								 uint8_t bIrq1WaitFor,
                                 uint8_t * pIrq0Reg,
								 uint8_t * pIrq1Reg
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIrq0EnReg;
    uint8_t     PH_MEMLOC_REM bIrq1EnReg;
	uint8_t     PH_MEMLOC_REM bRegister;

    /* Parameter check */
    if (!bIrq0WaitFor && !bIrq1WaitFor)
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    // clear flags of interrupts that we are going to wait for
//    if(bIrq0WaitFor)
//    	PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIRQ, bIrq0WaitFor ));
//    if(bIrq1WaitFor)
//        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIRQ, bIrq1WaitFor));

    /* Enable IRQ0 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, &bIrq0EnReg));
//    bIrq0EnReg &= PHHAL_HW_PN512_BIT_IRQINV;
    bIrq0EnReg |= bIrq0WaitFor;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, bIrq0EnReg));
    /* Enable IRQ1 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, &bIrq1EnReg));
//    bIrq1EnReg &= PHHAL_HW_PN512_BIT_IRQPUSHPULL;
    bIrq1EnReg |= bIrq1WaitFor;
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, bIrq1EnReg));

    /* wait until an IRQ occurs */
    do
    {
        /* INSERT CODE HERE */

        usleep(100);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_STATUS1, &bRegister));
    }
    while (!(bRegister & PHHAL_HW_PN512_BIT_IRQ));

    /* Clear IRQ0 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, &bIrq0EnReg));
    bIrq0EnReg &= ~(bIrq0WaitFor);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, bIrq0EnReg));
    /* Clear IRQ1 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, &bIrq1EnReg));
    bIrq1EnReg &= ~(bIrq1WaitFor);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, bIrq1EnReg));

    /* Return contents of IRQ0 register */
    if (pIrq0Reg)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIRQ, pIrq0Reg));
    }

    /* Return contents of IRQ1 register */
    if (pIrq1Reg)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIRQ, pIrq1Reg));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_Pn512_ConfigIrq(
                                 phhalHw_Pn512_DataParams_t * pDataParams,
                                 uint8_t bIrq0EnMask,
								 uint8_t bIrq1EnMask,
                                 uint8_t bIrq0EnBits,
								 uint8_t bIrq1EnBits
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bIrq0EnReg;
    uint8_t     PH_MEMLOC_REM bIrq1EnReg;
	uint8_t     PH_MEMLOC_REM bRegister;

    /* Parameter check */
    if (!bIrq0EnMask && !bIrq1EnMask)
    {
        return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
    }

    // clear flags of interrupts that we are going to wait for
    if((bIrq0EnMask & bIrq0EnBits))
    	PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIRQ, (bIrq0EnMask & bIrq0EnBits) ));
    if((bIrq1EnMask & bIrq1EnBits))
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIRQ, (bIrq1EnMask & bIrq1EnBits) ));

    /* Enable IRQ0 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, &bIrq0EnReg));
    bIrq0EnReg |= (bIrq0EnMask & bIrq0EnBits); 		// enable interrupts (Mask = 1 & Bit = 1)
    bIrq0EnReg &= ~(bIrq0EnMask & (~bIrq0EnBits)); 	// disable interrupts (Mask = 1 & Bit = 0)
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, bIrq0EnReg));

    /* Enable IRQ1 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, &bIrq1EnReg));
    bIrq1EnReg |= (bIrq1EnMask & bIrq1EnBits); 		// enable interrupts (Mask = 1 & Bit = 1)
    bIrq1EnReg &= ~(bIrq1EnMask & (~bIrq1EnBits)); 	// disable interrupts (Mask = 1 & Bit = 0)
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, bIrq1EnReg));

    // Check if interrupt has already occurred, if not, enable MCU interrupt
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_STATUS1, &bRegister));
    if(!(bRegister & PHHAL_HW_PN512_BIT_IRQ))
    	phOsal_EnableIRQ(pDataParams->pOsalDataParams);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

phStatus_t phhalHw_Pn512_WaitIrqAndSleep(
								 phhalHw_Pn512_DataParams_t * pDataParams,
								 uint8_t bIrq0WaitFor,
								 uint8_t bIrq1WaitFor,
								 uint8_t * pIrq0Reg,
								 uint8_t * pIrq1Reg
								 )
{
	phStatus_t  PH_MEMLOC_REM statusTmp;
	uint8_t     PH_MEMLOC_REM bIrq0EnReg;
	uint8_t     PH_MEMLOC_REM bIrq1EnReg;
	uint8_t     PH_MEMLOC_REM bRegister;

	/* Parameter check */
	if (!bIrq0WaitFor && !bIrq1WaitFor)
	{
		return PH_ADD_COMPCODE(PH_ERR_INTERNAL_ERROR, PH_COMP_HAL);
	}

	// Check if interrupt has already occurred, if not, put MCU to sleep
//    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_STATUS1, &bRegister));
//    if(!(bRegister & PHHAL_HW_PN512_BIT_IRQ))
//    	phOsal_Sleep(pDataParams->pOsalDataParams);


    /* wait until an IRQ occurs */
    do
    {
        /* INSERT CODE HERE */

        usleep(100);
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_STATUS1, &bRegister));
    }
    while (!(bRegister & PHHAL_HW_PN512_BIT_IRQ));




    /* Clear IRQ0 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, &bIrq0EnReg));
    bIrq0EnReg &= ~(bIrq0WaitFor);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIEN, bIrq0EnReg));
    /* Clear IRQ1 interrupt sources */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, &bIrq1EnReg));
    bIrq1EnReg &= ~(bIrq1WaitFor);
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIEN, bIrq1EnReg));

    /* Return contents of IRQ0 register */
    if (pIrq0Reg)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_COMMIRQ, pIrq0Reg));
    }

    /* Return contents of IRQ1 register */
    if (pIrq1Reg)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams, PHHAL_HW_PN512_REG_DIVIRQ, pIrq1Reg));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_HAL);
}

#endif /* NXPBUILD__PHHAL_HW_PN512 */

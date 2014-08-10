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

/*==============================================================================================
 * file
 * phCardEmu.h
 *
 *  Created on: 19 Feb 2013
 *      Author: Alex
 *
 * History:
 *  Alex: 1.0 - initial version
 *
 ---------------------------------------------------------------------------------------------*/

#ifndef PHCARDEMU_H_
#define PHCARDEMU_H_

/*----------------------------------------------------------------------------------------------
 * Includes
 ---------------------------------------------------------------------------------------------*/
#include <phhalHw.h>

/*----------------------------------------------------------------------------------------------
 * Macros and definitions
 ---------------------------------------------------------------------------------------------*/
#ifndef _BYTE_
#define _BYTE_
typedef unsigned char byte;
#endif

/*----------------------------------------------------------------------------------------------
 * Function declaration
 ---------------------------------------------------------------------------------------------*/
void phCardEmu_Init(void * hal, void * osal, uint8_t sak);
phStatus_t phCardEmu_Activate(uint8_t ** ppRxBuffer, uint16_t * pRxLength);
uint8_t phCardEmu_T2T_Init(void);
void phCardEmu_T2T_Start(uint8_t * pRxBuffer, uint16_t wRxLength);

#endif /* PHCARDEMU_H_ */






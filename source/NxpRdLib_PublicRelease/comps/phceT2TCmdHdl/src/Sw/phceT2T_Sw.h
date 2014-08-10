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
#ifndef _PHCET2T_SW_H_
#define _PHCET2T_SW_H_

#include <phceT2TCmdHdl.h>

extern const uint8_t T2T_LOCK_CC_DEF[];
extern const uint8_t T2T_DATA_DEF[];
extern const uint16_t T2T_DATA_DEF_SIZE;

phStatus_t phceT2T_Sw_Listen
(
	phceT2T_Sw_DataParams_t * pDataParams,
	uint16_t   				  wOption,
    uint8_t 				* pRxBufferPar,
    uint16_t 				wRxLengthPar
);

phStatus_t phceT2T_Sw_ResetTag
(
	phceT2T_Sw_DataParams_t * pDataParams
);

#endif /* _PHCET2T_SW_H_ */

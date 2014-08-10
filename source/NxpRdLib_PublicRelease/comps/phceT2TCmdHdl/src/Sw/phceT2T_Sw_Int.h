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
#ifndef _PHCET2T_SW_INT_H_
#define _PHCET2T_SW_INT_H_

#include <phceT2TCmdHdl.h>

#define PHCET2T_STATE_NONE       0x00
#define PHCET2T_STATE_ACTIVATED  0x01
#define PHCET2T_STATE_SEND       0x02
#define PHCET2T_STATE_ERROR      0x03

#define PHCET2T_CMD_READ         0x30
#define PHCET2T_CMD_WRITE        0xA2
#define PHCET2T_CMD_SLP_REQ      0x50
#define PHCET2T_CMD_SECTOR_SELECT 0xC2

#define PHCET2T_RESP_ACK         0xA
#define PHCET2T_RESP_NACK        0x0

phStatus_t phceT2T_Sw_Int_Listen
(
	phceT2T_Sw_DataParams_t *  pDataParams,
	uint8_t *                  pRxBuffer,
	uint16_t *                 pRxLength,
	uint16_t *				      pListenOptions
);

phStatus_t phceT2T_Sw_Int_Read
(
	phceT2T_Sw_DataParams_t *  pDataParams,
	uint16_t *                 pListenOptions,
	uint8_t *                  pRxBuffer,
	uint16_t *                 pRxLength
);

phStatus_t phceT2T_Sw_Int_Write
(
	phceT2T_Sw_DataParams_t *  pDataParams,
	uint16_t *                 pListenOptions,
	uint8_t *                  pRxBuffer,
	uint16_t *                 pRxLength
);

phStatus_t phceT2T_Sw_Int_SectorSelect
(
	phceT2T_Sw_DataParams_t *  pDataParams,
	uint16_t *                 pListenOptions,
	uint8_t *                  pRxBuffer,
	uint16_t *                 pRxLength
);

phStatus_t phceT2T_Sw_Int_SectorSelect_Part2
(
	phceT2T_Sw_DataParams_t *  pDataParams,
	uint16_t *                 pListenOptions,
	uint8_t *                  pRxBuffer,
	uint16_t *                 pRxLength
);
#endif /* _PHCET2T_SW_INT_H_ */

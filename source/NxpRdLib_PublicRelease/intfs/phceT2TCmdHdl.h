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
#ifndef _phceT2TCmdHdl_H_
#define _phceT2TCmdHdl_H_

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef NXPBUILD__PHCET2T_SW

/** \defgroup phceT2T_Sw Component : Software
* @{
*/

#define PHNFC_TAGTYPE2_SW_ID 0x01U /**< ID for Software NFC Tag Type 2 layer     */

/**
* \brief Software NFC Tag Type 2 parameter structure
*/
typedef struct
{
	uint16_t wId;				      /**< Layer ID for this component, NEVER MODIFY! */

	uint8_t  bState;		          /**< Holds current state */

	uint8_t  bSelSector;			  /**< Holds current selected sector */

	uint8_t * pTagMemory;	  		  /**< Holds pointer to tag memory */
	uint16_t wTagLength;	  		  /**< Length of tag memory */

	uint8_t * pTxBuffer;	  		  /**< Holds pointer to transmit buffer */
	uint16_t wTxLength;	  		  	  /**< Length of transmit buffer */

	uint8_t bAckNackResponse;	  	  /**< Response byte */

	uint8_t bLastCmd;				  /**< Keeps record of the last command received */
	uint8_t bWaitForSector;			  /**< If 1, tag is in sector select state */

	void *   pHalDataParams; 		  /**< Pointer to lower layer data parameters */
	void *   pOsalDataParams; 		  /**< Pointer to lower layer data parameters */
} phceT2T_Sw_DataParams_t;

/**
* \brief Perform initialization of the NFC Type 2 Tag
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phceT2T_Sw_Init(
								phceT2T_Sw_DataParams_t * pDataParams, 	/**< [In] Pointer to this layer's parameter structure. */
								uint16_t wSizeOfDataParams,				/**< [In] Specifies the size of the data parameter structure. */
								void * pHalDataParams,					/**< [In] Pointer to the HAL parameter structure of the underlying layer */
								void * pOsalDataParams,					/**< [In] Pointer to the OSAL parameter structure of the layer */
								uint8_t * pTagMemory,
								uint16_t wTagLength,
								uint16_t wDataLength
						   );

#endif

#ifdef NXPBUILD__PHCET2T

#define PHCE_T2T_ERR_SUCCESS_READ         (PH_ERR_CUSTOM_BEGIN + 0x01U) /**< Custom status code returned when user must provide data */
#define PHCE_T2T_ERR_NEW_STATE            (PH_ERR_CUSTOM_BEGIN + 0x02U) /**< Internal status code. */
#define PHCE_T2T_ERR_SUCCESS_WRITE        (PH_ERR_CUSTOM_BEGIN + 0x03U) /**< Custom status code returned when user must provide data */
#define PHCE_T2T_ERR_DEACTIVATED          (PH_ERR_CUSTOM_BEGIN + 0x04U) /**< Custom status code returned when tag needs to be reactivated */

#define PHCE_T2T_TAG_STATE_INIT	   	  	  0x00U /**< Tag state is INITIALIZED */
#define PHCE_T2T_TAG_STATE_RW	 	      0x01U /**< Tag state is READ/WRITE */
#define PHCE_T2T_TAG_STATE_RO	 	      0x02U /**< Tag state is READ ONLY */

#define PHCE_T2T_AC_GRANTED		      	  0x00U /**< NDEF file read/write access condition granted */
#define PHCE_T2T_AC_NOT_GRANTED	          0xFFU /**< NDEF file write access condition not granted */

/**
* \brief Waits for a new target. Returns only if there is an error or if user must provide data back to the target (status code #PHCE_T2T_ERR_SUCCESS_READ).
*
* \return Status code
* \retval #PHCE_T2T_ERR_SUCCESS_READ Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phceT2TCmdHdl_Listen(
		  	  	  	  	  	  void *     pDataParams, /**< [In]  Pointer to this layer's parameter structure. */
		  	  	  	  	  	  uint16_t   wOption,     /**< [In]  Option parameter. */
							  uint8_t    * pRxBuffer,
							  uint16_t   wRxLength
		  	  	  	  	 );

/**
* \brief Resets the internal state of the tag.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phceT2TCmdHdl_ResetTag(
						   	   void * pDataParams /**< [In]  Pointer to this layer's parameter structure. */
                           );

#endif

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PH_CE_T4T_H_ */


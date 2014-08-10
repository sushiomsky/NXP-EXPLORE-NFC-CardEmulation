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
* $Revision: 1.12 $
* $Date: Tue Dec  4 10:58:10 2012 $
*
* History:
*  PC: Generated 23. Aug 2012
*
*/

#ifndef PHOSAL_H
#define PHOSAL_H

#include <ph_Status.h>
#include <stdlib.h>

#define PH_OSAL_MAX_TRANS_VARS 9
#define PH_OSAL_MAX_TRANS_BYTES 5120

#define PH_OSAL_MAX_FLASH_VARS 6
#define PH_OSAL_MAX_FLASH_BYTES 65536

#define PH_OSAL_MAX_PVR_VARS 10

#define PH_OSAL_PVR_COMMIT (uint8_t) 1

typedef struct {
	char appID;
	void * ptr;
	unsigned short size;
	uint8_t event;
} phOsal_TransVars_s;

typedef struct {
	char appID;
	void * ptr;
	unsigned short size;
} phOsal_FlashVars_s;

typedef struct {
	void * ptr;
	uint8_t size;
	uint64_t content;
} phOsal_PVR_s;

extern phOsal_TransVars_s phOsal_transVars[PH_OSAL_MAX_TRANS_VARS];
extern uint8_t phOsal_transVarsCount;
extern uint16_t phOsal_transVarsTotalBytes;

extern phOsal_FlashVars_s phOsal_flashVars[PH_OSAL_MAX_FLASH_VARS];
extern uint8_t phOsal_flashVarsCount;
extern uint16_t phOsal_flashVarsTotalBytes;

extern phOsal_PVR_s phOsal_PVR[PH_OSAL_MAX_PVR_VARS];
extern uint8_t phOsal_PVRCount;

extern phOsal_PVR_s * phOsal_PVR_inFlash;
extern phOsal_FlashVars_s * phOsal_flashVars_inFlash;
extern phOsal_TransVars_s * phOsal_transVars_inFlash;

extern const char phOsal_PVR_MagicNumber[3];

extern char phOsal_allocingApp;

#define PH_OSAL_COMMDEV_PRWUP 		0x00U
#define PH_OSAL_COMMDEV_PRWDOWN 	   0x01U
#define PH_OSAL_COMMDEV_RESET 		0x02U

/** \defgroup phOsal Osal
* \brief These Components implement the Osal commands.
* @{
*/

#define PH_OSAL_TIMER_UNIT_US       0x00U           /**< Indicates that the specified delay is in micro seconds.*/
#define PH_OSAL_TIMER_UNIT_MS       0x01U           /**< Indicates that the specified delay is in Milli seconds. */
#define PH_OSAL_INVALID_TIMER_ID    0xFFFF          /**< Invalid Timer Id. */

/**
* \name OSAL error codes
*/
/** @{*/

#define PH_OSAL_ERR_NO_FREE_TIMER   (PH_ERR_CUSTOM_BEGIN + 0)   /**< Error condition indicating that no timer is available for allocation. */
#define PH_OSAL_ERR_INVALID_TIMER   (PH_ERR_CUSTOM_BEGIN + 1)   /**< Indicates that the timer ID that was supplied was invalid. */

/** @}*/

/* This Macro to be used to resolve Unused and unreference
 * compiler warnings. 
 */
#define PHOSAL_UNUSED_VARIABLE(x) for((x)=(x);(x)!=(x);)

/**
 *
 * \brief Timer callback interface which will be called once registered timer
 * timeout expires.
 * \param[in] TimerId  Timer Id for which callback is called.
 * \retval  None
 */
typedef void (*ppCallBck_t)(uint32_t TimerId, void *pContext);

#ifdef  __cplusplus
extern "C" {
#endif	/* __cplusplus */

#ifdef NXPBUILD__PH_OSAL_RPi
/** \defgroup phOsal_Lpc12xx Component : LPC12xx
* @{
*/

#define PH_OSAL_RPi_ID                        0x04U            /**< ID for RPi osal component */

/**
 * \brief RPi osal parameter stucture
 */
typedef struct
{
    uint16_t wId;                                                 /**< ID of this component, do not modify */
    /* Other stuff */
    uint8_t * bLEDPins;
    uint8_t bLEDCount;

    uint8_t * bDIPSWPins;
    uint8_t bDIPSWCount;

    uint8_t bCommDev_ResetPin;
} phOsal_RPi_DataParams_t;

phStatus_t phOsal_RPi_Init(
                            phOsal_RPi_DataParams_t * pDataParams	/**<[In] Data parameters representing this component */
);

phStatus_t phOsal_RPi_CommDev_Config(
                            phOsal_RPi_DataParams_t * pDataParams, 	/**< [In] Pointer to this layers parameter structure. */
                            uint8_t bPin								/**< [In] Pin in the GPIO0 where the NRSTPD pin is connected to */
);

#endif /* NXPBUILD__PH_OSAL_RPi */

#ifdef NXPBUILD__PH_OSAL

phStatus_t phOsal_Timer_Init(void * pDataParams);

/**
* \brief Allocates a timer to the application
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phOsal_Timer_Create(
                            void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                            uint32_t *timerId    /**< [In] Timer Id */
);

/**
* \brief Start the timer
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phOsal_Timer_Start(
                            void * pDataParams,                 /**< [In] Pointer to this layers parameter structure. */
                            uint32_t dwTimerId,                 /**< [In] Timer Id */
                            uint8_t 	 bTimerDelayUnit,       /**<[In] The time unit in which delay is represented */
                            uint32_t dwRegTimeCnt,              /**< [In] Time delay count */
                            ppCallBck_t pApplication_callback,  /**< [In] Callback to be called on time out */
                            void  * pContext                    /**< [In] Callback function context */
);

/**
* \brief Stop the timer
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phOsal_Timer_Stop(
                             void * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
                             uint32_t dwTimerId     /**< [In] Timer Id */
);

/**
* \brief Timer wait function
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phOsal_Timer_Wait(
                             void * pDataParams,        /**< [In] Pointer to this layers parameter structure. */
                             uint8_t bTimerDelayUnit,   /**< [In] Delay value unit could be in micro seconds or milliseconds */ 
                             uint16_t wDelay            /**< [In] Delay in microseconds */ 
);

/**
* \brief Delete or deallocate the timer
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phOsal_Timer_Delete(
                        void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                        uint32_t dwTimerId   /**< [In] Timer Id */
);

phStatus_t phOsal_Mem_Falloc(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        uint16_t wNum, 		/**< [In] Number of elements to allocate */
                        uint8_t bSize, 		/**< [In] Size of each element */
                        void ** pMem        /**< [Out] Pointer to Memory allocated */
);

phStatus_t phOsal_Mem_Malloc(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        uint16_t wNum, 		/**< [In] Number of elements to allocate */
                        uint8_t bSize, 		/**< [In] Size of each element */
                        uint8_t bEvent, 	/**< [In] Event for transient variable */
                        void ** pMem        /**< [Out] Pointer to Memory allocated */
);

phStatus_t phOsal_Mem_Copy(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        void * pSrc, 		/**< [In] Pointer to source */
                        uint16_t wSrcOff, 	/**< [In] Source pointer offset */
                        void * pDst, 		/**< [In] Pointer to destination */
                        uint16_t wDstOff, 	/**< [In] Destination pointer offset*/
                        uint16_t wSize 		/**< [In] Size of the memory to copy */
);

phStatus_t phOsal_Mem_PVR(
		                  void * pDataParams, 	/**< [In] Pointer to this layers parameter structure. */
		                  void * pPointer, 							/**< [In] Pointer to memory to be registered */
		                  uint8_t bSize, 								/**< [In] Size of the variable */
		                  uint64_t wContent,    						/**< [In] Content of the variable */
		                  uint8_t bCommit	    						/**< [In] Commit changes on tables to non-volatile */
);

phStatus_t phOsal_Mem_PVR_Commit(
		                  void * pDataParams 	/**< [In] Pointer to this layers parameter structure. */
);

phStatus_t phOsal_Mem_PVR_Restore(
                        void * pDataParams 	/**< [In] Pointer to this layers parameter structure. */
);

phStatus_t phOsal_Mem_FreeAppVars(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        char bAppId 		/**< [In] ID of the App that allocated the memory */
);

phStatus_t phOsal_Mem_ClearAppVars(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        char bAppId, 		/**< [In] ID of the App that allocated the memory */
                        uint8_t bEvent 		/**< [In] Event for transient variable */
);

phStatus_t phOsal_Sleep(
                        void * pDataParams  /**< [In] Pointer to this layers parameter structure. */
);

phStatus_t phOsal_LEDOn(
						      void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
						      uint8_t bLED		/**< [In] Number of the LED*/
);

phStatus_t phOsal_LEDOff(
						      void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
						      uint8_t bLED		/**< [In] Number of the LED*/
);

phStatus_t phOsal_LEDToggle(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        uint8_t bLED		/**< [In] Number of the LED*/
);

phStatus_t phOsal_DIPSW_GetValue(
                        void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                        uint8_t bDIPSW,		/**< [In] Number of the DIPSW */
                        uint8_t * dwValue	/**< [Out] Value of the DIPSW */
);

phStatus_t phOsal_EnableIRQ(
                        void * pDataParams /**< [In] Pointer to this layers parameter structure. */
);

phStatus_t phOsal_DisableIRQ(
	                    void * pDataParams /**< [In] Pointer to this layers parameter structure. */
);

phStatus_t phOsal_CommDev_Control(
//	phOsal_Lpc12xx_DataParams_t * pDataParams, 	/**< [In] Pointer to this layers parameter structure. */
	void * pDataParams, 	/**< [In] Pointer to this layers parameter structure. */
	uint8_t bAction								/**< [In] Action to be made: Power Up, Down or Reset */
);
/** @} */
#endif /* NXPBUILD__PH_OSAL */

#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif /* PHOSAL_H */
   

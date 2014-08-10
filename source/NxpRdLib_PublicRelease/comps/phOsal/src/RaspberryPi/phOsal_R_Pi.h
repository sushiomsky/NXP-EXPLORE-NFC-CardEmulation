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
 * LPC12xx OSAL Component of Reader Library Framework.
 * $Author: nxp $
 * $Revision: 1.0 $
 * $Date: Wed Apr 03 2013 $
 *
 * History:
 *
 */

#ifndef PHOSAL_RPI_H
#define PHOSAL_RPI_H


phStatus_t phOsal_RPi_Timer_Init(phOsal_RPi_DataParams_t * pDataParams);

/**
 * Starts the timer with set to expire after \a dwRegTimeCnt and 
 * calls the callback funnction ppApplicationCallback once timer expires.
 * On LPC12xx, we are using one of the two available TMR32 modules. 
 *
 * \return   Status code
 * \retval   #PH_OSAL_ERR_INVALID_TIMER If timer ID supplied was invalid
 * \retval   #PH_ERR_SUCCESS            on success   
 *
 */ 
phStatus_t phOsal_RPi_Timer_Start(
        phOsal_RPi_DataParams_t * pDataParams,  /**<[In] pointer to this layer's data structure */
        uint32_t     dwTimerId,                 /**<[In] Valid timer ID as returned by phOsal_Timer_Create() */
        uint32_t     dwRegTimeCnt,              /**<[In] The required amount of time out in MS */
        ppCallBck_t  pApplicationCallback,      /**<[In] Pointer to the call back function that will be called once timer expires */
        void         *pContext                  /**<[In] Argument with which the call back function will be called */
);

/**
 * Stops the said timer.
 * This function does not free the timer. It only disables the timer.
 * Use phOsal_Timer_Delete() to free the timer.
 *
 * \return  status code
 * \retval  #PH_OSAL_ERR_INVALID_TIMER if the timer ID supplied was invalid 
 * \retval  #PH_ERR_SUCCESS            on success
 *
 */

phStatus_t phOsal_RPi_Timer_Stop(
        phOsal_RPi_DataParams_t * pDataParams,  /**<[In] Pointer to this layer's data structure */
        uint32_t dwTimerId                      /**<[In] Id of the timer to be stopped */
);

/**
 * Frees the timer.
 * When this function is called, the timer with given ID is returned to the
 * free timer pool.
 *
 * \return  status code
 * \retval  #PH_OSAL_ERR_INVALID_TIMER if the timer ID supplied was invalid 
 * \retval  #PH_ERR_SUCCESS            on success
 *
 */
phStatus_t phOsal_RPi_Timer_Delete(
        phOsal_RPi_DataParams_t * pDataParams,  /**<[In] Pointer to this layer's data structure */
        uint32_t dwTimerId                      /**<[In] Timer ID */
);

/**
 * Creates a timer component.
 * Internally, an array is maintained which stores timers along with 
 * information as to whether the timer is available or not. This 
 * function searches a free timer that is available and returns the 
 * timer id in \a pTimerId. If there are no free timers,
 * then appropriate error code is returned.
 * 
 * \return   status code
 * \retval   #PH_ERR_SUCCESS on success
 * \retval   #PH_OSAL_ERR_NO_FREE_TIMER if no timers are available 
 *
 */

phStatus_t phOsal_RPi_Timer_Create(
        phOsal_RPi_DataParams_t *pDataParams,   /**<[In]  DataParams representing this component */
        uint32_t * pTimerId                     /**<[Out] Timer ID that was selected in case of success or else -1 */
);

/**
 * Delays the execution thread by given amount of delay.
 * This function is capable of delaying the execution in terms of Milli 
 * seconds as well in terms of micro seconds 
 *
 * \return   status code
 * \retval   #PH_ERR_SUCCESS
 *
 */

phStatus_t phOsal_RPi_Mem_Malloc(
        phOsal_RPi_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
        uint16_t wNum,                          /**< [In] Number of elements to allocate */
        uint8_t bSize,                          /**< [In] Size of each element */
        uint8_t bEvent,                         /**< [In] Event for transient variable */
        void ** pMem                            /**< [Out] Pointer to Memory allocated */
);

phStatus_t phOsal_RPi_Mem_Copy(
        phOsal_RPi_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
//        void * pSrc,                          /**< [In] Pointer to source */
        uint8_t  * pSrc,                        /**< [In] Pointer to source */
        uint16_t wSrcOff,                       /**< [In] Source pointer offset */
//        void * pDst,                          /**< [In] Pointer to destination */
        uint8_t  * pDst,                        /**< [In] Pointer to destination */
        uint16_t wDstOff,                       /**< [In] Destination pointer offset*/
        uint16_t wSize                          /**< [In] Size of the memory to copy */
);

phStatus_t phOsal_RPi_Timer_Wait(
        phOsal_RPi_DataParams_t * pDataParams,  /**<[In] Data structure representing this component */
        uint8_t bTimerDelayUnit,                /**<[In] The time unit in which delay is represented */
        uint16_t wDelay                         /**<{in] Amount of delay by which execution is to be delayed */
);

phStatus_t phOsal_RPi_GetMemory(
        phOsal_RPi_DataParams_t *pDataParams,   /**< [In] Pointer to this layers parameter structure. */
        uint32_t dwLength,                      /**< [In] Required memory length */
        void ** pMem                            /**< [Out] Pointer to Memory allocated */
);

phStatus_t phOsal_RPi_FreeMemory(
        phOsal_RPi_DataParams_t *pDataParams,   /**< [In] Pointer to this layers parameter structure. */
        void * ptr                              /**< [In] Pointer to memory to be freed */
);

#endif /* PHOSAL_R_PI_H */

/*==============================================================================================
 *   End of File
 ---------------------------------------------------------------------------------------------*/


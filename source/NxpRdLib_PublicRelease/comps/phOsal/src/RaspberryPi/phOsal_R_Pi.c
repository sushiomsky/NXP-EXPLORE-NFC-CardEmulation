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
 * RaspberryPi OSAL Component of Reader Library Framework.
 * $Author: nxp $
 * $Revision: 1.0 $
 * $Date: Wed Apr 03 2013 $
 *
 * History:
 *
 *
 */


#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phOsal.h>

#ifdef NXPBUILD__PH_OSAL_RPi
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#include "phOsal_R_Pi.h"

//#define	TRACE_TIMER

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

static phOsal_RPi_DataParams_t * phOsal_pDataParams;

static timer_t             *timerid;
static struct sigevent     sev;
static struct itimerspec   its;
static struct sigaction    action;
static ppCallBck_t	      pcall_back;
static void                *pCont;

/* Timer expired */
static void handle_alarm(int s)
    {
    (pcall_back)((uint32_t)*timerid, pCont);
#ifdef TRACE_TIMER
    printf("Timer: EXPIRE\n");
#endif
    }

/* Instal Signal handler */
static int RPi_Install_Handler(int signum, void (*handler)(int))
    {
    sigset_t mask;

    /* Unblock the signal. */
    sigemptyset(&mask);
    sigaddset(&mask, signum);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    /* Install the signal handler. */
    action.sa_handler = handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(signum, &action, NULL);

    return 0;
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Init( phOsal_RPi_DataParams_t * pDataParams )
    {

    uint8_t i;

    pDataParams->wId = PH_COMP_OSAL | PH_OSAL_RPi_ID;

    phOsal_transVarsCount = 0;
    phOsal_transVarsTotalBytes = 0;
    phOsal_flashVarsCount = 0;
    phOsal_flashVarsTotalBytes = 0;
    phOsal_PVRCount = 0;

    for(i = 0; i < PH_OSAL_MAX_TRANS_VARS; i++)
        {
        phOsal_transVars[i].appID = -1;
        phOsal_transVars[i].ptr = NULL;
        phOsal_transVars[i].size = 0;
        phOsal_transVars[i].event = 0;
        }

    for(i = 0; i < PH_OSAL_MAX_FLASH_VARS; i++)
        {
        phOsal_flashVars[i].appID = -1;
        phOsal_flashVars[i].ptr = NULL;
        phOsal_flashVars[i].size = 0;
        }

    phOsal_allocingApp = -1;

    phOsal_pDataParams = pDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Timer_Init( phOsal_RPi_DataParams_t * pDataParams )
    {
    pDataParams->wId = PH_COMP_OSAL | PH_OSAL_RPi_ID;
    
    /* Lets be ready with the timer */
#ifdef TRACE_TIMER
    printf("Timer: INIT\n");
#endif
    action.sa_handler = NULL;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Timer_Create(
        phOsal_RPi_DataParams_t *pDataParams,
        uint32_t * pTimerId
)
    {
#ifdef TRACE_TIMER
    printf("Timer: CREATE\n");
#endif	
    timerid = (timer_t*)pTimerId;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    if (timer_create(CLOCKID, &sev, (timer_t*)pTimerId) == -1)
        {
        /* No timer found, need to return error */
        return PH_ADD_COMPCODE(PH_OSAL_ERR_NO_FREE_TIMER,  PH_COMP_OSAL);
        }
    else
        {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
        }
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Timer_Wait(
        phOsal_RPi_DataParams_t * pDataParams,
        uint8_t bTimerDelayUnit,
        uint16_t wDelay
)
    {
    int ret;
    /* We will always use timer 1 for delaying purpose */
    //printf("Delay: %0x, type: %0x\n",wDelay, bTimerDelayUnit);
    if (bTimerDelayUnit == PH_OSAL_TIMER_UNIT_MS)
        {
        /* Delay in terms of Milli seconds */
        ret = usleep(1000 * wDelay);
        }
    else
        {
        /* Delay in terms of Micro seconds */
        ret = usleep(wDelay);
        }
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
    }


/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Timer_Start(
        phOsal_RPi_DataParams_t * pDataParams,
        uint32_t     dwTimerId,
        uint32_t     dwRegTimeCnt,
        ppCallBck_t  pApplicationCallback,
        void         *pContext
)
    {
    
#ifdef TRACE_TIMER
    printf("Timer: START\n");
#endif
    if(action.sa_handler == NULL)
        {
#ifdef TRACE_TIMER
        printf("Timer: Instal Handler\n");
#endif
        RPi_Install_Handler(SIGALRM, handle_alarm);
        }

    pcall_back = pApplicationCallback;
    pCont = pContext;

    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    its.it_value.tv_sec = dwRegTimeCnt / 1000;
    its.it_value.tv_nsec = (dwRegTimeCnt % 1000) * 1000000;

    if (timer_settime(*timerid, 0, &its, NULL) == -1)
        {
#ifdef TRACE_TIMER
        printf("Timer: START ERR\n");
#endif
        return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);   
        }
    else
        {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
        }
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Timer_Stop(
        phOsal_RPi_DataParams_t * pDataParams,
        uint32_t dwTimerId
)
    {
#ifdef TRACE_TIMER
    printf("Timer: STOP\n");
#endif
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;

    action.sa_handler = NULL;
    if (timer_settime(*timerid, 0, &its, NULL)== -1)
        {
#ifdef TRACE_TIMER
        printf("Timer: STOP ERR\n");
#endif
        return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
        }
    else
        {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
        }
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Timer_Delete(
        phOsal_RPi_DataParams_t * pDataParams,
        uint32_t dwTimerId
)
    {

#ifdef TRACE_TIMER
    printf("Timer: DELETE\n");
#endif
    if (timer_delete( *timerid )== -1)
        {
#ifdef TRACE_TIMER
        printf("Timer: DELETE ERR\n");
#endif
        return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
        }
    else
        {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
        }
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Mem_Malloc(
        phOsal_RPi_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
        uint16_t wNum,                          /**< [In] Number of elements to allocate */
        uint8_t bSize,                          /**< [In] Size of each element */
        uint8_t bEvent,                         /**< [In] Event for transient variable */
        void ** pMem                            /**< [Out] Pointer to Memory allocated */
)
    {
    if(((wNum * bSize) <= (PH_OSAL_MAX_TRANS_BYTES - phOsal_transVarsTotalBytes)) &&
            (phOsal_transVarsCount < PH_OSAL_MAX_TRANS_VARS)
    )
        {
        void * ptr = NULL;
        if(wNum > 0)
            {
            ptr = (void *) calloc(wNum,bSize);//malloc((num * size));
            }
        if(ptr != NULL || (wNum == 0))
            {
            phOsal_transVars[phOsal_transVarsCount].appID = phOsal_allocingApp;
            phOsal_transVars[phOsal_transVarsCount].ptr = pMem;
            phOsal_transVars[phOsal_transVarsCount].size = (wNum * bSize);
            phOsal_transVars[phOsal_transVarsCount].event = bEvent;
            phOsal_transVarsCount++;
            phOsal_transVarsTotalBytes += (wNum * bSize);
            *pMem = ptr;
            return PH_ERR_SUCCESS;
            }
        else
            {
            // fail
            *pMem = NULL;
            return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_OSAL);
            }
        }
    else
        {
        // fail
        *pMem = NULL;
        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_OSAL);
        }
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Mem_Falloc(
        phOsal_RPi_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
        uint16_t wNum,                          /**< [In] Number of elements to allocate */
        uint8_t bSize,                          /**< [In] Size of each element */
        void ** pMem                            /**< [Out] Pointer to Memory allocated */
)
    {
//    if(
//            ((wNum * bSize) <= (PH_OSAL_MAX_FLASH_BYTES - phOsal_flashVarsTotalBytes)) &&
//            (phOsal_flashVarsCount < PH_OSAL_MAX_FLASH_VARS)
//    )
//        {
//        void * ptr;
//        ptr = (void *) (LPC_BEGIN_RESERVED_FLASH - phOsal_flashVarsTotalBytes - (wNum * bSize));
//        phOsal_flashVars[phOsal_flashVarsCount].appID = phOsal_allocingApp;
//        phOsal_flashVars[phOsal_flashVarsCount].ptr = ptr;
//        phOsal_flashVars[phOsal_flashVarsCount].size = (wNum * bSize);
//        phOsal_flashVarsCount++;
//        phOsal_flashVarsTotalBytes += (wNum * bSize);
//        *pMem = ptr;
        return PH_ERR_SUCCESS;
//        }
//    else
//        {
//        // fail
//        *pMem = NULL;
//        return PH_ADD_COMPCODE(PH_ERR_RESOURCE_ERROR, PH_COMP_OSAL);
//        }
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_Mem_Copy(
        phOsal_RPi_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
        uint8_t * pSrc,                         /**< [In] Pointer to source */
        uint16_t wSrcOff,                       /**< [In] Source pointer offset */
        uint8_t * pDst,                         /**< [In] Pointer to destination */
        uint16_t wDstOff,                       /**< [In] Destination pointer offset*/
        uint16_t wSize                          /**< [In] Size of the memory to copy */
)
    {
    if(wSize > 0)
        {

        memcpy( &pDst[wDstOff], &pSrc[wSrcOff], wSize);
        return PH_ERR_SUCCESS;
        }
    else
        {
        printf("!!! MEM copy - data lengt 0");
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_OSAL);
        }
    }

/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_GetMemory(
        phOsal_RPi_DataParams_t *pDataParams,
        uint32_t dwLength,
        void ** pMem
)
    {
    phStatus_t status = PH_ERR_SUCCESS;

    PHOSAL_UNUSED_VARIABLE(pDataParams);
    pMem = (void *) malloc(dwLength);

    if (pMem == NULL)
        status = PH_ERR_RESOURCE_ERROR;

    return status;
    }


/*==============================================================================================
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_RPi_FreeMemory(
        phOsal_RPi_DataParams_t *pDataParams,
        void * ptr
)
    {
    phStatus_t status = PH_ERR_SUCCESS;

    PHOSAL_UNUSED_VARIABLE(pDataParams);
    if(NULL !=  ptr)
        free(ptr);

    return status;
    }

#endif /* NXPBUILD__PH_OSAL_R_Pi */

/*==============================================================================================
 *   End of File
 ---------------------------------------------------------------------------------------------*/

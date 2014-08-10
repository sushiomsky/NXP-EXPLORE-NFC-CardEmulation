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
* Build System Definitions for Reader Library Framework.
* $Author: jayaprakash.desai $
* $Revision: 1.6 $
* $Date: Wed May 25 14:06:15 2011 $
*
* History:
*  MK: Generated 15. October 2009
*
*/

#ifndef PH_NXPBUILD_H
#define PH_NXPBUILD_H

/** \defgroup ph_NxpBuild NXP Build
* \brief Controls build behaviour of components.
* @{
*/

/* NXPBUILD_DELETE: included code lines should be always removed from code */

/* NXP Build defines                            */
/* use #define to include components            */
/* comment out #define to exclude components    */

/* DEBUG build mode */

//#define NXPBUILD__PH_DEBUG

/* BAL components */

//#define NXPBUILD__PHBAL_REG_STUB                /**< Stub BAL definition */
//#define NXPBUILD__PHBAL_REG_LPC12XXSPI          /**< SerialWin BAL definition */
#define NXPBUILD__PHBAL_REG_RPISPI              /**< SerialWin BAL definition */

#if defined (NXPBUILD__PHBAL_REG_STUB)          || \
    defined (NXPBUILD__PHBAL_REG_LPC12XXSPI)	|| \
    defined (NXPBUILD__PHBAL_REG_RPISPI)
    #define NXPBUILD__PHBAL_REG                 /**< Generic BAL definition */
#endif

/* HAL components */

#define NXPBUILD__PHHAL_HW_PN512                /**< Pn512 HAL definition */

#if defined (NXPBUILD__PHHAL_HW_PN512)
    #define NXPBUILD__PHHAL_HW                  /**< Generic HAL definition */
#endif

/* PAL ISO 14443-3A components */

#define NXPBUILD__PHPAL_I14443P3A_SW            /**< Software PAL ISO 14443-3A definition */

#if defined (NXPBUILD__PHPAL_I14443P3A_SW)
    #define NXPBUILD__PHPAL_I14443P3A           /**< Generic PAL ISO 14443-3A definition */
#endif

/* PAL ISO 14443-3B components */

//#define NXPBUILD__PHPAL_I14443P3B_SW            /**< Software PAL ISO 14443-3B definition */

#if defined (NXPBUILD__PHPAL_I14443P3B_SW)
    #define NXPBUILD__PHPAL_I14443P3B           /**< Generic PAL ISO 14443-3B definition */
#endif

/* PAL ISO 14443-4A components */

#define NXPBUILD__PHPAL_I14443P4A_SW            /**< Software PAL ISO 14443-4A definition */
        
#if defined (NXPBUILD__PHPAL_I14443P4A_SW)
    #define NXPBUILD__PHPAL_I14443P4A           /**< Generic PAL ISO 14443-4A definition */
#endif

/* PAL ISO 14443-4 components */

#define NXPBUILD__PHPAL_I14443P4_SW             /**< Software PAL ISO 14443-4 definition */

#if defined (NXPBUILD__PHPAL_I14443P4_SW)
    #define NXPBUILD__PHPAL_I14443P4            /**< Generic PAL ISO 14443-4 definition */
#endif

/* PAL ISO 14443-4 Card components */

//#define NXPBUILD__PHPAL_I14443P4C_SW

#if defined (NXPBUILD__PHPAL_I14443P4C_SW)
	#define NXPBUILD__PHPAL_I14443P4C
#endif

/* PAL MIFARE components */

//#define NXPBUILD__PHPAL_MIFARE_STUB             /**< Stub PAL MIFARE */
//#define NXPBUILD__PHPAL_MIFARE_SW               /**< Software PAL MIFARE */

#if defined (NXPBUILD__PHPAL_MIFARE_STUB)       || \
    defined (NXPBUILD__PHPAL_MIFARE_SW)
    #define NXPBUILD__PHPAL_MIFARE              /**< Generic PAL MIFARE definition */
#endif

/* PAL ISO15693 ICode SLI components */

//#define NXPBUILD__PHPAL_SLI15693_SW             /**< Software PAL Sli 15693 */

#if defined (NXPBUILD__PHPAL_SLI15693_SW)
    #define NXPBUILD__PHPAL_SLI15693            /**< Generic PAL Sli 15693 definition */
#endif 

/* PAL Felica components */

//#define NXPBUILD__PHPAL_FELICA_SW               /**< Software PAL Felica */

#if defined (NXPBUILD__PHPAL_FELICA_SW)
    #define NXPBUILD__PHPAL_FELICA              /**< Generic PAL Felica definition */
#endif 

/* PAL ICode EPC/UID components */

//#define NXPBUILD__PHPAL_EPCUID_SW               /**< Software ICode EPC/UID */

#if defined (NXPBUILD__PHPAL_EPCUID_SW)
    #define NXPBUILD__PHPAL_EPCUID              /**< Generic PAL ICode EPC/UID definition */
#endif

/* PAL ISO 18092 components */

//#define NXPBUILD__PHPAL_I18092MPI_SW            /**< Software PAL ISO 18092 definition */

#if defined (NXPBUILD__PHPAL_I18092MPI_SW)
    #define NXPBUILD__PHPAL_I18092MPI           /**< Generic PAL ISO 18092 definition */
#endif

/* AL Felica components */


/* AL MIFARE Classic components */

//#define NXPBUILD__PHAL_MFC_SW                   /**< Software MIFARE Classic */

#if defined (NXPBUILD__PHAL_MFC_SW)
    #define NXPBUILD__PHAL_MFC                  /**< Generic AL MIFARE Classic definition */
#endif

/* AL MIFARE Plus components */


/* AL MIFARE Ultralight components */

//#define NXPBUILD__PHAL_MFUL_SW                  /**< Software MIFARE Ultralight */

#if defined (NXPBUILD__PHAL_MFUL_SW)
    #define NXPBUILD__PHAL_MFUL                 /**< Generic AL MIFARE Ultralight definition */
#endif

/* AL Virtual Card Architecture components */


/* DL FPGA Box components */


/* DL Amplifier components */


/* DL Thermostream components */


/* DL Oscilloscope components */


/* DL Master Amplifier Oscilloscope components */


/* DL Stepper components */


/* AL I15693 components */


/* AL Sli components */


/* CID Manager components */

//#define NXPBUILD__PH_CIDMANAGER_SW              /**< Software CID Manager */

#if defined (NXPBUILD__PH_CIDMANAGER_SW)
    #define NXPBUILD__PH_CIDMANAGER             /**< Generic CID Manager definition */
#endif

/* Crypto components */


/* CryptoRng components */


/* KeyStore components */

//#define NXPBUILD__PH_KEYSTORE_SW                /**< Software KeyStore */

#if defined (NXPBUILD__PH_KEYSTORE_SW)
    #define NXPBUILD__PH_KEYSTORE               /**< Generic KeyStore definition */
#endif

/* Card Emulation components */
#define NXPBUILD__PHCET2T_SW

#if defined (NXPBUILD__PHCET2T_SW)
   #define NXPBUILD__PHCET2T
#endif

/* OSAL components */
//#define  NXPBUILD__PH_OSAL_LPC12XX                      /**< Software OSAL for LPC12xx */
#define  NXPBUILD__PH_OSAL_RPi 		                     /**< Software OSAL for RPi */

#if defined (NXPBUILD__PH_OSAL_LPC12XX) || \
	defined (NXPBUILD__PH_OSAL_RPi)
   #define NXPBUILD__PH_OSAL                           /**< Generic OSAL definition */
#endif

/* Log component */

//#define NXPBUILD__PH_LOG

/** @}
* end of ph_NxpBuild
*/

#endif /* PH_NXPBUILD_H */

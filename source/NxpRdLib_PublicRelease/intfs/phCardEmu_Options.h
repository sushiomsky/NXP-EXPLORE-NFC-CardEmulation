/*==============================================================================================
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
 * phCardEmu_Options.h
 *
 *  Created on: 19 Feb 2013
 *      Author: Alex
 */

#ifndef PHCARDEMU_OPTIONS_H_
#define PHCARDEMU_OPTIONS_H_

// HARDWARE CONFIGURATION
#define PHCARDEMU_BLUEBOARD

// All pins on GPIO_0
    #define PHCARDEMU_IRQ_PIN        29
    #define PHCARDEMU_RESET_PIN      28

// T2T - configuration
// include the below to enable the PHDC layer on top of the T2T emulation
    #define PHCARDEMU_T2T_MSG              /* NDEF processing for Facebook application*/

    // use these definitions for the stand-alone T2T emulation
    #define PHCARDEMU_T2T_DATAMEM_SIZE  0x7F8    // For NDEF, MAX is 2040 (0x7F8),

    #define PHCARDEMU_T2T_ON_RAM			// MUST be enabled, T2T on flash is
    // not supported on this device
    // because the time to write to flash
    // is longer than FDT-T2T-WRITE
// PHDC configuration

// comment this out to let the tag always send temperature data
#define PHDC_ONE_TEMP_THEN_RELEASE

// include this to avoid random data and instead have a count-up and then count-down behaviour
#define PHDC_DETERMINISTIC_TEMPS

#endif /* PHCARDEMU_OPTIONS_H_ */

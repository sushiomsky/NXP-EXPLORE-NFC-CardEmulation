
#include <phCardEmu_Options.h>
#include <phhalHw.h>
#include "CheckNdef.h"

NdefDesc_	   NdefDesc;

/*
 * input:
 *  pointer start of T2T card memory
 *  pointer to Ndef description structure
 * output:
 *  NDEF_TEXT_WRITE - writen message is text
 *  NDEF_OTHER_WRITE - writen message have different type
 *  NDEF_NO_WRITE - no write operation
 *  NDEF_PROGRESS - ndef message in progress
 */
uint32_t CheckNdef(uint8_t *pt2t, NdefDesc_ *ndef)
    {
    uint8_t 	*pNdef;
    uint32_t	ret = NDEF_PROGRESS;

    if(num_de > 0)
        {
        if(num_wr > 0)
            {
            /* basic starting pointer */
            pNdef = pt2t + NDEF_MESSAGE_OFFSET;	// ndef flags

            /* check the TLV length and correct the actual pointer in NDEF field */
            if (*(pNdef-1) == 0xFF)
                {
                pNdef++;
                pNdef++;
                }
            ndef->typelen = *(pNdef+1);

            /* calculate the message length and set the pointer to next actual data
             * check for Short message
             */
            if(*pNdef & 0x10)
                { //short message
                ndef->paylen = *(pNdef+2);
                ndef->typestart = pNdef + 3;
                }
            else
                { // long message
                ndef->paylen = 0x1000000*(*(pNdef+2)) + 0x10000*(*(pNdef+3)) + 0x100*(*(pNdef+4)) + *(pNdef+5);
                ndef->typestart = pNdef + 6;
                }

            /* if the ID is present, correct the position of data packet */
            if(*pNdef & 0x08)		// check if ID field exist
                {// ID exist
                ndef->typestart += 1;
                ndef->paystart = ndef->typestart + 1 + *(ndef->typestart-1);
                }
            else
                {// message without ID
                ndef->paystart = ndef->typestart + 1;
                }

            /* Check the type of the NDEF message, if Text ("T") return value like a flag for
             * Facebook write down. Set pointer to the clear payload without language extension.
             */
            if((*ndef->typestart == 0x54) && (ndef->typelen == 1))
                {// text message
                ndef->textlen = ndef->paylen - *ndef->paystart - 1;
                ndef->textstart = ndef->paystart + *ndef->paystart + 1;
                ret = NDEF_TEXT_WRITE;
                }
            else
                {// other type of message
                ndef->textlen = ndef->paylen;
                ndef->textstart = ndef->paystart;
                ret = NDEF_OTHER_WRITE;
                }
            }
        else
            {
            ret = NDEF_NO_WRITE;
            }
        num_de = 0;	// reset counters
        num_wr = 0;
        num_rd = 0;
        }
    return ret;
    }

/*==============================================================================================
 *   End of File
 ---------------------------------------------------------------------------------------------*/

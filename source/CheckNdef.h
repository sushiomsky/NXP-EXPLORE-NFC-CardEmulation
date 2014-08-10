/*
 * CheckNdef.h
 *
 *  Created on: 12 Sep 2013
 *      Author: nxp52459
 */


#ifndef CHECKNDEF_H_
#define CHECKNDEF_H_

#include <phhalHw.h>

#define  NDEF_MESSAGE_OFFSET  0x12  // ndef message offset from begin of card memory

#define	NDEF_TEXT_WRITE      0
#define	NDEF_OTHER_WRITE     1
#define	NDEF_NO_WRITE        2
#define	NDEF_PROGRESS        3

typedef struct
{
	uint8_t		*typestart;		// pointer to start type of message
	uint8_t	   typelen;		   // legth of type
	uint8_t		*paystart;		// pointer to start of full payload
	uint32_t	   paylen;			// length of full payload
	uint8_t		*textstart;		// pointer to start of text without language part
	uint32_t	   textlen;		   // length of text without language part
}NdefDesc_;

extern uint16_t		num_de;
extern uint16_t		num_wr;
extern uint16_t		num_rd;
extern uint8_t 		*t2tMemory;
extern NdefDesc_	   NdefDesc;

uint32_t CheckNdef(uint8_t *t2t, NdefDesc_ *desc);

#endif /* CHECKNDEF_H_ */

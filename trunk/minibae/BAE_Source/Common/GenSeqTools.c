/*****************************************************************************/
/*
** "GenSeqTool.c"
**
**	Generalized Music Synthesis package. Support functions for GenSeq.c.
**	Should be compiled on the same CPU as GenSeq.c
**
**	© Copyright 1999 Beatnik, Inc, All Rights Reserved.
**	Written by Steve Hales and Jim Nitchals
**
**	Beatnik products contain certain trade secrets and confidential and
**	proprietary information of Beatnik.  Use, reproduction, disclosure
**	and distribution by any means are prohibited, except pursuant to
**	a written license from Beatnik. Use of copyright notice is
**	precautionary and does not imply publication or disclosure.
**
**	Restricted Rights Legend:
**	Use, duplication, or disclosure by the Government is subject to
**	restrictions as set forth in subparagraph (c)(1)(ii) of The
**	Rights in Technical Data and Computer Software clause in DFARS
**	252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial
**	Computer Software--Restricted Rights at 48 CFR 52.227-19, as
**	applicable.
**
**	Confidential-- Internal use only
**
** Overview
**	General purpose Music Synthesis software, C-only implementation (no assembly
**	language optimizations made)
**
**	Contains only MIDI Sequencer
**
**	Modification History:
**
**	12/7/99		Split from GenSeq.c
**	12/13/99	MSD: changed XGet... functions to accept byteOffset param
**	12/15/99	MSD: fixed XGetLongWordAligned
**					 Added String comparision functions.
**	3/7/2000	Fixed compiler warning in XStrnToLongWordAligned
*/
/*****************************************************************************/

#include "GenSnd.h"
#include "X_Assert.h"
#include "GenSeqTools.h"

// given a pointer, get a short int ordered in a Motorola way, and only 
// access on word bounderies
XWORD XGetShortWordAligned(XWORD *pData, long byteOffset)
{
	short	wordOffset;
	short	byteIndex;
	XWORD	result;
	
	if (byteOffset >= 0)
	{
		wordOffset = byteOffset / 2;
		byteIndex = byteOffset % 2;
	}
	else
	{
		wordOffset = -((-byteOffset + 1) / 2);
		byteIndex = (-byteOffset) % 2;
	}
	pData += wordOffset;

	if (byteIndex == 0)
	{
		#if X_WORD_ORDER == FALSE
		result = pData[0];
		#else
		result = ((pData[0] & 0x00FF) << 8) |
				 (pData[0] >> 8);
		#endif
	}
	else
	{
		#if X_WORD_ORDER == FALSE
		result = (pData[0] << 8) |
				 (pData[1] >> 8);
		#else
		result = (pData[0] & 0xFF00) |
				 (pData[1] & 0x00FF);
		#endif
	}
	return result;
}

// given a pointer, get a long ordered in a Motorola way
XDWORD XGetLongWordAligned(XWORD *pData, long byteOffset)
{
	short	wordOffset;
	short	byteIndex;
	XDWORD	result;
	
	if (byteOffset >= 0)
	{
		wordOffset = byteOffset / 2;
		byteIndex = byteOffset % 2;
	}
	else
	{
		wordOffset = -((-byteOffset + 1) / 2);
		byteIndex = (-byteOffset) % 2;
	}
	pData += wordOffset;
	
	if (byteIndex == 0)
	{
		#if X_WORD_ORDER == FALSE
		result = (pData[0] << 16) | pData[1];
		#else
		result =  (((pData[0] & 0x00FF) << 8) |
				    (pData[0] >> 8)) << 16;
		result |= (((pData[1] & 0x00FF) << 8) |
				    (pData[1] >> 8));
		#endif
	}
	else
	{
		#if X_WORD_ORDER == FALSE
		result =  ((pData[0] << 8) |
				   (pData[1] >> 8)) << 16;
		result |= ((pData[1] << 8) |
				   (pData[2] >> 8));
		#else
		result =  ((pData[0] & 0xFF00) |
				   (pData[1] & 0x00FF)) << 16;
		result |= ((pData[1] & 0xFF00) |
				   (pData[2] & 0x00FF));
		#endif
	}
	return result;
}


XWORD XGetCharWordAligned(XWORD *pData, long byteOffset)
{
	return (XGetShortWordAligned(pData, byteOffset) >> 8);
}


static short int PV_ToLower(short int c)
{
	return( ( ((c >= 'A') && (c <= 'Z')) ? c | 0x20 : c) );
}


short int XLStrnCmpWordAligned(const char* s1, XWORD *s2, long byteOffset, long n)
{
	XWORD	ch1, ch2;

	if ((s1 == NULL) || (s2 == NULL))
	{
		if ((s1 == NULL) && (s2 == NULL))
		{
			return 0;
		}
		return -1;
	}

	while (n)
	{
		ch1 = *s1;
		ch2 = XGetCharWordAligned(s2, byteOffset);

		if (ch1 == '\0') break;

		if (PV_ToLower(ch1) != PV_ToLower(ch2))
		{
			return (ch1 - ch2);
		}
		n--;
		s1++;
		byteOffset++;
	}

	return 0;
}


long XStrnToLongWordAligned(XWORD* pData, long byteOffset, long length)
{
	long	result, numDigits, count;
	XWORD	ch;

	result = 0;
	numDigits = 0;
	count = 0;

	while ((length > 0) && (numDigits < 12))
	{
		ch = XGetCharWordAligned(pData, byteOffset + count);
		if (ch == 0x20)
		{
			// ignore whitespace
		}
		else
		{
			if ((ch >= '0') && (ch <= '9'))
			{
				// found digit!
				result *= 10;
				result += (ch - '0'); 
				numDigits++;
			}
			else
			{
				// found non-digit, non-space character.
				break;
			}
		}
		count++;
		length--;
	}

	return result;
}

// EOF of GenSeqTools.c


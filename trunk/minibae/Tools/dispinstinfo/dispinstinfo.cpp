/**************************************************************************** * * dispinstinfo.cpp * * a command line tool using BAE to display instrument information. Depends * on deep knowledge of the inner workings of instruments for BAE.  * * � Copyright 2000 Beatnik, Inc, All Rights Reserved. * Written by Steve Hales (steve@beatnik.com) * * Legal Notice: *	Beatnik products contain certain trade secrets and confidential and *	proprietary information of Beatnik.  Use, reproduction, disclosure *	and distribution by any means are prohibited, except pursuant to *	a written license from Beatnik. Use of copyright notice is *	precautionary and does not imply publication or disclosure. * * Restricted Rights Legend: *	Use, duplication, or disclosure by the Government is subject to *	restrictions as set forth in subparagraph (c)(1)(ii) of The *	Rights in Technical Data and Computer Software clause in DFARS *	252.227-7013 or subparagraphs (c)(1) and (2) of the Commercial *	Computer Software--Restricted Rights at 48 CFR 52.227-19, as *	applicable. * * Confidential - Internal use only * * History: *	5/28/2000	Created * ****************************************************************************/#include <stdio.h>#include <stdlib.h>#include <windows.h>#include "BAE.h"#include "BAEFile.h"#include "BAEPrivate.h"#include <conio.h>#include "GenSnd.h"#include "GenPriv.h"#include "X_Formats.h" #include "X_Instruments.h"#include "X_EditorTools.h"// prototypeschar const usageString[] =	"USAGE: dispinstinfo <patches.hsb> [inst]\n"	"		<patches.hsb> is the instrument library to examine.\n"	"		[inst] optional instrument to display. Default is all instruments.\n";static char * PV_GetBoolText(XBOOL flag){	static char *b_false = "false";	static char *b_true = "true";	return (flag) ? b_true : b_false;}static void PV_PrintBasicInfo(GM_Instrument *theI){	if (theI)	{		printf("\tmasterRootKey %ld\n", (long)theI->masterRootKey);		printf("\tmiscParameter1 %ld\n", (long)theI->miscParameter1);		printf("\tmiscParameter2 %ld\n", (long)theI->miscParameter2);		printf("\tdisableSndLooping %s\n", PV_GetBoolText(theI->disableSndLooping));		printf("\tplayAtSampledFreq %s\n", PV_GetBoolText(theI->playAtSampledFreq));		printf("\tnotPolyphonic %s\n", PV_GetBoolText(theI->notPolyphonic));		printf("\textendedFormat %s\n", PV_GetBoolText(theI->extendedFormat));		printf("\tsampleAndHold %s\n", PV_GetBoolText(theI->sampleAndHold));		printf("\tuseSampleRate %s\n", PV_GetBoolText(theI->useSampleRate));		printf("\tuseSoundModifierAsRootKey %s\n", PV_GetBoolText(theI->useSoundModifierAsRootKey));		printf("\tavoidReverb %s\n", PV_GetBoolText(theI->avoidReverb));		printf("\tpanPlacement %ld\n", (long)theI->panPlacement);		printf("\tLPF_frequency %ld\n", (long)theI->LPF_frequency);		printf("\tLPF_resonance %ld\n", (long)theI->LPF_resonance);		printf("\tLPF_lowpassAmount %ld\n", (long)theI->LPF_lowpassAmount);	}		}static char * PV_GetEnvelopeFlagType(UNIT_TYPE type){	switch (type)	{		case ADSR_OFF:			return "ADSR_OFF\t";		case ADSR_LINEAR_RAMP:			return "ADSR_LINEAR_RAMP\t";		case ADSR_SUSTAIN:			return "ADSR_SUSTAIN\t";		case ADSR_TERMINATE:			return "ADSR_TERMINATE\t";		case ADSR_RELEASE:			return "ADSR_RELEASE\t";	}	return "\t";}static void PV_DisplayEnvelope(char *type, GM_ADSR *env){	long	count;	printf("\t %s Envelope\n", type);	printf("\t Sustaining decay level %ld (%f)\n", env->sustainingDecayLevel, 													(float)env->sustainingDecayLevel / (float)XFIXED_1);	for (count = 0; count < ADSR_STAGES; count++)	{		printf("\t %s time %ld\t\tlevel %ld\n",							PV_GetEnvelopeFlagType(env->ADSRFlags[count]),							env->ADSRTime[count],							env->ADSRLevel[count]							);	}}static void PV_DisplayInstrumentInfo(BAEFileResource *file, BAE_INSTRUMENT instrument){	BAEResult			err;	char				name[256];	GM_Instrument		*iData;	OPErr				opErr;	unsigned long		memoryRequired = 0;	err = file->GetResourceName(ID_INST, (XLongResourceID)instrument,										name, 255);	if (err == BAE_NO_ERROR)	{		printf("Details of instrument '%s' id %ld\n", name, instrument);		iData = PV_GetInstrument((XLongResourceID)instrument,								(XBankToken)file->GetFileReference(),								NULL,								0,								&opErr);		if (iData)		{			iData->usageReferenceCount++;		// increment reference count			// dis-assemble the instrument			// add the first level of the instrument.			memoryRequired += XGetPtrSize((XPTR)iData);			PV_PrintBasicInfo(iData);			PV_DisplayEnvelope("Volume", &iData->volumeADSRRecord);			if (iData->doKeymapSplit)			{			}			else			{				// single instrument			}			opErr = PV_UnloadInstrumentData(iData, GM_GetCurrentMixer(), TRUE);		}	}}// main()// ---------------------------------------------------------------------int main(int argc, char* argv[]){	BAEOutputMixer		*mixer;	BAEResult			err;	BAEFileResource		*file;	long				count;	char				*filePath;	unsigned long		totalByteSize;	if (argc < 2)	{		printf(usageString);		return -1;	}		filePath = argv[1];		printf("Beatnik Audio Engine instrument display\n");	printf("BAE Copyright 2000, Beatnik, Inc\n\n");	totalByteSize = 0;	mixer = new BAEOutputMixer();	if (mixer)	{		mixer->Open();	}	file = new BAEFileResource((BAEPathName)filePath, BAE_FILE_READ, &err);	if (file)	{		if (err == BAE_NO_ERROR)		{			err = BAE_UseThisFile((XFILE)file->GetFileReference(), FALSE);			if (err == BAE_NO_ERROR)			{				totalByteSize = 0;				if (argc == 3)				{					PV_DisplayInstrumentInfo(file, (BAE_INSTRUMENT)atoi(argv[2]));				}				else				{					for (count = 0; count < MAX_INSTRUMENTS * MAX_BANKS; count++)					{						PV_DisplayInstrumentInfo(file, (BAE_INSTRUMENT)count);					}				}			}		}		delete file;		if (mixer)		{			delete mixer;		}	}	printf("\nPress a key to exit\n");	while( !_kbhit() )	{		Sleep(1);	}	return 0;}// EOF#if 0static void PV_GetUnitTypeName(long unitType, char *typeName){	switch (unitType)	{		case X_ADSR:			XStrCpy(typeName, "Volume Envelope (ADSR)");			break;		case X_DEFAULT_MOD:			XStrCpy(typeName, "Use Default MOD wheel behavior (DMOD)");			break;					case X_CURVE:					// curve entry			XStrCpy(typeName, "Curve Entry (CURV)");			break;		case X_LOW_PASS_FILTER:		// low pass global filter parameters			XStrCpy(typeName, "Low pass filter enabled (LPGF)");			break;		case X_VOLUME_LFO:			XStrCpy(typeName, "Volume LFO (VOLU)");			break;		case X_PITCH_LFO:			XStrCpy(typeName, "Pitch LFO (PITC)");			break;		case X_STEREO_PAN_LFO:		case X_STEREO_PAN_NAME2:			XStrCpy(typeName, "Stereo placement LFO (SPAN)");			break;		case X_LOW_PASS_AMOUNT:			XStrCpy(typeName, "Low Pass Amount LFO (LPAM)");			break;		case X_LPF_DEPTH:			XStrCpy(typeName, "Low Pass Resonance LFO (LPRE)");			break;		case X_LPF_FREQUENCY:			XStrCpy(typeName, "Low Pass Resonant Frequency LFO (LPFR)");			break;	}}void ExportInstrumentTextInformation(FSSpec *pFile, void *pName){	InstrumentResource **theX;	short int			count, totalInstruments, theID, count2, count3;	short int			instArray[MAX_INSTRUMENTS * MAX_BANKS];	char				fileName[64];	short			volumeRef, fileRef;	OSErr			theErr;	char				text[256], name[256];	short int			samples[128];	XInstrument		*theI;	short int			maxADSRPoints;	short int			maxLFOs, localLFO;	short int			maxCurves;	pStrcpy(fileName, (char *)pName);	pStrcat(fileName, (char *)"\p info");	if (GetWriteFileName(fileName, &volumeRef, "\pSave instrument info as:"))	{		if (VOpenOrchestra(pFile) == noErr)		{			SetCursorType(watchCursor);				totalInstruments = XGetInstrumentArray(instArray, MAX_INSTRUMENTS * MAX_BANKS);			if (totalInstruments)			{				SetCursorType(watchCursor);				theErr = FSNewDataOnly(fileName, &fileRef, volumeRef, 'ttxt', ID_TEXT);				if (theErr == noErr)				{					FormatText(text, "Instrument information from �%p.�\rThere are %ld instruments.\r", 										pFile->name, (long)totalInstruments);					FSWriteCString(fileRef, text);					theI = (XInstrument *)NewPtrClear((long)sizeof(XInstrument));					if (theI)					{						maxADSRPoints = 0;						maxLFOs = 0;						maxCurves = 0;						for (count = 0; count < totalInstruments; count++)						{							localLFO = 0;							theID = instArray[count];							GetNameFromResourceID(ID_INST, theID, name);								theX = (InstrumentResource **)GetResource(ID_INST, theID);							if (theX)							{								FormatText(text, "INST �%p� ID %ld:\r", name, theID);								FSWriteCString(fileRef, text);								XSetMemory((void *)theI, (long)sizeof(XInstrument), 0);								PV_GetXInstrument(theX, theI);								ReleaseResource((Handle)theX);								FormatText(text, "  There are %ld unit types.\r", theI->unitCount);								FSWriteCString(fileRef, text);								for (count2 = 0; count2 < theI->unitCount; count2++)								{									PV_GetUnitTypeName(theI->units[count2].unitType, name);									FormatText(text, "    %s", name);									FSWriteCString(fileRef, text);									switch (theI->units[count2].unitType)									{										case X_ADSR:											PV_RemoveZeroEnvelope(&theI->units[count2].u.envelopeADSR);		// remove extra 0 time point from editing											FormatText(text, " has %ld points.\r", theI->units[count2].u.envelopeADSR.stageCount);											if (theI->units[count2].u.envelopeADSR.stageCount > maxADSRPoints)											{												maxADSRPoints = theI->units[count2].u.envelopeADSR.stageCount;											}											FSWriteCString(fileRef, text);											break;										case X_DEFAULT_MOD:											FSWriteCString(fileRef, "\r");											break;										case X_CURVE:											FormatText(text, " has %ld points.\r", theI->units[count2].u.curve.curveCount);											if (theI->units[count2].u.curve.curveCount > maxCurves)											{												maxCurves = theI->units[count2].u.curve.curveCount;											}											FSWriteCString(fileRef, text);											break;										case X_LOW_PASS_FILTER:											FSWriteCString(fileRef, "\r");											break;										case X_VOLUME_LFO:										case X_PITCH_LFO:										case X_STEREO_PAN_LFO:										case X_STEREO_PAN_NAME2:										case X_LOW_PASS_AMOUNT:										case X_LPF_DEPTH:										case X_LPF_FREQUENCY:											PV_RemoveZeroEnvelope(&theI->units[count2].u.lfo.envelopeLFO);		// remove extra 0 time point from editing											FormatText(text, " has %ld points.\r", theI->units[count2].u.lfo.envelopeLFO.stageCount);											if (theI->units[count2].u.lfo.envelopeLFO.stageCount > maxADSRPoints)											{												maxADSRPoints = theI->units[count2].u.lfo.envelopeLFO.stageCount;											}											FSWriteCString(fileRef, text);											localLFO++;											break;									}									}							}							if (localLFO > maxLFOs)							{								maxLFOs = localLFO;							}							count2 =  XCollectSoundsFromInstrumentID(theID, samples, 128);							FormatText(text, "  Total of %ld samples. Uses the following samples: ", (long)count2);							FSWriteCString(fileRef, text);							for (count3 = 0; count3 < count2; count3++)							{								FormatText(text, "%ld ", samples[count3]);								FSWriteCString(fileRef, text);							}							FSWriteCString(fileRef, "\r\r");						}						FormatText(text, "\r���� Max LFO's %ld\r", (long)maxLFOs);						FSWriteCString(fileRef, text);						FormatText(text, "���� Max Envelope points %ld\r", (long)maxADSRPoints);						FSWriteCString(fileRef, text);												FormatText(text, "���� Max curve points %ld\r", (long)maxCurves);						FSWriteCString(fileRef, text);					}					XDisposePtr((XPTR)theI);					FSWriteCString(fileRef, "\r\r");/*					// now write out sample information					FormatText(text, "Sample information from �%p.�\rThere are %ld samples.\r", 										pFile->name, (long)totalInstruments);*/									FSClose(fileRef);					FlushVol(NULL, volumeRef);				}			}		}		SetCursorType(arrowCursor);	}	VCloseOrchestra();}#endif
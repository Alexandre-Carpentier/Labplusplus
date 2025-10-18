/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _VISA_H__
#define _VISA_H__
//#pragma comment (lib, "visa32.lib") 
#include <visa.h> 

#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include <cassert>

#include "cProtocol.h"
#include "encoding.h"

/*- Resource Manager Functions and Operations -------------------------------*/
typedef ViStatus(*viOpenDefaultRM_)(ViPSession vi);
typedef ViStatus(*viFindRsrc_)(ViSession sesn, ViConstString expr, ViPFindList vi,
	ViPUInt32 retCnt, ViChar _VI_FAR desc[]);
typedef ViStatus(*viFindNext_)(ViFindList vi, ViChar _VI_FAR desc[]);
typedef ViStatus(*viParseRsrc_)(ViSession rmSesn, ViConstRsrc rsrcName,
	ViPUInt16 intfType, ViPUInt16 intfNum);
typedef ViStatus(*viParseRsrcEx_)(ViSession rmSesn, ViConstRsrc rsrcName, ViPUInt16 intfType,
	ViPUInt16 intfNum, ViChar _VI_FAR rsrcClass[],
	ViChar _VI_FAR expandedUnaliasedName[],
	ViChar _VI_FAR aliasIfExists[]);
typedef ViStatus(*viOpen_)(ViSession sesn, ViConstRsrc name, ViAccessMode mode,
	ViUInt32 timeout, ViPSession vi);
/*- Resource Template Operations --------------------------------------------*/
typedef ViStatus(*viClose_)(ViObject vi);
typedef ViStatus(*viSetAttribute_)(ViObject vi, ViAttr attrName, ViAttrState attrValue);
typedef ViStatus(*viGetAttribute_)(ViObject vi, ViAttr attrName, void _VI_PTR attrValue);
typedef ViStatus(*viStatusDesc_)(ViObject vi, ViStatus status, ViChar _VI_FAR desc[]);
typedef ViStatus(*viTerminate_)(ViObject vi, ViUInt16 degree, ViJobId jobId);
typedef ViStatus(*viLock_)(ViSession vi, ViAccessMode lockType, ViUInt32 timeout,
	ViConstKeyId requestedKey, ViChar _VI_FAR accessKey[]);
typedef ViStatus(*viUnlock_)(ViSession vi);
typedef ViStatus(*viEnableEvent_)(ViSession vi, ViEventType eventType, ViUInt16 mechanism,
	ViEventFilter context);
typedef ViStatus(*viDisableEvent_)(ViSession vi, ViEventType eventType, ViUInt16 mechanism);
typedef ViStatus(*viDiscardEvents_)(ViSession vi, ViEventType eventType, ViUInt16 mechanism);

/*- Basic I/O Operations ----------------------------------------------------*/
typedef ViStatus(*viRead_)(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus(*viReadToFile_)(ViSession vi, ViConstString filename, ViUInt32 cnt,
	ViPUInt32 retCnt);
typedef ViStatus(*viWrite_)(ViSession vi, ViConstBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus(*viWriteFromFile_)(ViSession vi, ViConstString filename, ViUInt32 cnt,
	ViPUInt32 retCnt);
typedef ViStatus(*viAssertTrigger_)(ViSession vi, ViUInt16 protocol);
typedef ViStatus(*viReadSTB_)(ViSession vi, ViPUInt16 status);
typedef ViStatus(*viClear_)(ViSession vi);

typedef ViStatus(*viSetBuf_)(ViSession vi, ViUInt16 mask, ViUInt32 size);
typedef ViStatus(*viFlush_)(ViSession vi, ViUInt16 mask);
typedef ViStatus(*viBufWrite_)(ViSession vi, ViConstBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus(*viBufRead_)(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus(*viPrintf_)(ViSession vi, ViConstString writeFmt, ...);
typedef ViStatus(*viVPrintf_)(ViSession vi, ViConstString writeFmt, ViVAList params);
typedef ViStatus(*viSPrintf_)(ViSession vi, ViPBuf buf, ViConstString writeFmt, ...);
typedef ViStatus(*viVSPrintf_)(ViSession vi, ViPBuf buf, ViConstString writeFmt,
	ViVAList params);
//- Resource Manager Functions and Operations -------------------------------
extern viOpenDefaultRM_ mviOpenDefaultRM;
extern viFindRsrc_ mviFindRsrc;
extern viFindNext_ mviFindNext;
extern viParseRsrc_ mviParseRsrc;
extern viOpen_ mviOpen;
//- Resource Template Operations --------------------------------------------
extern viClose_ mviClose;
extern viSetAttribute_ mviSetAttribute;
extern viGetAttribute_ mviGetAttribute;
extern viStatusDesc_ mviStatusDesc;
extern viTerminate_ mviTerminate;
extern viLock_ mviLock;
extern viUnlock_ mviUnlock;

//- Basic I/O Operations ----------------------------------------------------
extern viRead_ mviRead;
extern viReadToFile_ mviReadToFile;
extern viWrite_ mviWrite;
extern viWriteFromFile_ mviWriteFromFile;
extern viAssertTrigger_ mviAssertTrigger;
extern viReadSTB_ mviReadSTB;
extern viClear_ mviClear;

extern viSetBuf_ mviSetBuf;
extern viFlush_ mviFlush;
extern viBufWrite_ mviBufWrite;
extern viBufRead_ mviBufRead;
extern viPrintf_ mviPrintf;
extern viSPrintf_ mviSPrintf;
extern viVSPrintf_ mviVSPrintf;

// A Visa class is a protocol
class cVisa : public IProtocol {
public:
	cVisa();
	err_struct init() override;
	err_struct write(std::wstring scpi);
	err_struct read(std::wstring& scpi);
	err_struct close()override;

	err_struct last_error;

	ViSession ressource_manager = 0;
	ViSession device_ = 0;
	ViStatus status = 0;
};

#endif
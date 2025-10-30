#pragma once
#include <visa.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <print>

/*- Resource Manager Functions and Operations -------------------------------*/
typedef ViStatus (*viOpenDefaultRM_)(ViPSession vi);
typedef ViStatus (*viFindRsrc_)(ViSession sesn, ViConstString expr, ViPFindList vi,
	ViPUInt32 retCnt, ViChar _VI_FAR desc[]);
typedef ViStatus (*viFindNext_)(ViFindList vi, ViChar _VI_FAR desc[]);
typedef ViStatus (*viParseRsrc_)(ViSession rmSesn, ViConstRsrc rsrcName,
	ViPUInt16 intfType, ViPUInt16 intfNum);
typedef ViStatus (*viParseRsrcEx_)(ViSession rmSesn, ViConstRsrc rsrcName, ViPUInt16 intfType,
	ViPUInt16 intfNum, ViChar _VI_FAR rsrcClass[],
	ViChar _VI_FAR expandedUnaliasedName[],
	ViChar _VI_FAR aliasIfExists[]);
typedef ViStatus (*viOpen_)(ViSession sesn, ViConstRsrc name, ViAccessMode mode,
	ViUInt32 timeout, ViPSession vi);
/*- Resource Template Operations --------------------------------------------*/
typedef ViStatus (*viClose_)(ViObject vi);
typedef ViStatus (*viSetAttribute_)(ViObject vi, ViAttr attrName, ViAttrState attrValue);
typedef ViStatus (*viGetAttribute_)(ViObject vi, ViAttr attrName, void _VI_PTR attrValue);
typedef ViStatus (*viStatusDesc_)(ViObject vi, ViStatus status, ViChar _VI_FAR desc[]);
typedef ViStatus (*viTerminate_)(ViObject vi, ViUInt16 degree, ViJobId jobId);
typedef ViStatus (*viLock_)(ViSession vi, ViAccessMode lockType, ViUInt32 timeout,
	ViConstKeyId requestedKey, ViChar _VI_FAR accessKey[]);
typedef ViStatus (*viUnlock_)(ViSession vi);
typedef ViStatus (*viEnableEvent_)(ViSession vi, ViEventType eventType, ViUInt16 mechanism,
	ViEventFilter context);
typedef ViStatus (*viDisableEvent_)(ViSession vi, ViEventType eventType, ViUInt16 mechanism);
typedef ViStatus (*viDiscardEvents_)(ViSession vi, ViEventType eventType, ViUInt16 mechanism);

/*- Basic I/O Operations ----------------------------------------------------*/
typedef ViStatus (*viRead_)(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus (*viReadToFile_)(ViSession vi, ViConstString filename, ViUInt32 cnt,
	ViPUInt32 retCnt);
typedef ViStatus (*viWrite_)(ViSession vi, ViConstBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus (*viWriteFromFile_)(ViSession vi, ViConstString filename, ViUInt32 cnt,
	ViPUInt32 retCnt);
typedef ViStatus (*viAssertTrigger_)(ViSession vi, ViUInt16 protocol);
typedef ViStatus (*viReadSTB_)(ViSession vi, ViPUInt16 status);
typedef ViStatus (*viClear_)(ViSession vi);

typedef ViStatus (*viSetBuf_)(ViSession vi, ViUInt16 mask, ViUInt32 size);
typedef ViStatus (*viFlush_)(ViSession vi, ViUInt16 mask);
typedef ViStatus (*viBufWrite_)(ViSession vi, ViConstBuf  buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus (*viBufRead_)(ViSession vi, ViPBuf buf, ViUInt32 cnt, ViPUInt32 retCnt);
typedef ViStatus (*viPrintf_)(ViSession vi, ViConstString writeFmt, ...);
typedef ViStatus (*viVPrintf_)(ViSession vi, ViConstString writeFmt, ViVAList params);
typedef ViStatus (*viSPrintf_)(ViSession vi, ViPBuf buf, ViConstString writeFmt, ...);
typedef ViStatus (*viVSPrintf_)(ViSession vi, ViPBuf buf, ViConstString writeFmt,
	ViVAList params);


//- Resource Manager Functions and Operations -------------------------------
viOpenDefaultRM_ mviOpenDefaultRM;
viFindRsrc_ mviFindRsrc;
viFindNext_ mviFindNext;
viParseRsrc_ mviParseRsrc;
viOpen_ mviOpen;
//- Resource Template Operations --------------------------------------------
viClose_ mviClose;
viSetAttribute_ mviSetAttribute;
viGetAttribute_ mviGetAttribute;
viStatusDesc_ mviStatusDesc;
viTerminate_ mviTerminate;
viLock_ mviLock;
viUnlock_ mviUnlock;

//- Basic I/O Operations ----------------------------------------------------
viRead_ mviRead;
viReadToFile_ mviReadToFile;
viWrite_ mviWrite;
viWriteFromFile_ mviWriteFromFile;
viAssertTrigger_ mviAssertTrigger;
viReadSTB_ mviReadSTB;
viClear_ mviClear;

viSetBuf_ mviSetBuf;
viFlush_ mviFlush;
viBufWrite_ mviBufWrite;
viBufRead_ mviBufRead;
viPrintf_ mviPrintf;
viSPrintf_ mviSPrintf;
viVSPrintf_ mviVSPrintf;

//HMODULE hVisa = nullptr;


bool load_ni_visa_dll(HMODULE *hVisa)
{
	*hVisa = LoadLibraryA("visa32.dll");
	if (*hVisa == NULL)
	{
		// Handle the error if the DLL fails to load
		DWORD error = GetLastError();
		std::cerr << "Failed to load visa32.dll. Error code: " << error << std::endl;
		return false;
	}
	else
	{
		std::cout << "[*] visa32.dll loaded successfully." << std::endl;
	}
	return true;
}

bool load_ni_visa_ptrs(HMODULE* hVisa)
{
	if (*hVisa == NULL)
	{
		// Handle the error if the HMODULE is NULL
		DWORD error = GetLastError();
		std::cerr << "Failed to load visa32.dll. Error code: " << error << std::endl;
		return false;
	}

	mviOpenDefaultRM = (viOpenDefaultRM_)GetProcAddress(*hVisa, "viOpenDefaultRM");
	if (!mviOpenDefaultRM) { std::cerr << "Failed to get address for viOpenDefaultRM" << std::endl; return false; }
	std::print("[*] viOpenDefaultRM address loaded: {}\n", (void*)mviOpenDefaultRM);
	mviFindRsrc = (viFindRsrc_)GetProcAddress(*hVisa, "viFindRsrc");
	if (!mviFindRsrc) { std::cerr << "Failed to get address for viFindRsrc" << std::endl; return false; }
	std::print("[*] viFindRsrc address loaded: {}\n", (void*)mviFindRsrc);
	mviFindNext = (viFindNext_)GetProcAddress(*hVisa, "viFindNext");
	if (!mviFindNext) { std::cerr << "Failed to get address for viFindNext" << std::endl; return false; }
	std::print("[*] viFindNext address loaded: {}\n", (void*)mviFindNext);
	mviParseRsrc = (viParseRsrc_)GetProcAddress(*hVisa, "viParseRsrc");
	if (!mviParseRsrc) { std::cerr << "Failed to get address for viParseRsrc" << std::endl; return false; }
	std::print("[*] viParseRsrc address loaded: {}\n", (void*)mviParseRsrc);
	mviOpen= (viOpen_)GetProcAddress(*hVisa, "viOpen");
	if (!mviOpen) { std::cerr << "Failed to get address for viOpen" << std::endl; return false; }
	std::print("[*] viOpen address loaded: {}\n", (void*)mviOpen);
	mviClose = (viClose_)GetProcAddress(*hVisa, "viClose");
	if (!mviClose) { std::cerr << "Failed to get address for viClose" << std::endl; return false; }
	std::print("[*] viClose address loaded: {}\n", (void*)mviClose);
	mviSetAttribute = (viSetAttribute_)GetProcAddress(*hVisa, "viSetAttribute");
	if (!mviSetAttribute) { std::cerr << "Failed to get address for viSetAttribute" << std::endl; return false; }
	std::print("[*] viSetAttribute address loaded: {}\n", (void*)mviSetAttribute);	
	mviGetAttribute = (viGetAttribute_)GetProcAddress(*hVisa, "viGetAttribute");
	if (!mviGetAttribute) { std::cerr << "Failed to get address for viGetAttribute" << std::endl; return false; }
	std::print("[*] viGetAttribute address loaded: {}\n", (void*)mviGetAttribute);
	mviStatusDesc = (viStatusDesc_)GetProcAddress(*hVisa, "viStatusDesc");		
	if (!mviStatusDesc) { std::cerr << "Failed to get address for viStatusDesc" << std::endl; return false; }
	std::print("[*] viStatusDesc address loaded: {}\n", (void*)mviStatusDesc);
	mviTerminate= (viTerminate_)GetProcAddress(*hVisa, "viTerminate");
	if (!mviTerminate) { std::cerr << "Failed to get address for viTerminate" << std::endl; return false; }
	std::print("[*] viTerminate address loaded: {}\n", (void*)mviTerminate);
	mviLock = (viLock_)GetProcAddress(*hVisa, "viLock");
	if (!mviLock) { std::cerr << "Failed to get address for viLock" << std::endl; return false; }
	std::print("[*] viLock address loaded: {}\n", (void*)mviLock);
	mviUnlock = (viUnlock_)GetProcAddress(*hVisa, "viUnlock");
	if (!mviUnlock) { std::cerr << "Failed to get address for viUnlock" << std::endl; return false; }
	std::print("[*] viUnlock address loaded: {}\n", (void*)mviUnlock);
	

	mviRead = (viRead_)GetProcAddress(*hVisa, "viRead");
	if (!mviRead) { std::cerr << "Failed to get address for viRead" << std::endl; return false; }
	std::print("[*] viRead address loaded: {}\n", (void*)mviRead);
	mviReadToFile = (viReadToFile_)GetProcAddress(*hVisa, "viReadToFile");
	if (!mviReadToFile) { std::cerr << "Failed to get address for viReadToFile" << std::endl; return false; }
	std::print("[*] viReadToFile address loaded: {}\n", (void*)mviReadToFile);
	mviWrite = (viWrite_)GetProcAddress(*hVisa, "viWrite");
	if (!mviWrite) { std::cerr << "Failed to get address for viWrite" << std::endl; return false; }
	std::print("[*] viWrite address loaded: {}\n", (void*)mviWrite);
	mviWriteFromFile = (viWriteFromFile_)GetProcAddress(*hVisa, "viWriteFromFile");
	if (!mviWriteFromFile) { std::cerr << "Failed to get address for viWriteFromFile" << std::endl; return false; }
	std::print("[*] viWriteFromFile address loaded: {}\n", (void*)mviWriteFromFile);
	mviAssertTrigger = (viAssertTrigger_)GetProcAddress(*hVisa, "viAssertTrigger");
	if (!mviAssertTrigger) { std::cerr << "Failed to get address for viAssertTrigger" << std::endl; return false; }
	std::print("[*] viAssertTrigger address loaded: {}\n", (void*)mviAssertTrigger);
	mviReadSTB = (viReadSTB_)GetProcAddress(*hVisa, "viReadSTB");
	if (!mviReadSTB) { std::cerr << "Failed to get address for viReadSTB" << std::endl; return false; }
	std::print("[*] viReadSTB address loaded: {}\n", (void*)mviReadSTB);
	mviClear = (viClear_)GetProcAddress(*hVisa, "viClear");
	if (!mviClear) { std::cerr << "Failed to get address for viClear" << std::endl; return false; }
	std::print("[*] viClear address loaded: {}\n", (void*)mviClear);

	mviSetBuf = (viSetBuf_)GetProcAddress(*hVisa, "viSetBuf");
	if (!mviSetBuf) { std::cerr << "Failed to get address for viSetBuf" << std::endl; return false; }
	std::print("[*] viSetBuf address loaded: {}\n", (void*)mviSetBuf);
	mviFlush = (viFlush_)GetProcAddress(*hVisa, "viFlush");
	if (!mviFlush) { std::cerr << "Failed to get address for viFlush" << std::endl; return false; }
	std::print("[*] viFlush address loaded: {}\n", (void*)mviFlush);
	mviBufWrite = (viBufWrite_)GetProcAddress(*hVisa, "viBufWrite");
	if (!mviBufWrite) { std::cerr << "Failed to get address for viBufWrite" << std::endl; return false; }
	std::print("[*] viBufWrite address loaded: {}\n", (void*)mviBufWrite);
	
	mviBufRead = (viBufRead_)GetProcAddress(*hVisa, "viBufRead");
	if (!mviBufRead) { std::cerr << "Failed to get address for viBufRead" << std::endl; return false; }
	std::print("[*] viBufRead address loaded: {}\n", (void*)mviBufRead);
	mviPrintf = (viPrintf_)GetProcAddress(*hVisa, "viPrintf");
	if (!mviPrintf) { std::cerr << "Failed to get address for viPrintf" << std::endl; return false; }
	std::print("[*] viPrintf address loaded: {}\n", (void*)mviPrintf);
	mviSPrintf = (viSPrintf_)GetProcAddress(*hVisa, "viSPrintf");
	if (!mviSPrintf) { std::cerr << "Failed to get address for viSPrintf" << std::endl; return false; }
	std::print("[*] viSPrintf address loaded: {}\n", (void*)mviSPrintf);
	mviVSPrintf = (viVSPrintf_)GetProcAddress(*hVisa, "viVSPrintf");
	if (!mviVSPrintf) { std::cerr << "Failed to get address for viVSPrintf" << std::endl; return false; }
	std::print("[*] viVSPrintf address loaded: {}\n", (void*)mviVSPrintf);

	return true;
}

bool unload_ni_visa_dll(HMODULE* hVisa)
{
	if (*hVisa == NULL)
	{
		// Handle the error if the DLL fails to load
		DWORD error = GetLastError();
		std::cerr << "Failed to load visa32.dll. Error code: " << error << std::endl;
		return false;
	}

	FreeLibrary(*hVisa);
	return true;
}

/*
class visa_ptr_singleton
{
public:
	visa_ptr_singleton()
	{
		instance_ = new visa_ptr_singleton();
		hVisa = nullptr;
	}
	~visa_ptr_singleton()
	{
		if (instance_ != nullptr)
		{
			delete instance_;
			instance_ = nullptr;
			hVisa = nullptr;
		}
	}
	static visa_ptr_singleton* getInstance()
	{
		if (instance_ == nullptr)
		{
			instance_ = new visa_ptr_singleton();
			hVisa = nullptr;
		}
		return instance_;
	}
	static bool load_visa()
	{
		if (!load_ni_visa_dll(&hVisa))
		{
			std::print("[!] Failed to load NI visa32.dll..\n");
			return false;
		}

		if (!load_ni_visa_ptrs(&hVisa))
		{
			std::print("[!] Failed to load NI visa32.dll functions..\n");
			return false;
		}
		return true;
	}
private:
	static visa_ptr_singleton* instance_;
	static HMODULE hVisa;
};
*/
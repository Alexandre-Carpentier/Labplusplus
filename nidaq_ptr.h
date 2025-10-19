#pragma once
// Must Install Nationnal Instrument DAQmx Lib freely available here:
// https://www.ni.com/fr-fr/support/downloads/drivers/download.ni-daqmx.html#460239
// Default instalation path is located at :
// C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\include
#include "C:\\Program Files (x86)\\National Instruments\\Shared\\ExternalCompilerSupport\\C\\include\\NIDAQmx.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <print>

/*- Resource Manager Functions and Operations -------------------------------*/
//typedef ViStatus(*viOpenDefaultRM_)(ViPSession vi);
/*
int32 __CFUNC     DAQmxSelfTestDevice(const char deviceName[]);
int32 __CFUNC DAQmxGetDevAISupportedMeasTypes(const char device[], int32* data, uInt32 arraySizeInElements);
int32 __CFUNC DAQmxGetDevProductType(const char device[], char* data, uInt32 bufferSize);
int32 __CFUNC DAQmxGetDevChassisModuleDevNames(const char device[], char* data, uInt32 bufferSize);
int32 __CFUNC     DAQmxCreateTask(const char taskName[], TaskHandle* taskHandle);
int32 __CFUNC     DAQmxCreateAIVoltageChan(TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], int32 terminalConfig, float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);

int32 __CFUNC     DAQmxStartTask(TaskHandle taskHandle);
int32 __CFUNC     DAQmxStopTask(TaskHandle taskHandle);
int32 __CFUNC     DAQmxClearTask(TaskHandle taskHandle);

int32 __CFUNC     DAQmxCreateAIThrmcplChan(TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, int32 thermocoupleType, int32 cjcSource, float64 cjcVal, const char cjcChannel[]);
int32 __CFUNC     DAQmxCreateAIThrmstrChanIex(TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, int32 resistanceConfig, int32 currentExcitSource, float64 currentExcitVal, float64 a, float64 b, float64 c);
int32 __CFUNC     DAQmxCreateDIChan(TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);
int32 __CFUNC     DAQmxCreateDOChan(TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);

//*** Set/Get functions for DAQmx_Task_Channels ***
int32 __CFUNC DAQmxGetTaskChannels(TaskHandle taskHandle, char* data, uInt32 bufferSize);
int32 __CFUNC DAQmxGetAIMeasType(TaskHandle taskHandle, const char channel[], int32* data);
int32 __CFUNC     DAQmxReadAnalogF64(TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, float64 readArray[], uInt32 arraySizeInSamps, int32* sampsPerChanRead, bool32* reserved);
int32 __CFUNC     DAQmxReadDigitalLines(TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, uInt8 readArray[], uInt32 arraySizeInBytes, int32* sampsPerChanRead, int32* numBytesPerSamp, bool32* reserved);
int32 __CFUNC     DAQmxWriteDigitalLines(TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, const uInt8 writeArray[], int32* sampsPerChanWritten, bool32* reserved);

int32 __CFUNC DAQmxGetTaskNumChans(TaskHandle taskHandle, uInt32* data);
*/


typedef int32(*DAQmxSelfTestDevice_)(const char deviceName[]);
typedef int32(*DAQmxGetDevAISupportedMeasTypes_)(const char device[], int32* data, uInt32 arraySizeInElements);
typedef int32(*DAQmxGetDevProductType_)(const char device[], char* data, uInt32 bufferSize);
typedef int32(*DAQmxGetDevChassisModuleDevNames_)(const char device[], char* data, uInt32 bufferSize);
typedef int32(*DAQmxCreateTask_)(const char taskName[], TaskHandle* taskHandle);
typedef int32(*DAQmxCreateAIVoltageChan_)(TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], int32 terminalConfig, float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);
typedef int32(*DAQmxStartTask_)(TaskHandle taskHandle);
typedef int32(*DAQmxStopTask_)(TaskHandle taskHandle);
typedef int32(*DAQmxClearTask_)(TaskHandle taskHandle);
typedef int32(*DAQmxCreateAIThrmcplChan_)(TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, int32 thermocoupleType, int32 cjcSource, float64 cjcVal, const char cjcChannel[]);
typedef int32(*DAQmxCreateAIThrmstrChanIex_)(TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], float64 minVal, float64 maxVal, int32 units, int32 resistanceConfig, int32 currentExcitSource, float64 currentExcitVal, float64 a, float64 b, float64 c);
typedef int32(*DAQmxCreateDIChan_)(TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);
typedef int32(*DAQmxCreateDOChan_)(TaskHandle taskHandle, const char lines[], const char nameToAssignToLines[], int32 lineGrouping);
typedef int32(*DAQmxGetTaskChannels_)(TaskHandle taskHandle, char* data, uInt32 bufferSize);
typedef int32(*DAQmxGetAIMeasType_)(TaskHandle taskHandle, const char channel[], int32* data);
typedef int32(*DAQmxReadAnalogF64_)(TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, float64 readArray[], uInt32 arraySizeInSamps, int32* sampsPerChanRead, bool32* reserved);
typedef int32(*DAQmxReadDigitalLines_)(TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, uInt8 readArray[], uInt32 arraySizeInBytes, int32* sampsPerChanRead, int32* numBytesPerSamp, bool32* reserved);
typedef int32(*DAQmxWriteDigitalLines_)(TaskHandle taskHandle, int32 numSampsPerChan, bool32 autoStart, float64 timeout, bool32 dataLayout, const uInt8 writeArray[], int32* sampsPerChanWritten, bool32* reserved);
typedef int32(*DAQmxGetTaskNumChans_)(TaskHandle taskHandle, uInt32* data);

DAQmxSelfTestDevice_ mDAQmxSelfTestDevice;
DAQmxGetDevAISupportedMeasTypes_ mDAQmxGetDevAISupportedMeasTypes;
DAQmxGetDevProductType_ mDAQmxGetDevProductType;
DAQmxGetDevChassisModuleDevNames_ mDAQmxGetDevChassisModuleDevNames;
DAQmxCreateTask_ mDAQmxCreateTask;
DAQmxCreateAIVoltageChan_ mDAQmxCreateAIVoltageChan;
DAQmxStartTask_ mDAQmxStartTask;
DAQmxStopTask_ mDAQmxStopTask;
DAQmxClearTask_ mDAQmxClearTask;
DAQmxCreateAIThrmcplChan_ mDAQmxCreateAIThrmcplChan;
DAQmxCreateAIThrmstrChanIex_ mDAQmxCreateAIThrmstrChanIex;
DAQmxCreateDIChan_ mDAQmxCreateDIChan;
DAQmxCreateDOChan_ mDAQmxCreateDOChan;
DAQmxGetTaskChannels_ mDAQmxGetTaskChannels;
DAQmxGetAIMeasType_ mDAQmxGetAIMeasType;
DAQmxReadAnalogF64_ mDAQmxReadAnalogF64;
DAQmxReadDigitalLines_ mDAQmxReadDigitalLines;
DAQmxWriteDigitalLines_ mDAQmxWriteDigitalLines;
DAQmxGetTaskNumChans_ mDAQmxGetTaskNumChans;

bool load_ni_daq_dll(HMODULE* hNidaq)
{
	*hNidaq = LoadLibraryA("nicaiu.dll");
	if (*hNidaq == NULL)
	{
		// Handle the error if the DLL fails to load
		DWORD error = GetLastError();
		std::cerr << "Failed to load hNidaq.dll. Error code: " << error << std::endl;
		return false;
	}
	else
	{
		std::cout << "[*] hNidaq.dll loaded successfully." << std::endl;
	}
	return true;
}

bool load_ni_daq_ptrs(HMODULE* hNidaq)
{
	if (*hNidaq == NULL)
	{
		// Handle the error if the HMODULE is NULL
		DWORD error = GetLastError();
		std::cerr << "Failed to load nicaiu.dll. Error code: " << error << std::endl;
		return false;
	}
	mDAQmxSelfTestDevice = (DAQmxSelfTestDevice_)GetProcAddress(*hNidaq, "DAQmxSelfTestDevice");
	if (!mDAQmxSelfTestDevice) { std::cerr << "Failed to get address for DAQmxSelfTestDevice" << std::endl; return false; }
	std::print("[*] DAQmxSelfTestDevice address loaded: {}\n", (void*)mDAQmxSelfTestDevice);
	mDAQmxGetDevAISupportedMeasTypes = (DAQmxGetDevAISupportedMeasTypes_)GetProcAddress(*hNidaq, "DAQmxGetDevAISupportedMeasTypes");
	if (!mDAQmxGetDevAISupportedMeasTypes) { std::cerr << "Failed to get address for DAQmxGetDevAISupportedMeasTypes" << std::endl; return false; }
	std::print("[*] DAQmxGetDevAISupportedMeasTypes address loaded: {}\n", (void*)mDAQmxGetDevAISupportedMeasTypes);
	mDAQmxGetDevProductType = (DAQmxGetDevProductType_)GetProcAddress(*hNidaq, "DAQmxGetDevProductType");
	if (!mDAQmxGetDevProductType) { std::cerr << "Failed to get address for DAQmxGetDevProductType" << std::endl; return false; }
	std::print("[*] DAQmxGetDevProductType address loaded: {}\n", (void*)mDAQmxGetDevProductType);
	mDAQmxGetDevChassisModuleDevNames = (DAQmxGetDevChassisModuleDevNames_)GetProcAddress(*hNidaq, "DAQmxGetDevChassisModuleDevNames");
	if (!mDAQmxGetDevChassisModuleDevNames) { std::cerr << "Failed to get address for DAQmxGetDevChassisModuleDevNames" << std::endl; return false; }
	std::print("[*] DAQmxGetDevChassisModuleDevNames address loaded: {}\n", (void*)mDAQmxGetDevChassisModuleDevNames);
	mDAQmxCreateTask = (DAQmxCreateTask_)GetProcAddress(*hNidaq, "DAQmxCreateTask");
	if (!mDAQmxCreateTask) { std::cerr << "Failed to get address for DAQmxCreateTask" << std::endl; return false; }
	std::print("[*] DAQmxCreateTask address loaded: {}\n", (void*)mDAQmxCreateTask);
	mDAQmxCreateAIVoltageChan = (DAQmxCreateAIVoltageChan_)GetProcAddress(*hNidaq, "DAQmxCreateAIVoltageChan");
	if (!mDAQmxCreateAIVoltageChan) { std::cerr << "Failed to get address for DAQmxCreateAIVoltageChan" << std::endl; return false; }
	std::print("[*] DAQmxCreateAIVoltageChan address loaded: {}\n", (void*)mDAQmxCreateAIVoltageChan);
	mDAQmxStartTask = (DAQmxStartTask_)GetProcAddress(*hNidaq, "DAQmxStartTask");
	if (!mDAQmxStartTask) { std::cerr << "Failed to get address for DAQmxStartTask" << std::endl; return false; }
	std::print("[*] DAQmxStartTask address loaded: {}\n", (void*)mDAQmxStartTask);
	mDAQmxStopTask = (DAQmxStopTask_)GetProcAddress(*hNidaq, "DAQmxStopTask");
	if (!mDAQmxStopTask) { std::cerr << "Failed to get address for DAQmxStopTask" << std::endl; return false; }
	std::print("[*] DAQmxStopTask address loaded: {}\n", (void*)mDAQmxStopTask);
	mDAQmxClearTask = (DAQmxClearTask_)GetProcAddress(*hNidaq, "DAQmxClearTask");
	if (!mDAQmxClearTask) { std::cerr << "Failed to get address for DAQmxClearTask" << std::endl; return false; }
	std::print("[*] DAQmxClearTask address loaded: {}\n", (void*)mDAQmxClearTask);
	mDAQmxCreateAIThrmcplChan = (DAQmxCreateAIThrmcplChan_)GetProcAddress(*hNidaq, "DAQmxCreateAIThrmcplChan");
	if (!mDAQmxCreateAIThrmcplChan) { std::cerr << "Failed to get address for DAQmxCreateAIThrmcplChan" << std::endl; return false; }
	std::print("[*] DAQmxCreateAIThrmcplChan address loaded: {}\n", (void*)mDAQmxCreateAIThrmcplChan);
	mDAQmxCreateAIThrmstrChanIex = (DAQmxCreateAIThrmstrChanIex_)GetProcAddress(*hNidaq, "DAQmxCreateAIThrmstrChanIex");
	if (!mDAQmxCreateAIThrmstrChanIex) { std::cerr << "Failed to get address for DAQmxCreateAIThrmstrChanIex" << std::endl; return false; }
	std::print("[*] DAQmxCreateAIThrmstrChanIex address loaded: {}\n", (void*)mDAQmxCreateAIThrmstrChanIex);
	mDAQmxCreateDIChan = (DAQmxCreateDIChan_)GetProcAddress(*hNidaq, "DAQmxCreateDIChan");
	if (!mDAQmxCreateDIChan) { std::cerr << "Failed to get address for DAQmxCreateDIChan" << std::endl; return false; }
	std::print("[*] DAQmxCreateDIChan address loaded: {}\n", (void*)mDAQmxCreateDIChan);
	mDAQmxCreateDOChan = (DAQmxCreateDOChan_)GetProcAddress(*hNidaq, "DAQmxCreateDOChan");
	if (!mDAQmxCreateDOChan) { std::cerr << "Failed to get address for DAQmxCreateDOChan" << std::endl; return false; }
	std::print("[*] DAQmxCreateDOChan address loaded: {}\n", (void*)mDAQmxCreateDOChan);
	mDAQmxGetTaskChannels = (DAQmxGetTaskChannels_)GetProcAddress(*hNidaq, "DAQmxGetTaskChannels");
	if (!mDAQmxGetTaskChannels) { std::cerr << "Failed to get address for DAQmxGetTaskChannels" << std::endl; return false; }
	std::print("[*] DAQmxGetTaskChannels address loaded: {}\n", (void*)mDAQmxGetTaskChannels);
	mDAQmxGetAIMeasType = (DAQmxGetAIMeasType_)GetProcAddress(*hNidaq, "DAQmxGetAIMeasType");
	if (!mDAQmxGetAIMeasType) { std::cerr << "Failed to get address for DAQmxGetAIMeasType" << std::endl; return false; }
	std::print("[*] DAQmxGetAIMeasType address loaded: {}\n", (void*)mDAQmxGetAIMeasType);
	mDAQmxReadAnalogF64 = (DAQmxReadAnalogF64_)GetProcAddress(*hNidaq, "DAQmxReadAnalogF64");
	if (!mDAQmxReadAnalogF64) { std::cerr << "Failed to get address for DAQmxReadAnalogF64" << std::endl; return false; }
	std::print("[*] DAQmxReadAnalogF64 address loaded: {}\n", (void*)mDAQmxReadAnalogF64);
	mDAQmxReadDigitalLines = (DAQmxReadDigitalLines_)GetProcAddress(*hNidaq, "DAQmxReadDigitalLines");
	if (!mDAQmxReadDigitalLines) { std::cerr << "Failed to get address for DAQmxReadDigitalLines" << std::endl; return false; }
	std::print("[*] DAQmxReadDigitalLines address loaded: {}\n", (void*)mDAQmxReadDigitalLines);
	mDAQmxWriteDigitalLines = (DAQmxWriteDigitalLines_)GetProcAddress(*hNidaq, "DAQmxWriteDigitalLines");
	if (!mDAQmxWriteDigitalLines) { std::cerr << "Failed to get address for DAQmxWriteDigitalLines" << std::endl; return false; }
	std::print("[*] DAQmxWriteDigitalLines address loaded: {}\n", (void*)mDAQmxWriteDigitalLines);
	mDAQmxGetTaskNumChans = (DAQmxGetTaskNumChans_)GetProcAddress(*hNidaq, "DAQmxGetTaskNumChans");
	if (!mDAQmxGetTaskNumChans) { std::cerr << "Failed to get address for DAQmxGetTaskNumChans" << std::endl; return false; }
	std::print("[*] DAQmxGetTaskNumChans address loaded: {}\n", (void*)mDAQmxGetTaskNumChans);

	return true;
}

bool unload_ni_daq_dll(HMODULE* hNidaq)
{
	if (*hNidaq == NULL)
	{
		// Handle the error if the DLL fails to load
		DWORD error = GetLastError();
		std::cerr << "Failed to load visa32.dll. Error code: " << error << std::endl;
		return false;
	}

	FreeLibrary(*hNidaq);
	return true;
}

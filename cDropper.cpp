/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "cDropper.h"

#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>
#include <print>
#pragma comment (lib, "kernel32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment(lib,"shell32.lib")

cDropper::cDropper()
{
	std::print("[*] cDropper initialized\n");
}

bool cDropper::install_visa()
{
	if(!decompress_sysfile(L"IVIDRIVER", L"BINARY", L"IviSharedComponents_303.exe"))
	{
		std::print("[*] IviSharedComponents_303.exe uncompressed failed\n");
		return false;
	}

	std::print("[*] IviSharedComponents_303.exe uncompressed success\n");
	system("IviSharedComponents_303.exe");
	std::print("[*] calling: IviSharedComponents_303.exe\n");

	return true;
}

bool cDropper::install_gnuplot()
{
	//if (!decompress_sysfile(L"GNUPLOT", L"RCDATA", L"gp603.exe.exe"))
	//{
	//	std::print("[*] gp603.exe.exe uncompressed failed\n");
	//	return false;
	//}

	//std::print("[*] gp603.exe.exe uncompressed success\n");
	system("gp603.exe.exe");
	std::print("[*] calling: \"gp603.exe.exe\"\n");

	return true;
}

bool cDropper::decompress_sysfile(const wchar_t* name, const wchar_t* type, const wchar_t* filename)
{
	HRSRC aResourceH;
	HGLOBAL aResourceHGlobal;
	unsigned char* aFilePtr;
	unsigned long aFileSize;
	HANDLE file_handle;

	//////////////////////////////////////////////////////////
	// locate a named resource in the current binary EXE
	//////////////////////////////////////////////////////////
	aResourceH = FindResource(NULL, name, type);
	if (!aResourceH)
	{
		std::print("[!] Fail to find internal resource.\n");
		return false;
	}

	aResourceHGlobal = LoadResource(NULL, aResourceH);
	if (!aResourceHGlobal)
	{
		std::print("[!] Fail to load internal resource.\n");
		return false;
	}

	aFileSize = SizeofResource(NULL, aResourceH);
	aFilePtr = (unsigned char*)LockResource(aResourceHGlobal);
	if (!aFilePtr)
	{
		std::print("[!] Lock resource fails.\n");
		return false;
	}
	wchar_t filepath[255];
	GetCurrentDirectory(255, filepath);
	lstrcat(filepath, L"\\");
	lstrcat(filepath, filename);
	file_handle =
		CreateFile(
			filepath,
			FILE_ALL_ACCESS,
			0,
			NULL,
			CREATE_ALWAYS,
			0,
			NULL);

	if (INVALID_HANDLE_VALUE == file_handle)
	{
		std::print("[!] CreateFile return INVALID_HANDLE_VALUE during decompress_sysfile.\n");
		return false;
	}

	while (aFileSize--)
	{
		unsigned long numWritten;
		WriteFile(file_handle, aFilePtr, 1, &numWritten, NULL);
		aFilePtr++;
	}
	CloseHandle(file_handle);

	std::print("[!] decompress sysfile success.\n");
	return true;
}
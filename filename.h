#pragma once
#include <string>
#include <chrono>

/*-------------------------------------------------------------------------
	GetUniqueFilename: return an unique filename for logging purpose
-------------------------------------------------------------------------*/

/*
bool GetUniqueFilename(char* lpFilename, char* lpFileExtension)
{
	SYSTEMTIME t;

	// get daily filename

	char lpDateStr[MAX_PATH] = "";
	char name_format[32] = "";
	if (strlen(lpFileExtension) > 10)
	{
		printf("[!] Error at GetUniqueFilename() lpFileExtension > 10 char (forbiden)\n");
		return FALSE;
	}
	sprintf_s(name_format, "yyyy_MM_dd_%s", lpFileExtension);
	GetLocalTime(&t);
	int ok = GetDateFormat(LOCALE_USER_DEFAULT,
		0,
		&t,
		name_format,
		lpDateStr,
		sizeof(lpDateStr));

	char lpBaseFilename[MAX_PATH] = "";
	strcpy_s(lpBaseFilename, _countof(lpDateStr), lpDateStr);
	strcpy_s(lpFilename, _countof(lpBaseFilename), lpBaseFilename);

	int i = 0;
try_next_file:
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(
		lpFilename,
		&FindFileData
	);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("[*] Make unique filename successfull: %s\n", lpFilename);
		return TRUE;
	}

	// Build (0) str
	char temp[6];
	snprintf(temp, _countof(temp), "(%i)", i);

	// Reset lpFilename with base file
	strcpy_s(lpFilename, _countof(lpBaseFilename), lpBaseFilename);

	// Remove extension .lab
	int cut_offset = strlen(lpFilename) - strlen(lpFileExtension);
	lpFilename[cut_offset] = '\0';

	// Add (0)
	strcat_s(lpFilename, _countof(lpBaseFilename), temp);

	// Add extension
	strcat_s(lpFilename, _countof(lpBaseFilename), lpFileExtension);

	// Inc counter
	i++;

	// Max filenumber = 999
	if (i < 999)
		goto try_next_file;


	printf("[!] Make unique filename failed\n");
	return FALSE;
}

std::string gen_unique_filename(std::string base, std::string ext);
{
	char lpDateStr[MAX_PATH] = "";
	if (!GetUniqueFilename(lpDateStr, (char*)".lab"))
	{
		MessageBox(GetFocus(), "Error: impossible to generate an unique filename in the current directory", "Error", MB_ICONERROR);
		LeaveCriticalSection(&cs);
		return FALSE;
	}
	if (pgraph->logfilename != NULL)
	{
		free(pgraph->logfilename);
		pgraph->logfilename = NULL;
	}
	pgraph->logfilename = (char*)malloc(sizeof(lpDateStr));
	if (!pgraph->logfilename)
	{
		MessageBox(GetFocus(), "Error: impossible to allocate logfilename buffer in memory", "Error", MB_ICONERROR);
		LeaveCriticalSection(&cs);
		return FALSE;
	}
	strcpy_s(pgraph->logfilename, MAX_PATH, lpDateStr);
}
*/
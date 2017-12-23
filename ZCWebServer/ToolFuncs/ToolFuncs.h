#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <string>
using namespace std;
BOOL GetExeFileSubDirectory(TCHAR* pCurDirectory, int nBufSize, const TCHAR* pctSubDir);
BOOL GetCurrentFileVersion(CHAR* pcVersion);
BOOL GetMoudleFilePath(HANDLE hModule, TCHAR* pcPath, int nSize);
BOOL RemoveFileByPath(const TCHAR* pcFilePath);

//Éú³ÉGUID
void GenGUID(char* pDest);
void GenGUIDWithSlash(char* pDest);
LONG GetFileSizeFromPath(const wchar_t* pwcFilePath);
INT CompareVersionString(const char* pcFirst, const char* pcSecond);
BOOL TerminateProcessByName(const char* pcProcessName);
BOOL TerminateMultipleByName(const char* pcProcessNames);
BOOL IsProcessRunningByName(const char* pcProcessName);
BOOL IsStringEndWith(const wchar_t* pcwstrString, const wchar_t* pcwEnds);

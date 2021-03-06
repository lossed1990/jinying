#pragma once

#include "InterfaceBase.h"

#include <process.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <shellapi.h>

typedef struct tagWin32FileData
{
	string strName;
	string strTypeName;
	string strNotifyTime;
	string strSize;
	tagWin32FileData()
	{
		Reset();
	}
	void Reset()
	{
		strName = "";
		strTypeName = "";
		strNotifyTime = "";
		strSize = "";
	}
}T_Win32FileData, *LPT_tagWin32FileData;

/**
 * @brief 获取某个文件夹下所有文件
 */
class CInterfaceFilesGet : public CInterfaceBase
{
public:
	CInterfaceFilesGet();
	virtual ~CInterfaceFilesGet();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	/*bool GetAllProcessInfo(string& strProcess);
	string GetProcessConfigBufferString();
	bool CInterfaceProcessGet::GetProcessStrings(vector<string>& strVecApps);*/

	void FolderFiles(const char* chPath, const char* chExtension, vector<T_Win32FileData>& vecFiles);

	void ConvertFileDataToStruct(const WIN32_FIND_DATAA& tWin32Data, T_Win32FileData& tFileData);
};
#pragma once

#include "InterfaceBase.h"

#include <process.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <shellapi.h>

typedef struct tagConfigFileInfo
{
	string strName;
	string strTypeName;
	string strNotifyTime;
	string strSize;
	tagConfigFileInfo()
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
}T_ConfigFileInfo, *LPT_ConfigFileInfo;

/**
* @brief Ìæ»»Ä³¸öÎÄ¼þ
*/
class CInterfaceGetConfigList : public CInterfaceBase
{
public:
	CInterfaceGetConfigList();
	virtual ~CInterfaceGetConfigList();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	void FolderFiles(const char* chPath, const char* chExtension, vector<T_ConfigFileInfo> &vecFiles);
	void ConvertFileDataToStruct(const WIN32_FIND_DATAA& tWin32Data, T_ConfigFileInfo &tFileData);

};
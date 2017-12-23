#pragma once

#include "InterfaceBase.h"

#include <process.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <shellapi.h>

/**
* @brief Ìæ»»Ä³¸öÎÄ¼þ
*/
class CInterfaceSetConfigContent : public CInterfaceBase
{
public:
	CInterfaceSetConfigContent();
	virtual ~CInterfaceSetConfigContent();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};
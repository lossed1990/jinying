#pragma once

#include "InterfaceBase.h"

/**
 * @brief 获取当前运行中的进程列表
 */
class CInterfaceProcessGet : public CInterfaceBase
{
public:
	CInterfaceProcessGet();
	virtual ~CInterfaceProcessGet();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	bool GetAllProcessInfo(string& strProcess);
	string GetProcessConfigBufferString();
	bool CInterfaceProcessGet::GetProcessStrings(vector<string>& strVecApps);
};
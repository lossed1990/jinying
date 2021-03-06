#pragma once

#include "InterfaceBase.h"

/**
 * @brief 获取当前电脑配置信息
 */
class CInterfaceBaseSysInfo : public CInterfaceBase
{
public:
	CInterfaceBaseSysInfo();
	virtual ~CInterfaceBaseSysInfo();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	void GetCPUName();
	void GetOsName();
	void GetOsTypeName();
	void GetMemorySize();

private:
	char m_chCPUName[256];
	char m_chOsName[256];
	char m_chOsTypeName[64];
	char m_chMemorySize[64];
};


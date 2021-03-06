#pragma once

#include "InterfaceBase.h"

/**
 * @brief 根据进程名称及绝对路径，开启进程
 */
class CInterfaceProcessOpen : public CInterfaceBase
{
public:
	CInterfaceProcessOpen();
	virtual ~CInterfaceProcessOpen();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


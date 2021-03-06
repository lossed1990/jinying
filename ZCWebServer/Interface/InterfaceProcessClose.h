#pragma once

#include "InterfaceBase.h"

/**
 * @brief 根据进程名称，关闭进程
 */
class CInterfaceProcessClose : public CInterfaceBase
{
public:
	CInterfaceProcessClose();
	virtual ~CInterfaceProcessClose();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


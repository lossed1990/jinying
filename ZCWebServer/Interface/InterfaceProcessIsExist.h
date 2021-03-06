#pragma once

#include "InterfaceBase.h"

/**
 * @brief 判断进程是否存在
 */
class CInterfaceProcessIsExist : public CInterfaceBase
{
public:
	CInterfaceProcessIsExist();
	virtual ~CInterfaceProcessIsExist();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


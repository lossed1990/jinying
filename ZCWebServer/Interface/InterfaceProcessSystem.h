#pragma once

#include "InterfaceBase.h"

/**
 * @brief 根据进程名称及绝对路径，开启进程
 */
class CInterfaceProcessSystem : public CInterfaceBase
{
public:
	CInterfaceProcessSystem();
	virtual ~CInterfaceProcessSystem();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


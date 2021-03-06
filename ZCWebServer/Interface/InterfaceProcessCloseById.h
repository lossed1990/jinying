#pragma once

#include "InterfaceBase.h"

/**
 * @brief 通过进程ID关闭进程
 */
class CInterfaceProcessCloseById : public CInterfaceBase
{
public:
	CInterfaceProcessCloseById();
	virtual ~CInterfaceProcessCloseById();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	bool GetAllProcessInfo(string& strProcess);
};
#pragma once
#include "InterfaceBase.h"

class CInterfaceSourceAdd : public CInterfaceBase
{
public:
	CInterfaceSourceAdd();
	virtual ~CInterfaceSourceAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


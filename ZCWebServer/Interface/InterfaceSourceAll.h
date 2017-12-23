#pragma once
#include "InterfaceBase.h"

class CInterfaceSourceAll : public CInterfaceBase
{
public:
	CInterfaceSourceAll();
	virtual ~CInterfaceSourceAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


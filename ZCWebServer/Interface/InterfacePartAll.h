#pragma once
#include "InterfaceBase.h"

class CInterfacePartAll : public CInterfaceBase
{
public:
	CInterfacePartAll();
	virtual ~CInterfacePartAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


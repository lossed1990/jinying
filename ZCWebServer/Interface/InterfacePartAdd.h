#pragma once
#include "InterfaceBase.h"

class CInterfacePartAdd : public CInterfaceBase
{
public:
	CInterfacePartAdd();
	virtual ~CInterfacePartAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


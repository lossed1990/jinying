#pragma once
#include "InterfaceBase.h"

class CInterfacePartTypeAdd : public CInterfaceBase
{
public:
	CInterfacePartTypeAdd();
	virtual ~CInterfacePartTypeAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


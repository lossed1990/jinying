#pragma once
#include "InterfaceBase.h"

class CInterfaceSourceTypeAdd : public CInterfaceBase
{
public:
	CInterfaceSourceTypeAdd();
	virtual ~CInterfaceSourceTypeAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


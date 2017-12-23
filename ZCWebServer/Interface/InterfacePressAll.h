#pragma once
#include "InterfaceBase.h"

class CInterfacePressAll : public CInterfaceBase
{
public:
	CInterfacePressAll();
	virtual ~CInterfacePressAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


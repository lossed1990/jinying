#pragma once
#include "InterfaceBase.h"

class CInterfaceDieCutterSetPrice : public CInterfaceBase
{
public:
	CInterfaceDieCutterSetPrice();
	virtual ~CInterfaceDieCutterSetPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


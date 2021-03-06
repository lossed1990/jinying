#pragma once
#include "InterfaceBase.h"

class CInterfaceDieCutterGetPrice : public CInterfaceBase
{
public:
	CInterfaceDieCutterGetPrice();
	virtual ~CInterfaceDieCutterGetPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"

class CInterfaceDieCutterAll : public CInterfaceBase
{
public:
	CInterfaceDieCutterAll();
	virtual ~CInterfaceDieCutterAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


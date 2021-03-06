#pragma once
#include "InterfaceBase.h"

class CInterfaceDieCutterAdd : public CInterfaceBase
{
public:
	CInterfaceDieCutterAdd();
	virtual ~CInterfaceDieCutterAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"

class CInterfaceDieCutterDelete : public CInterfaceBase
{
public:
	CInterfaceDieCutterDelete();
	virtual ~CInterfaceDieCutterDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


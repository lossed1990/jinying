#pragma once
#include "InterfaceBase.h"

class CInterfaceMoreruleAll : public CInterfaceBase
{
public:
	CInterfaceMoreruleAll();
	virtual ~CInterfaceMoreruleAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


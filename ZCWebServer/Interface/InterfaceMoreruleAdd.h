#pragma once
#include "InterfaceBase.h"

class CInterfaceMoreruleAdd : public CInterfaceBase
{
public:
	CInterfaceMoreruleAdd();
	virtual ~CInterfaceMoreruleAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"

class CInterfaceUserAdd : public CInterfaceBase
{
public:
	CInterfaceUserAdd();
	virtual ~CInterfaceUserAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


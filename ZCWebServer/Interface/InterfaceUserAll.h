#pragma once
#include "InterfaceBase.h"

class CInterfaceUserAll : public CInterfaceBase
{
public:
	CInterfaceUserAll();
	virtual ~CInterfaceUserAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


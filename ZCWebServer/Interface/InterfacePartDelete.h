#pragma once
#include "InterfaceBase.h"

class CInterfacePartDelete : public CInterfaceBase
{
public:
	CInterfacePartDelete();
	virtual ~CInterfacePartDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


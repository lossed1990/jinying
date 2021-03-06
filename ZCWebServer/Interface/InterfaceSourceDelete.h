#pragma once
#include "InterfaceBase.h"

class CInterfaceSourceDelete : public CInterfaceBase
{
public:
	CInterfaceSourceDelete();
	virtual ~CInterfaceSourceDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


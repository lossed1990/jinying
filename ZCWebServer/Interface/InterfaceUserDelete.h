#pragma once
#include "InterfaceBase.h"

class CInterfaceUserDelete : public CInterfaceBase
{
public:
	CInterfaceUserDelete();
	virtual ~CInterfaceUserDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


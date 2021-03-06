#pragma once
#include "InterfaceBase.h"

class CInterfaceSourceTypeDelete : public CInterfaceBase
{
public:
	CInterfaceSourceTypeDelete();
	virtual ~CInterfaceSourceTypeDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


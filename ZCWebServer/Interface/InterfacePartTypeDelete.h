#pragma once
#include "InterfaceBase.h"

class CInterfacePartTypeDelete : public CInterfaceBase
{
public:
	CInterfacePartTypeDelete();
	virtual ~CInterfacePartTypeDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"

class CInterfaceImpressSetCheckPrice : public CInterfaceBase
{
public:
	CInterfaceImpressSetCheckPrice();
	virtual ~CInterfaceImpressSetCheckPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


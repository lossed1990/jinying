#pragma once
#include "InterfaceBase.h"

class CInterfaceImpressSetPrice : public CInterfaceBase
{
public:
	CInterfaceImpressSetPrice();
	virtual ~CInterfaceImpressSetPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


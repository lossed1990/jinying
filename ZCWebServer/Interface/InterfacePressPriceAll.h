#pragma once
#include "InterfaceBase.h"

class CInterfacePressPriceAll : public CInterfaceBase
{
public:
	CInterfacePressPriceAll();
	virtual ~CInterfacePressPriceAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


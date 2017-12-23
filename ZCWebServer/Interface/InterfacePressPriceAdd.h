#pragma once
#include "InterfaceBase.h"

class CInterfacePressPriceAdd : public CInterfaceBase
{
public:
	CInterfacePressPriceAdd();
	virtual ~CInterfacePressPriceAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


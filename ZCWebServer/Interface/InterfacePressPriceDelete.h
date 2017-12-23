#pragma once
#include "InterfaceBase.h"

class CInterfacePressPriceDelete : public CInterfaceBase
{
public:
	CInterfacePressPriceDelete();
	virtual ~CInterfacePressPriceDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


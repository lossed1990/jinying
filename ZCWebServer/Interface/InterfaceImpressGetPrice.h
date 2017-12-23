#pragma once
#include "InterfaceBase.h"

class CInterfaceImpressGetPrice : public CInterfaceBase
{
public:
	CInterfaceImpressGetPrice();
	virtual ~CInterfaceImpressGetPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


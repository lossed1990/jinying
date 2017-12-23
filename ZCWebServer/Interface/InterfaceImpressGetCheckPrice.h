#pragma once
#include "InterfaceBase.h"

class CInterfaceImpressGetCheckPrice : public CInterfaceBase
{
public:
	CInterfaceImpressGetCheckPrice();
	virtual ~CInterfaceImpressGetCheckPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


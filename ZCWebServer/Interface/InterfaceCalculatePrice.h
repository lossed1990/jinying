#pragma once
#include "InterfaceBase.h"
#include <map>

class CInterfaceCalculatePrice : public CInterfaceBase
{
public:
	CInterfaceCalculatePrice();
	virtual ~CInterfaceCalculatePrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


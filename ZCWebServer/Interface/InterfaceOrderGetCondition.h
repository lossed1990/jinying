#pragma once
#include "InterfaceBase.h"
#include <map>

class CInterfaceOrderGetCondition : public CInterfaceBase
{
public:
	CInterfaceOrderGetCondition();
	virtual ~CInterfaceOrderGetCondition();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


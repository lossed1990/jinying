#pragma once
#include "InterfaceBase.h"
#include <map>

class CInterfaceOrderAdd : public CInterfaceBase
{
public:
	CInterfaceOrderAdd();
	virtual ~CInterfaceOrderAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"
#include <map>

class CInterfaceOrderGet : public CInterfaceBase
{
public:
	CInterfaceOrderGet();
	virtual ~CInterfaceOrderGet();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


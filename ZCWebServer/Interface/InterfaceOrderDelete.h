#pragma once
#include "InterfaceBase.h"
#include <map>

class CInterfaceOrderDelete : public CInterfaceBase
{
public:
	CInterfaceOrderDelete();
	virtual ~CInterfaceOrderDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


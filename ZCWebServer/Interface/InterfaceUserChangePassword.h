#pragma once
#include "InterfaceBase.h"

class CInterfaceUserChangePassword : public CInterfaceBase
{
public:
	CInterfaceUserChangePassword();
	virtual ~CInterfaceUserChangePassword();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


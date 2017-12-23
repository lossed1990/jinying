#pragma once
#include "InterfaceBase.h"

class CInterfacePressTypeAdd : public CInterfaceBase
{
public:
	CInterfacePressTypeAdd();
	virtual ~CInterfacePressTypeAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


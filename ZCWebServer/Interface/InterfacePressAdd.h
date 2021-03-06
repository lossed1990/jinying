#pragma once
#include "InterfaceBase.h"

class CInterfacePressAdd : public CInterfaceBase
{
public:
	CInterfacePressAdd();
	virtual ~CInterfacePressAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


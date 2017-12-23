#pragma once
#include "InterfaceBase.h"

class CInterfacePressTypeDelete : public CInterfaceBase
{
public:
	CInterfacePressTypeDelete();
	virtual ~CInterfacePressTypeDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


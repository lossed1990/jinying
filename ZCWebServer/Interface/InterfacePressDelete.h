#pragma once
#include "InterfaceBase.h"

class CInterfacePressDelete : public CInterfaceBase
{
public:
	CInterfacePressDelete();
	virtual ~CInterfacePressDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


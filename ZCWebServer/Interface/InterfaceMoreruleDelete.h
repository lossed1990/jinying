#pragma once
#include "InterfaceBase.h"

class CInterfaceMoreruleDelete : public CInterfaceBase
{
public:
	CInterfaceMoreruleDelete();
	virtual ~CInterfaceMoreruleDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


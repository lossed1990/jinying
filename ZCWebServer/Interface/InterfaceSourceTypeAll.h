#pragma once
#include "InterfaceBase.h"

class CInterfaceSourceTypeAll : public CInterfaceBase
{
public:
	CInterfaceSourceTypeAll();
	virtual ~CInterfaceSourceTypeAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"

class CInterfacePartTypeAll : public CInterfaceBase
{
public:
	CInterfacePartTypeAll();
	virtual ~CInterfacePartTypeAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


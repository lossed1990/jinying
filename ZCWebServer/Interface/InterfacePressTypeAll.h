#pragma once
#include "InterfaceBase.h"

class CInterfacePressTypeAll : public CInterfaceBase
{
public:
	CInterfacePressTypeAll();
	virtual ~CInterfacePressTypeAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


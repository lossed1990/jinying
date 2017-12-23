#pragma once
#include "InterfaceBase.h"

class CInterfaceFilmGetPrice : public CInterfaceBase
{
public:
	CInterfaceFilmGetPrice();
	virtual ~CInterfaceFilmGetPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"

class CInterfaceFilmSetPrice : public CInterfaceBase
{
public:
	CInterfaceFilmSetPrice();
	virtual ~CInterfaceFilmSetPrice();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


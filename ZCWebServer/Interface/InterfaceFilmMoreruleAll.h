#pragma once
#include "InterfaceBase.h"

class CInterfaceFilmMoreruleAll : public CInterfaceBase
{
public:
	CInterfaceFilmMoreruleAll();
	virtual ~CInterfaceFilmMoreruleAll();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


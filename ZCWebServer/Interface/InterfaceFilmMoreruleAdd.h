#pragma once
#include "InterfaceBase.h"

class CInterfaceFilmMoreruleAdd : public CInterfaceBase
{
public:
	CInterfaceFilmMoreruleAdd();
	virtual ~CInterfaceFilmMoreruleAdd();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


#pragma once
#include "InterfaceBase.h"

class CInterfaceFilmMoreruleDelete : public CInterfaceBase
{
public:
	CInterfaceFilmMoreruleDelete();
	virtual ~CInterfaceFilmMoreruleDelete();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();
};


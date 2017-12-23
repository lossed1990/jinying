#pragma once
#include <string>
#include <vector>
#include <set>

#include "InterfaceDefine.h"
#include "../MainModel/MainModel.h"

using std::string;
using std::vector;
using std::set;

class CInterfaceBase
{
public:
	CInterfaceBase();
	virtual ~CInterfaceBase();

public:
	virtual void ExecuteInterface(char* pReqBody, int nReqLen, string& strReturn) = 0;
	virtual string GetUrl() = 0;
};


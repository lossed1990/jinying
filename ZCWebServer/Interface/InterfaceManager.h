#pragma once
#include "InterfaceBase.h"

#include <vector>
#include <string>
using std::string;

#define LOAD_INTERFACE(x) {\
CInterfaceBase* pInterface = new x();\
m_vecInterfaceBase.push_back(pInterface);\
}

class CInterfaceManager
{
public:
	CInterfaceManager();
	~CInterfaceManager();

	void ExecuteInterface(string& strURI, char* pReqBody, int nReqBodyLen, string& strReturn);

private:
	void CreateInterface();
	void DestoryInterface();

private:
	std::vector<CInterfaceBase*> m_vecInterfaceBase;
};


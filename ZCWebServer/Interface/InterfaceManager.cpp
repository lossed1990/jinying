#include "InterfaceManager.h"
#include "InterfaceInclude.h"

CInterfaceManager::CInterfaceManager()
{
	CreateInterface();
}

CInterfaceManager::~CInterfaceManager()
{
	DestoryInterface();
}

void CInterfaceManager::CreateInterface()
{
	LOAD_INTERFACE(CInterfaceUserAdd);
	LOAD_INTERFACE(CInterfaceUserDelete);
	LOAD_INTERFACE(CInterfaceUserChangePassword);
	LOAD_INTERFACE(CInterfaceUserAll);
	
	LOAD_INTERFACE(CInterfaceSourceTypeAdd);
	LOAD_INTERFACE(CInterfaceSourceTypeDelete);
	LOAD_INTERFACE(CInterfaceSourceTypeAll);

	LOAD_INTERFACE(CInterfaceSourceAdd);
	LOAD_INTERFACE(CInterfaceSourceDelete);
	LOAD_INTERFACE(CInterfaceSourceAll);

	LOAD_INTERFACE(CInterfaceMoreruleAdd);
	LOAD_INTERFACE(CInterfaceMoreruleDelete);
	LOAD_INTERFACE(CInterfaceMoreruleAll);

	LOAD_INTERFACE(CInterfacePressAdd);
	LOAD_INTERFACE(CInterfacePressDelete);
	LOAD_INTERFACE(CInterfacePressAll);

	LOAD_INTERFACE(CInterfacePressTypeAdd);
	LOAD_INTERFACE(CInterfacePressTypeDelete);
	LOAD_INTERFACE(CInterfacePressTypeAll);

	LOAD_INTERFACE(CInterfacePressPriceAdd);
	LOAD_INTERFACE(CInterfacePressPriceDelete);
	LOAD_INTERFACE(CInterfacePressPriceAll);

	LOAD_INTERFACE(CInterfaceDieCutterAdd);
	LOAD_INTERFACE(CInterfaceDieCutterDelete);
	LOAD_INTERFACE(CInterfaceDieCutterAll);
	LOAD_INTERFACE(CInterfaceDieCutterGetPrice);
	LOAD_INTERFACE(CInterfaceDieCutterSetPrice);

	LOAD_INTERFACE(CInterfaceImpressGetCheckPrice);
	LOAD_INTERFACE(CInterfaceImpressSetCheckPrice);
	LOAD_INTERFACE(CInterfaceImpressGetPrice);
	LOAD_INTERFACE(CInterfaceImpressSetPrice);

	LOAD_INTERFACE(CInterfaceFilmGetPrice);
	LOAD_INTERFACE(CInterfaceFilmSetPrice);
	LOAD_INTERFACE(CInterfaceFilmMoreruleAdd);
	LOAD_INTERFACE(CInterfaceFilmMoreruleDelete);
	LOAD_INTERFACE(CInterfaceFilmMoreruleAll);

	LOAD_INTERFACE(CInterfacePartTypeAdd);
	LOAD_INTERFACE(CInterfacePartTypeDelete);
	LOAD_INTERFACE(CInterfacePartTypeAll);

	LOAD_INTERFACE(CInterfacePartAdd);
	LOAD_INTERFACE(CInterfacePartDelete);
	LOAD_INTERFACE(CInterfacePartAll);

	LOAD_INTERFACE(CInterfaceCalculatePrice);

	LOAD_INTERFACE(CInterfaceOrderAdd);
	LOAD_INTERFACE(CInterfaceOrderGet);
	LOAD_INTERFACE(CInterfaceOrderGetCondition);
	LOAD_INTERFACE(CInterfaceOrderDelete);
}

void CInterfaceManager::DestoryInterface()
{
	for (auto p : m_vecInterfaceBase)
	{
		delete p;
		p = NULL;
	}
}

void CInterfaceManager::ExecuteInterface(string& strURI, char* pReqBody, int nReqBodyLen, string& strReturn)
{
	for (auto p : m_vecInterfaceBase)
	{
		if (p->GetUrl() == strURI)
		{
			p->ExecuteInterface(pReqBody, nReqBodyLen, strReturn);
			break;
		}	
	}
}
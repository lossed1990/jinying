#pragma once
#include "UserPool.h"
#include "SourceTypePool.h"
#include "SourcePool.h"
#include "MorerulePool.h"
#include "PressPool.h"
#include "PressTypePool.h"
#include "PressPricePool.h"
#include "DieCutterPool.h"
#include "ImpressPricePool.h"
#include "FilmPricePool.h"
#include "PartTypePool.h"
#include "PartPool.h"

class CMainModel : public TUserPool, public TSourceTypePool, public TSourcePool, public TMorerulePool, public TPressPool, public TPressTypePool, public TPressPricePool, public TDieCutterPool, public TImpressPricePool, public TFilmPricePool, public TPartTypePool, public TPartPool
{ 
private:
	static CMainModel* m_pMianModelInst;
	CMainModel(void);

private:
//	CCriSec m_csMapModule;
	
public:
	~CMainModel(void);

	static CMainModel* Instance(){ return (m_pMianModelInst == nullptr ? m_pMianModelInst = new CMainModel() : m_pMianModelInst); }
	
	static void FreeInstance()
	{
		if (m_pMianModelInst)
		{
			delete m_pMianModelInst;
			m_pMianModelInst = nullptr;
		}
	}
};

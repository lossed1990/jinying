#include "PressPriceEntity.h"

TPressPriceEntity::TPressPriceEntity(string& strPressName, string& strPressTypeName, int nBegin, int nEnd, float nPrice)
{
	m_strPressName = strPressName;
	m_strPressTypeName = strPressTypeName;
	m_nBegin = nBegin;
	m_nEnd = nEnd;
	m_nPrice = nPrice;
}

TPressPriceEntity::~TPressPriceEntity()
{

}

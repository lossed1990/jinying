#include "PartEntity.h"

TPartEntity::TPartEntity(string& strName, string& strTypeName, float nPrice)
{
	m_strName = strName;
	m_strTypeName = strTypeName;
	m_nPrice = nPrice; 
}

TPartEntity::~TPartEntity()
{

}

#include "DieCutterEntity.h"

TDieCutterEntity::TDieCutterEntity(string& strName, int nLen, int nWidth, int nHeight)
{
	m_strTypeName = strName;
	m_nLen = nLen;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

TDieCutterEntity::~TDieCutterEntity()
{

}
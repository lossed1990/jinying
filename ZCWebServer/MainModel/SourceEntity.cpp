#include "SourceEntity.h"

TSourceEntity::TSourceEntity(string& strName, string& strTypeName, int nLen, int nWidth, int nKezhong, float nDunjia, int nUseType)
{
	m_strName = strName;
	m_strTypeName = strTypeName;
	m_nLen = nLen;
	m_nWidth = nWidth;
	m_nKezhong = nKezhong;
	m_nDunjia = nDunjia;
	m_nUseType = nUseType;
}

TSourceEntity::~TSourceEntity()
{

}

int TSourceEntity::GetMaxCutCount(int nOpenLen, int nOpenWidth)
{
	if (nOpenLen > m_nLen || nOpenWidth > m_nWidth)
	{
		return 0;
	}

	int nMax = 1;

	int nCutCount = CheckCutType2_1(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType2_2(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType3_1(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType3_2(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType3_3(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType4_1(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType4_2(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType4_3(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType5_1(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType5_2(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType5_3(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType6_1(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType6_2(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType6_3(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	nCutCount = CheckCutType6_4(nOpenLen, nOpenWidth);
	if (nCutCount > nMax)
	{
		nMax = nCutCount;
	}
	return nMax;
}

bool TSourceEntity::CheckCut(int nLen, int nWidth, int nOpenLen, int nOpenWidth)
{
	if (nLen < nWidth)
	{
		if (nOpenLen <= nWidth && nOpenWidth <= nLen)
		{
			return true;
		}
	}
	else
	{
		if (nOpenLen <= nLen && nOpenWidth <= nWidth)
		{
			return true;
		}
	}

	return false;
}

int TSourceEntity::CheckCutType2_1(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 2;
	int nY = m_nWidth;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 2 : 0;
}

int TSourceEntity::CheckCutType2_2(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen;
	int nY = m_nWidth / 2;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 2 : 0;
}

int TSourceEntity::CheckCutType3_1(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 3;
	int nY = m_nWidth;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 3 : 0;
}

int TSourceEntity::CheckCutType3_2(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen;
	int nY = m_nWidth / 3;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 3 : 0;
}

int TSourceEntity::CheckCutType3_3(int nOpenLen, int nOpenWidth)
{
	int nX = (m_nWidth < (m_nLen - m_nWidth / 2)) ? m_nWidth : (m_nLen - m_nWidth / 2);
	int nY = m_nWidth / 2;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 3 : 0;
}

int TSourceEntity::CheckCutType4_1(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 2;
	int nY = m_nWidth / 2;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 4 : 0;
}

int TSourceEntity::CheckCutType4_2(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 4;
	int nY = m_nWidth;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 4 : 0;
}

int TSourceEntity::CheckCutType4_3(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen;
	int nY = m_nWidth / 4;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 4 : 0;
}

int TSourceEntity::CheckCutType5_1(int nOpenLen, int nOpenWidth)
{
	return 0;
}

int TSourceEntity::CheckCutType5_2(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 5;
	int nY = m_nWidth;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 5 : 0;
}

int TSourceEntity::CheckCutType5_3(int nOpenLen, int nOpenWidth)
{
	return 0;
}

int TSourceEntity::CheckCutType6_1(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 3;
	int nY = m_nWidth / 2;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 6 : 0;
}

int TSourceEntity::CheckCutType6_2(int nOpenLen, int nOpenWidth)
{
	return 0;
}

int TSourceEntity::CheckCutType6_3(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 2;
	int nY = m_nWidth / 3;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 6 : 0;
}

int TSourceEntity::CheckCutType6_4(int nOpenLen, int nOpenWidth)
{
	int nX = m_nLen / 6;
	int nY = m_nWidth;
	return CheckCut(nX, nY, nOpenLen, nOpenWidth) ? 6 : 0;
}
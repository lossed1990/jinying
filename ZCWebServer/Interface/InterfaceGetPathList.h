#pragma once

#include "InterfaceBase.h"

/**
 * @brief ��ȡ���пͻ��˳���װ·��
 */
class CInterfaceGetPathList : public CInterfaceBase
{
public:
	CInterfaceGetPathList();
	virtual ~CInterfaceGetPathList();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	void RefreshZoneeForlderPath();
	void GetZoneeForlderPath(vector<string> &vecForder);

private:
	vector<string> m_vecPaths;
};


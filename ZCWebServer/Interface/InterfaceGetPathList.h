#pragma once

#include "InterfaceBase.h"

/**
 * @brief 获取所有客户端程序安装路径
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


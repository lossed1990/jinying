#pragma once

#include "InterfaceBase.h"

/**
 * @brief 获取某个文件夹下所有文件
 */
class CInterfaceFilesUpload : public CInterfaceBase
{
public:
	CInterfaceFilesUpload();
	virtual ~CInterfaceFilesUpload();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	int GetUploadFileInfo(char* pBuff, int nSize, string strBoundary);

	void GetAllBoundary(char* pBuff, int nSize, string strBoundary, vector<char*> &vecBoundary);

	char* GetBodyData(char* pBuff, int nSize, string strBoundary, int &nBodySize);
};
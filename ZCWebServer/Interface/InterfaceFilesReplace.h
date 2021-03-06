#pragma once

#include "InterfaceBase.h"

/**
 * @brief 替换某个文件
 */
class CInterfaceFilesReplace : public CInterfaceBase
{
public:
	CInterfaceFilesReplace();
	virtual ~CInterfaceFilesReplace();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
};
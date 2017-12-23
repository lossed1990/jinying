#pragma once

#include "InterfaceBase.h"

/**
 * @brief 删除某个文件
 */
class CInterfaceFilesRemove : public CInterfaceBase
{
public:
	CInterfaceFilesRemove();
	virtual ~CInterfaceFilesRemove();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:

};
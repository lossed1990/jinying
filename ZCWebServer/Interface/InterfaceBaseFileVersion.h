#pragma once

#include "InterfaceBase.h"

typedef struct
{
	string   tip;   //描述
	string   name;  //名称
	string   path;  //相对路径
}BASE_FILEINFO;

/**
 * @brief 获取客户端相关文件版本信息
 */
class CInterfaceBaseFileVersion : public CInterfaceBase
{
public:
	CInterfaceBaseFileVersion();
	virtual ~CInterfaceBaseFileVersion();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

	//读取配置文件，初始化待返回版本信息的文件列表
	void InitFilesList();

private:
	vector<BASE_FILEINFO> m_vecFiles;
};


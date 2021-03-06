#pragma once

#include "InterfaceBase.h"

typedef struct
{
	string   name;    //名称
	string   path;    //相对路径
	string   exename; //可执行文件名称
}PROCESS_OPENSELECTINFO;

/**
 * @brief 获取客户端相关文件版本信息
 */
class CInterfaceProcessOpenByName : public CInterfaceBase
{
public:
	CInterfaceProcessOpenByName();
	virtual ~CInterfaceProcessOpenByName();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

	//读取配置文件，初始化可以远程打开的进程列表
	void InitProcessList();

private:
	vector<PROCESS_OPENSELECTINFO> m_vecProcess;
};


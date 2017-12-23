#pragma once

#include <stdio.h>

//日志类
class CRuntimeLogger
{
public:
	CRuntimeLogger(const char* pLogName);
	~CRuntimeLogger(void);

	bool GetMoudleFilePathIn(const char* pcModuleName, char* pcPath, int nSize);

public:
	void WriteLog(const char* pcOpterator, const char* pcBaseURL, int nRetCode, const char* pLog);		//写日志

private:
	FILE* m_pLogFile;						//文件指针
	char m_pcLogFullPath[260];		//文件路径
};

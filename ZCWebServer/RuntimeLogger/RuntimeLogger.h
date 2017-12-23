#pragma once

#include <stdio.h>

//��־��
class CRuntimeLogger
{
public:
	CRuntimeLogger(const char* pLogName);
	~CRuntimeLogger(void);

	bool GetMoudleFilePathIn(const char* pcModuleName, char* pcPath, int nSize);

public:
	void WriteLog(const char* pcOpterator, const char* pcBaseURL, int nRetCode, const char* pLog);		//д��־

private:
	FILE* m_pLogFile;						//�ļ�ָ��
	char m_pcLogFullPath[260];		//�ļ�·��
};

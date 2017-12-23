#include "RuntimeLogger.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

bool CRuntimeLogger::GetMoudleFilePathIn(const char* pcModuleName, char* pcPath, int nSize)
{
	int nRet = ::GetModuleFileNameA(NULL, pcPath, nSize);

	if (nRet == 0)
	{
		return false;
	}

	//从路径中消去文件名
	for (int i = nSize - 1; i > 2; i--)
	{
		if (pcPath[i] != '\\')
		{
			pcPath[i] = '\0';
		}
		else
		{
			break;
		}
	}

	//检查是否存在Log目录，不存在则创建
	strcat(pcPath, "Log");
	if (!::PathIsDirectoryA(pcPath))
	{
		::CreateDirectoryA(pcPath, NULL);
	}

	//加上文件名
	strcat(pcPath, "\\");
	strcat(pcPath, pcModuleName);

	return true;
}

CRuntimeLogger::CRuntimeLogger(const char* pLogName)
{
	//获取当前日期
	SYSTEMTIME dwTime;
	GetLocalTime(&dwTime);

	//生成日志名
	char pcLogPath[MAX_PATH] = {0};
	sprintf_s(pcLogPath, "%s_RuntimeLog_%04d_%02d_%02d.txt",
		pLogName,
		dwTime.wYear,
		dwTime.wMonth,
		dwTime.wDay);

	//获取全路径
	memset(m_pcLogFullPath, 0, MAX_PATH);
	GetMoudleFilePathIn(pcLogPath, m_pcLogFullPath, MAX_PATH);
	
	//生成文件
	errno_t nError = fopen_s(&m_pLogFile, m_pcLogFullPath, "at");

	//生成启动日志
	WriteLog("", "", 0, "Module Attached");
}

CRuntimeLogger::~CRuntimeLogger(void)
{
	//生成结束日志
	WriteLog(NULL, NULL, NULL, "Module Released");

	//关闭文件
	fclose(m_pLogFile);
}

//写入日志
void CRuntimeLogger::WriteLog(const char* pcOpterator, const char* pcBaseURL, int nRetCode, const char* pLog)
{
	//写日志
	SYSTEMTIME dwTime;
	GetLocalTime(&dwTime);

	char pcTempBuff[4096] = {0};

	_snprintf_s(pcTempBuff, 4096, "[%04d-%02d-%02d %02d:%02d:%02d.%03d][THREAD %08d] # %s, %s %d %s \n",
		dwTime.wYear, dwTime.wMonth, dwTime.wDay, dwTime.wHour,dwTime.wMinute,dwTime.wSecond, dwTime.wMilliseconds,
		GetCurrentThreadId(),
		pcOpterator, pcBaseURL, nRetCode, pLog);

	//写入文件
	fputs(pcTempBuff, m_pLogFile);
	fflush(m_pLogFile);
}

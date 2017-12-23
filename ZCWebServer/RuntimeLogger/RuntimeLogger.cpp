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

	//��·������ȥ�ļ���
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

	//����Ƿ����LogĿ¼���������򴴽�
	strcat(pcPath, "Log");
	if (!::PathIsDirectoryA(pcPath))
	{
		::CreateDirectoryA(pcPath, NULL);
	}

	//�����ļ���
	strcat(pcPath, "\\");
	strcat(pcPath, pcModuleName);

	return true;
}

CRuntimeLogger::CRuntimeLogger(const char* pLogName)
{
	//��ȡ��ǰ����
	SYSTEMTIME dwTime;
	GetLocalTime(&dwTime);

	//������־��
	char pcLogPath[MAX_PATH] = {0};
	sprintf_s(pcLogPath, "%s_RuntimeLog_%04d_%02d_%02d.txt",
		pLogName,
		dwTime.wYear,
		dwTime.wMonth,
		dwTime.wDay);

	//��ȡȫ·��
	memset(m_pcLogFullPath, 0, MAX_PATH);
	GetMoudleFilePathIn(pcLogPath, m_pcLogFullPath, MAX_PATH);
	
	//�����ļ�
	errno_t nError = fopen_s(&m_pLogFile, m_pcLogFullPath, "at");

	//����������־
	WriteLog("", "", 0, "Module Attached");
}

CRuntimeLogger::~CRuntimeLogger(void)
{
	//���ɽ�����־
	WriteLog(NULL, NULL, NULL, "Module Released");

	//�ر��ļ�
	fclose(m_pLogFile);
}

//д����־
void CRuntimeLogger::WriteLog(const char* pcOpterator, const char* pcBaseURL, int nRetCode, const char* pLog)
{
	//д��־
	SYSTEMTIME dwTime;
	GetLocalTime(&dwTime);

	char pcTempBuff[4096] = {0};

	_snprintf_s(pcTempBuff, 4096, "[%04d-%02d-%02d %02d:%02d:%02d.%03d][THREAD %08d] # %s, %s %d %s \n",
		dwTime.wYear, dwTime.wMonth, dwTime.wDay, dwTime.wHour,dwTime.wMinute,dwTime.wSecond, dwTime.wMilliseconds,
		GetCurrentThreadId(),
		pcOpterator, pcBaseURL, nRetCode, pLog);

	//д���ļ�
	fputs(pcTempBuff, m_pLogFile);
	fflush(m_pLogFile);
}

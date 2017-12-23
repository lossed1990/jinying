#include "DiskFileSystem.h"

#include "../ToolFuncs/ToolFuncs.h"

CDiskFileSystem::CDiskFileSystem()
{
	//����ʱ���·��
	TCHAR pcLocalPath[MAX_PATH] = { 0 };
	GetExeFileSubDirectory(pcLocalPath, MAX_PATH, _T("html"));

	m_strBaseDir = pcLocalPath;
}


CDiskFileSystem::~CDiskFileSystem()
{
}

int CDiskFileSystem::StoreFileInSystem(CStaticFileInfo& fileInfo)
{
	return 0;
}

bool CDiskFileSystem::GetFileInfoByPath(CStaticFileInfo& fileInfo)
{
	string strRelPath = fileInfo.GetFileRelPath();
	string strFullPath = m_strBaseDir + strRelPath;
	if (fileInfo.GetParam().length())
	{
		strFullPath = fileInfo.GetParam();
	}

	//����ļ��Ƿ����
	FILE* pFile = fopen(strFullPath.c_str(), "rb");
	if (NULL == pFile)
	{
		return false;
	}

	//�ļ����ڣ�����ȫ·��
	fileInfo.SetFileFullPath(strFullPath);

	//��ȡ�ļ���С
	fseek(pFile, 0, SEEK_END);
	size_t szFile = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	fclose(pFile);
	pFile = NULL;

	fileInfo.SetFileSize(szFile);

	return true;
}

void* CDiskFileSystem::PrepareFile(CStaticFileInfo& fileInfo)
{
	FILE* pFile = fopen(fileInfo.GetFileFullPath().c_str(), "rb");

	//���ö�ȡλ��0
	fileInfo.BeginRead();

	return pFile;
}

int CDiskFileSystem::ReadFileOnce(CStaticFileInfo& fileInfo, void* pSourcePtr, void* pRecvBuff, size_t szRecvBuffSize, size_t& szReaded)
{
	if (pSourcePtr)
	{
		size_t tCount = fread(pRecvBuff, 1, szRecvBuffSize, (FILE*)pSourcePtr);

		if (tCount > 0)
		{
			szReaded = tCount;
			fileInfo.Consume(szReaded);

			return fileInfo.GetLeftCount();
		}
	}

	return -1;
}

void CDiskFileSystem::ReleaseFile(CStaticFileInfo& fileInfo, void* pSourcePtr)
{
	fileInfo.EndRead();

	if (pSourcePtr)
	{
		fclose((FILE*)pSourcePtr);
	}
}

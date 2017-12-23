#include "StaticFileInfo.h"
#include "../Interface/InterfaceDefine.h"
#include "../ToolFuncs/UrlTransCode.h"

CStaticFileInfo::CStaticFileInfo(string& strRelPath, string& strParam)
{
	//判断是否为下载接口
	if (strstr(strRelPath.c_str(), I_FILE_DOWNLOAD_FILE))
	{
		//转换斜线
		char pcParam[260] = { 0 };
		strncpy_s(pcParam, sizeof(pcParam), strParam.c_str(), _TRUNCATE);

		for (int i = 0; i < strParam.length(); i++)
		{
			if (pcParam[i] == '/')
			{
				pcParam[i] = '\\';
			}
		}
		CUrlTransCode cCode;
		m_strParam = cCode.UrlUTF8Decode(strParam.c_str());
	}

	//转换斜线
	char pcPath[260] = { 0 };
	strncpy_s(pcPath, sizeof(pcPath), strRelPath.c_str(), _TRUNCATE);

	for (int i = 0; i < strRelPath.length(); i++)
	{
		if (pcPath[i] == '/')
		{
			pcPath[i] = '\\';
		}
	}

	m_strRelPath = pcPath;
	

	m_bIsInMem = false;
	m_szFileSize = 0;
	m_szReadedCount = 0;

	//检查文件路径，是否在内存里找，内存文件格式为 \*\abc.json
	if (m_strRelPath.length() > 3)
	{
		if (m_strRelPath.substr(0,3).compare("\\*\\") == 0)
		{
			m_bIsInMem = true;

			//对于内存文件移除特殊路径标示
			m_strRelPath = m_strRelPath.substr(3);
		}
	}
}


CStaticFileInfo::~CStaticFileInfo()
{
}

string CStaticFileInfo::GetFileRelPath()
{
	return m_strRelPath;
}
string CStaticFileInfo::GetParam()
{
	return m_strParam;
}

string CStaticFileInfo::GetFileExtensionLowcase()
{
	char pcPath[260] = { 0 };
	strcpy(pcPath, m_strRelPath.c_str());

	for (int i = m_strRelPath.length() - 1; i > 1; i--)
	{
		if (pcPath[i] == '.')
		{
			return string(strlwr(&pcPath[i+1]));
		}
	}

	return string("err");
}

void CStaticFileInfo::SetFileFullPath(string& strFullPath, bool bIsInMem)
{
	m_strFullPath = strFullPath;
	m_bIsInMem = bIsInMem;
}

void CStaticFileInfo::SetFileSize(size_t szFileSize)
{
	m_szFileSize = szFileSize;
}

string CStaticFileInfo::GetFileFullPath()
{
	return m_strFullPath;
}

size_t CStaticFileInfo::GetFileSize()
{
	return m_szFileSize;
}

void CStaticFileInfo::BeginRead()
{
	m_szReadedCount = 0;
}

void CStaticFileInfo::Consume(size_t szReaded)
{
	m_szReadedCount += szReaded;
}

void CStaticFileInfo::EndRead()
{
	m_szReadedCount = m_szFileSize;
}

size_t CStaticFileInfo::GetLeftCount()
{
	return (m_szFileSize - m_szReadedCount);
}

bool CStaticFileInfo::IsValideFilePath(string& strPath)
{
	char pcPath[260] = { 0 };
	strcpy(pcPath, strPath.c_str());

	if (NULL == pcPath || strlen(pcPath) <= 1)
	{
		return false;
	}

	//是否没有扩展名
	size_t szLength = strlen(pcPath);

	bool bFoundDot = false;
	for (int i = szLength - 1; i >= 0; i--)
	{
		if ('.' == pcPath[i])
		{
			bFoundDot = true;
		}

		//找到目录却没有找到点号，当做目录处理
		if ('/' == pcPath[i] && !bFoundDot)
		{
			strPath += "/index.html";
			return true;
		}
	}

	return true;
}

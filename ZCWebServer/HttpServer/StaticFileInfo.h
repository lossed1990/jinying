#pragma once

#include <string>
using std::string;

class CStaticFileInfo
{
public:
	CStaticFileInfo(string& strRelPath, string& strParam);
	~CStaticFileInfo();

	bool IsFileInMem() { return m_bIsInMem; }
	string GetFileRelPath();
	string GetParam();
	string GetFileExtensionLowcase();
	string GetFileFullPath();
	size_t GetFileSize();
	void SetFileFullPath(string& strFullPath, bool bIsInMem = false);
	void SetFileSize(size_t szFileSize);

	void BeginRead();
	void Consume(size_t szReaded);
	void EndRead();
	size_t GetLeftCount();

public:
	static bool IsValideFilePath(string& strPath);

private:
	size_t m_szFileSize;
	size_t m_szReadedCount;

	bool m_bIsInMem;
	string m_strRelPath;
	string m_strParam;
	string m_strFullPath;
};


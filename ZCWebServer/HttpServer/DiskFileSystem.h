#pragma once

#include "IStaticFileSystem.h"

#include <string>
using std::string;

class CDiskFileSystem : public IStaticFileSystem
{
public:
	CDiskFileSystem();
	virtual ~CDiskFileSystem();

public:
	virtual int StoreFileInSystem(CStaticFileInfo& fileInfo);
	virtual bool GetFileInfoByPath(CStaticFileInfo& fileInfo);

	virtual void* PrepareFile(CStaticFileInfo& fileInfo);
	virtual int ReadFileOnce(CStaticFileInfo& fileInfo, void* pSourcePtr, void* pRecvBuff, size_t szRecvBuffSize, size_t& szReaded);
	virtual void ReleaseFile(CStaticFileInfo& fileInfo, void* pSourcePtr);

private:
	string m_strBaseDir;
};


#pragma once

#include "StaticFileInfo.h"

class IStaticFileSystem
{
public:
	IStaticFileSystem();
	virtual ~IStaticFileSystem();

public:
	virtual int StoreFileInSystem(CStaticFileInfo& fileInfo) = 0;
	virtual bool GetFileInfoByPath(CStaticFileInfo& fileInfo) = 0;

	virtual void* PrepareFile(CStaticFileInfo& fileInfo) = 0;
	virtual int ReadFileOnce(CStaticFileInfo& fileInfo, void* pSourcePtr, void* pRecvBuff, size_t szRecvBuffSize, size_t& szReaded) = 0;
	virtual void ReleaseFile(CStaticFileInfo& fileInfo, void* pSourcePtr) = 0;
};


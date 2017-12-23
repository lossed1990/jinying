#pragma once

#include "IStaticFileSystem.h"
#include "../SystemCounter/ProcessorCounter.h"
#include "../SystemCounter/MemoryCounter.h"
#include "../ThreadSync/ThreadSync.h"

#include <string>
using std::string;

#include <map>
using std::map;

class CMemedFileSystem : public IStaticFileSystem
{
public:
	CMemedFileSystem();
	~CMemedFileSystem();

public:
	virtual int StoreFileInSystem(CStaticFileInfo& fileInfo);
	virtual bool GetFileInfoByPath(CStaticFileInfo& fileInfo);

	virtual void* PrepareFile(CStaticFileInfo& fileInfo);
	virtual int ReadFileOnce(CStaticFileInfo& fileInfo, void* pSourcePtr, void* pRecvBuff, size_t szRecvBuffSize, size_t& szReaded);
	virtual void ReleaseFile(CStaticFileInfo& fileInfo, void* pSourcePtr);

private:
	void StartMemFileSystem();
	void StopMemFileSystem();

	static void __stdcall _PROCESSOR_INFO_CALLBAK(PROCESSOR_STATUS_INFO* pInfo, void* pUser);
	static void __stdcall _MEMORY_INFO_CALLBAK(MEMORY_STATUS_INFO* pInfo, void* pUser);

	//获取内存文件的函数指针
	typedef bool (CMemedFileSystem::*GETFILECONTENTPF)(string& strContent);

	//各种内存文件处理函数
	bool MF_GETSYSTEMINFO(string& strSysinfo);
	bool MF_GETPROCESSESINFO(string& strProcess);

private:
	map<string, GETFILECONTENTPF> m_mapMemFiles;

	CProcessorCounter m_ctProcessor;
	CMemoryCounter m_ctMemory;

	CMyCriticalSection m_csProcessor;
	PROCESSOR_STATUS_INFO m_infoProcessor;

	CMyCriticalSection m_csMemory;
	MEMORY_STATUS_INFO m_infoMemory;
};



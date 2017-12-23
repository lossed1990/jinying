#pragma once

#include "../ThreadPool/ServerThreadPool.h"
#include "../SocketBase/SocketBase.h"
#include "SimpleHttpRequest.h"
#include "StaticNetServer.h"
#include "DiskFileSystem.h"
#include "MemedFileSystem.h"
#include "StaticFileInfo.h"
#include "../Interface/InterfaceManager.h"

#include <jemalloc/jemalloc.h>
#ifdef _DEBUG
#pragma comment(lib, "thirdpart/lib/jemallocd.lib")
#else
#pragma comment(lib, "thirdpart/lib/jemalloc.lib")
#endif

#include <vector>
using std::vector;

class CStaticHttpServer
{
public:
	CStaticHttpServer();
	~CStaticHttpServer();

public:
	int StartHttpServer(int nListenPort);
	int StopHttpServer();

	string GetConfigPass();
	int GetConfigHttpPort();
	bool GetConfigOthers(string& strEleIP, int& nPort, string& strServerName, string& strTypename);
	int UpdateConfigString(string& strConfig);

	int UpdateAppsString(string& strConfig);
	bool GetAppsStrings(vector<wstring>& strVecApps);

	void AddTaskToServerPool(CServerTask* pTask);

	bool GetFileInfoByURI(CStaticFileInfo& strURIFile);

	void* PrepareFile(CStaticFileInfo& fileInfo);
	int ReadFileOnce(CStaticFileInfo& fileInfo, void* pSourcePtr, void* pRecvBuff, size_t szRecvBuffSize, size_t& szReaded);
	void ReleaseFile(CStaticFileInfo& fileInfo, void* pSourcePtr);

	void ExecuteInterface(string& strURI, char* pReqBody, int nReqBodyLen, string& strReturn);
private:
	string GetConfigBufferString();
	string GetAppsBufferString();

	static void __stdcall AcceptConnectCallback(SOCKADDR_IN* pRemoteAddr, SOCKET s, DWORD dwUser);

	CServerThreadPool m_poolServer;
	CStaticNetServer m_netServer;

	CMemedFileSystem m_memFiles;
	CDiskFileSystem m_diskFiles;

	CInterfaceManager m_interfaceManger;
};


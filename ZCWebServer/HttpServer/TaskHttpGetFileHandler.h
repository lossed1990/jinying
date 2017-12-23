#pragma once

#include "StaticHttpServer.h"
#include "StaticFileInfo.h"

#include "../ThreadPool/ServerTask.h"
#include "../SocketBase/SocketBase.h"

class CTaskHttpGetFileHandler : public CServerTask
{
public:
	CTaskHttpGetFileHandler(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strURI, string &strParam);
	virtual ~CTaskHttpGetFileHandler();

public:
	virtual void prepareTask();
	virtual void executeTask();
	virtual void cleanupTask();

private:
	CStaticHttpServer* m_pServer;
	CSocketBase* m_pClientSocket;

	CStaticFileInfo m_fileInfo;
};


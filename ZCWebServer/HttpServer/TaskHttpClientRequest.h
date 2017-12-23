#pragma once

#include "../ThreadPool/ServerTask.h"
#include "../SocketBase/SocketBase.h"

#include "StaticHttpServer.h"

class CTaskHttpClientRequest : public CServerTask
{
public:
	CTaskHttpClientRequest(CStaticHttpServer* pServer, CSocketBase* pSocket, SOCKADDR_IN* pRemoteAddr);
	virtual ~CTaskHttpClientRequest();

public:
	virtual void prepareTask();
	virtual void executeTask();
	virtual void cleanupTask();

private:
	CSocketBase* m_psClientSocket;
	CStaticHttpServer* m_pServer;
	SOCKADDR_IN m_addrClient;
	
	void* m_pRecvReuqestBuff;

private:
	void HandleGET(CSimpleHttpRequest& sReq);
	void HandlePOST(CSimpleHttpRequest& sReq);
};
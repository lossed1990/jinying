#pragma once

#include "StaticHttpServer.h"
#include "StaticFileInfo.h"

#include "../ThreadPool/ServerTask.h"
#include "../SocketBase/SocketBase.h"

class CTaskHttpPostHandler : public CServerTask
{
public:
	CTaskHttpPostHandler(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strURI, char* pReqBody, int nBodyLen);
	virtual ~CTaskHttpPostHandler();

public:
	virtual void prepareTask();
	virtual void executeTask();
	virtual void cleanupTask();

public:
	CServerTask* createPostHandlerTask(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strURI, string& strReqBody);

private:
	CStaticHttpServer* m_pServer;
	CSocketBase* m_pClientSocket;

	string m_strURI;

	char* m_pReqBodyBuf;
	int m_nReqBodyLen;
	//string m_strReqBody;
};


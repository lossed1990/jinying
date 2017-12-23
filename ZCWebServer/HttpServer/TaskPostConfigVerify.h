#pragma once

#include "StaticHttpServer.h"

#include "../ThreadPool/ServerTask.h"
#include "../SocketBase/SocketBase.h"

class CTaskPostConfigVerify : public CServerTask
{
public:
	CTaskPostConfigVerify(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strReqBody);
	virtual ~CTaskPostConfigVerify();

public:
	virtual void prepareTask();
	virtual void executeTask();
	virtual void cleanupTask();

private:
	void sendForbidden();
	void redirectConfig(string& strUserName, int nUserType);

private:
	CStaticHttpServer* m_pServer;
	CSocketBase* m_pClientSocket;

	string m_strReqBody;
};


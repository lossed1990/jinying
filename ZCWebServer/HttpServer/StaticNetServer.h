#pragma once

#include "../SocketBase/SocketBase.h"

//接收外部连接回调，尽量不要阻塞
typedef void (__stdcall *ACCEPT_CONNECTION_CALLBACK)(SOCKADDR_IN* pRemoteAddr, SOCKET s, DWORD dwUser);

class CStaticNetServer
{
public:
	CStaticNetServer();
	~CStaticNetServer();

public:
	static unsigned __stdcall ServerListenThread(void*);

	int StartListenThread(int nPort, ACCEPT_CONNECTION_CALLBACK pfCB, DWORD dwUser);
	int StopServer();

private:
	int m_nListenPort;
	int m_nServerType;
	CSocketBase m_sListenSocket;

	BOOL m_bIsExit;
	BOOL m_bIsListenning;

	ACCEPT_CONNECTION_CALLBACK m_pfAcceptCB;
	DWORD m_dwUser;
};


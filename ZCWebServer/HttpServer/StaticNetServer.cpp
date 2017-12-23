#include "StaticNetServer.h"

#include <process.h>

CStaticNetServer::CStaticNetServer()
{
	m_bIsExit = FALSE;
	m_bIsListenning = FALSE;

	m_pfAcceptCB = NULL;
	m_dwUser = 0;
}


CStaticNetServer::~CStaticNetServer()
{
}

//监听线程
unsigned __stdcall CStaticNetServer::ServerListenThread(void* pParam)
{
	CStaticNetServer* pThis = (CStaticNetServer*)pParam;

	if (pThis == NULL) return 0;

	//建立监听线程，阻塞监听
	pThis->m_bIsListenning = TRUE;

	pThis->m_sListenSocket.StartListenAndAccept(NULL, pThis->m_pfAcceptCB, pThis->m_dwUser, &pThis->m_bIsExit, 1);

	pThis->m_bIsListenning = FALSE;

	return 0;
}

//开启监听
int CStaticNetServer::StartListenThread(int nPort, ACCEPT_CONNECTION_CALLBACK pfCB, DWORD dwUser)
{
	//尝试绑定端口
	if (!m_sListenSocket.BindOnPort(nPort))
	{
		return -1;
	}

	//端口绑定OK
	m_nListenPort = nPort;

	//记录用户参数
	m_pfAcceptCB = pfCB;
	m_dwUser = dwUser;

	//开启新处理线程
	HANDLE hThread;
	unsigned unThreadID;

	//设置退出标记
	m_bIsExit = FALSE;

	hThread = (HANDLE)_beginthreadex(NULL, 0, ServerListenThread, (void*)this, 0, &unThreadID);

	//已经不需要此HANDLE
	CloseHandle(hThread);

	return 0;
}

//结束监听
int CStaticNetServer::StopServer()
{
	m_bIsExit = TRUE;

	while (m_bIsListenning)
	{
		Sleep(100);
	}

	//销毁socket，解除绑定
	m_sListenSocket.Destroy();

	return 0;
}

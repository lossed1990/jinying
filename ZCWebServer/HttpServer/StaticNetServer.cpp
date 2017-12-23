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

//�����߳�
unsigned __stdcall CStaticNetServer::ServerListenThread(void* pParam)
{
	CStaticNetServer* pThis = (CStaticNetServer*)pParam;

	if (pThis == NULL) return 0;

	//���������̣߳���������
	pThis->m_bIsListenning = TRUE;

	pThis->m_sListenSocket.StartListenAndAccept(NULL, pThis->m_pfAcceptCB, pThis->m_dwUser, &pThis->m_bIsExit, 1);

	pThis->m_bIsListenning = FALSE;

	return 0;
}

//��������
int CStaticNetServer::StartListenThread(int nPort, ACCEPT_CONNECTION_CALLBACK pfCB, DWORD dwUser)
{
	//���԰󶨶˿�
	if (!m_sListenSocket.BindOnPort(nPort))
	{
		return -1;
	}

	//�˿ڰ�OK
	m_nListenPort = nPort;

	//��¼�û�����
	m_pfAcceptCB = pfCB;
	m_dwUser = dwUser;

	//�����´����߳�
	HANDLE hThread;
	unsigned unThreadID;

	//�����˳����
	m_bIsExit = FALSE;

	hThread = (HANDLE)_beginthreadex(NULL, 0, ServerListenThread, (void*)this, 0, &unThreadID);

	//�Ѿ�����Ҫ��HANDLE
	CloseHandle(hThread);

	return 0;
}

//��������
int CStaticNetServer::StopServer()
{
	m_bIsExit = TRUE;

	while (m_bIsListenning)
	{
		Sleep(100);
	}

	//����socket�������
	m_sListenSocket.Destroy();

	return 0;
}

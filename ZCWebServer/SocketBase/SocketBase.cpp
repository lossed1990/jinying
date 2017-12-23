#include "SocketBase.h"

#include <Ws2tcpip.h>
#include <process.h>
#include <jemalloc/jemalloc.h>
#include <string>
#include <vector>
using namespace std;

extern HANDLE g_hBoundaryLock;
extern vector<string> g_vecBoundary;

void CSocketBase::InitSocketLib()
{
	//��ʼ��SOCKET����
	WORD wVersionRequested;
	WSADATA wsaData;
	int nErr;

	wVersionRequested = MAKEWORD(2, 2);

	nErr = WSAStartup(wVersionRequested, &wsaData);
	if (nErr != 0) 
	{
		return;
	}
}

void CSocketBase::ReleaseSocketLib()
{
	//����SOCKET����
	WSACleanup();
}

CSocketBase::CSocketBase()
{
    m_socket = NULL;
    m_SocketWriteEvent = NULL;
	m_SocketReadEvent = NULL;

    m_bIsConnected = false;

    //��ʼ��Զ�̲���
    memset(m_pcRemoteIP, 0, SOB_IP_LENGTH);
    m_sRemotePort = 0;
}

CSocketBase::~CSocketBase()
{
    if(m_socket)
    {
        closesocket(m_socket);
		m_socket = NULL;
    }

    if(m_SocketWriteEvent || m_SocketReadEvent)
    {
        WSACloseEvent(m_SocketWriteEvent);
		WSACloseEvent(m_SocketReadEvent);
		m_SocketWriteEvent = NULL;
		m_SocketReadEvent = NULL;
    }
}

void CSocketBase::detachSocket()
{
	WSAEventSelect(m_socket, m_SocketWriteEvent, 0);
	WSAEventSelect(m_socket, m_SocketReadEvent, 0);

	WSACloseEvent(m_SocketWriteEvent);
	WSACloseEvent(m_SocketReadEvent);
	m_SocketWriteEvent = NULL;
	m_SocketReadEvent = NULL;

	m_bIsConnected = false;
	m_socket = NULL;
}

bool CSocketBase::attachRawSocket(SOCKET s, bool bIsConnected)
{
	//�й�SOCKET
	m_socket = s;

	//�����йܵ�SOCKET��Ĭ��������
	m_bIsConnected = bIsConnected;

	//�����첽�¼�
	if(m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if(m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	//��ȡԶ����Ϣ
	if(bIsConnected)
	{
		struct sockaddr_in addrPeer;
		int nLen = sizeof(addrPeer);
		if(getpeername(s, (struct sockaddr*)&addrPeer, &nLen) == SOCKET_ERROR)
		{
			return false;
		}

		strcpy(m_pcRemoteIP, inet_ntoa(addrPeer.sin_addr));
		m_sRemotePort = ntohs(addrPeer.sin_port);
	}

	return true;
}

void CSocketBase::Destroy()
{
	this->~CSocketBase();
}

//------------------------------------------------------------------------------
// ����socket
//------------------------------------------------------------------------------
SOCKET CSocketBase::CreateTCPSocket()
{
    //����socket���������е�ǰ��
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //�������������������Ϊ��
    if(s == INVALID_SOCKET)
    {
        s = NULL;
        m_nLastWSAError = WSAGetLastError();
    }

	//�����첽�¼�
	if(m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if(m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	//����SOCKET����
	const char chOpt = 1;
	int nRet = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));

    return s;    
}

//------------------------------------------------------------------------------
// ����socket
//------------------------------------------------------------------------------
SOCKET CSocketBase::CreateUDPSocket()
{
	//����socket���������е�ǰ��
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//�������������������Ϊ��
	if(s == INVALID_SOCKET)
	{
		s = NULL;
		m_nLastWSAError = WSAGetLastError();
	}

	//�����첽�¼�
	if(m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if(m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	return s;    
}

//------------------------------------------------------------------------------
// ���ý��ܳ�ʱ
//------------------------------------------------------------------------------
void CSocketBase::SetRecvTimeOut(UINT uiMSec)
{
    UINT uiMseTimeOut = uiMSec;

    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

//------------------------------------------------------------------------------
// ���÷��ͳ�ʱ
//------------------------------------------------------------------------------
void CSocketBase::SetSendTimeOut(UINT uiMSec)
{
    UINT uiMseTimeOut = uiMSec;

    setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

//------------------------------------------------------------------------------
// ���ý��ܻ���
//------------------------------------------------------------------------------
void CSocketBase::SetRecvBufferSize(UINT uiByte)
{
    UINT uiBufferSize = uiByte;

    setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&uiBufferSize, sizeof(uiBufferSize));    
}

//------------------------------------------------------------------------------
// ���÷��ͻ���
//------------------------------------------------------------------------------
void CSocketBase::SetSendBufferSize(UINT uiByte)
{
    UINT uiBufferSize = uiByte;

    setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&uiBufferSize, sizeof(uiBufferSize));    
}

//------------------------------------------------------------------------------
// socket
//------------------------------------------------------------------------------
SOCKET CSocketBase::getRawSocket()
{
    return m_socket;
}

//------------------------------------------------------------------------------
// �󶨵�TCP�˿�
//------------------------------------------------------------------------------
bool CSocketBase::BindOnPort(UINT uiPort)
{
	//���socket��Ч���½���Ϊ�˿����ظ�����
	if(m_socket == NULL)
	{
		m_socket = CreateTCPSocket();
	}	

	//�������ؼ���
	SOCKADDR_IN addrLocal;
	memset(&addrLocal, 0, sizeof(addrLocal));

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);
	addrLocal.sin_port = htons(uiPort);

	//��
	int nRet = bind(m_socket, (PSOCKADDR)&addrLocal, sizeof(addrLocal));

	//Ψһ��ԭ���Ƕ˿ڱ�ռ��
	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
// �󶨵�UDP�˿�
//------------------------------------------------------------------------------
bool CSocketBase::BindOnUDPPort(UINT uiPort)
{
	//���socket��Ч���½���Ϊ�˿����ظ�����
	if(m_socket == NULL)
	{
		m_socket = CreateUDPSocket();
	}	

	//�������ؼ���
	SOCKADDR_IN addrLocal;
	memset(&addrLocal, 0, sizeof(addrLocal));

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);
	addrLocal.sin_port = htons(uiPort);

	//��
	int nRet = bind(m_socket, (PSOCKADDR)&addrLocal, sizeof(addrLocal));

	//Ψһ��ԭ���Ƕ˿ڱ�ռ��
	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	return true;	
}

//------------------------------------------------------------------------------
// ��ʼ�������ӣ�����������
//------------------------------------------------------------------------------
bool CSocketBase::StartListenAndAccept(HANDLE_ACCEPT_THREAD pThreadFunc, HANDLE_ACCEPT_CALLBACK pCallback, DWORD dwUser, BOOL* pExitFlag, USHORT nLoopTimeOutSec)
{
	//ע�������¼�
	WSAResetEvent(m_SocketReadEvent);           //���֮ǰ��δ������¼�
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_ACCEPT | FD_CLOSE);

	//����
	int nRet = listen(m_socket, 8);

	//Ψһ��ԭ���Ƕ˿ڱ�ռ��
	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	//�ȴ�����
	while((pExitFlag == NULL ? TRUE : !(*pExitFlag)))
	{
		DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nLoopTimeOutSec*1000, FALSE);

		if(dwRet == WSA_WAIT_EVENT_0)
		{
			//��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			WSAResetEvent(m_SocketReadEvent);
			WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

			if((wsaEvents.lNetworkEvents & FD_ACCEPT) &&
				(wsaEvents.iErrorCode[FD_ACCEPT_BIT] == 0))
			{
				//��¼Զ�̵�ַ
				SOCKADDR_IN addrRemote;
				memset(&addrRemote, 0, sizeof(addrRemote));
				int nAddrSize = sizeof(addrRemote);

				SOCKET sockRemote = accept(m_socket, (PSOCKADDR)&addrRemote, &nAddrSize);

				//��Ч����
				if(sockRemote == INVALID_SOCKET)
				{
					m_nLastWSAError = WSAGetLastError();

					//���̳߳�������
					continue;
				}

				//��������̺߳����������߳�
				if (pThreadFunc)
				{
					//�����´����߳�
					HANDLE hThread;
					unsigned unThreadID;

					hThread = (HANDLE)_beginthreadex(NULL, 0, pThreadFunc, (void*)sockRemote, 0, &unThreadID);

					//�Ѿ�����Ҫ��HANDLE
					CloseHandle(hThread);
				}
				else if (pCallback)		//�������ص�����лص�
				{
					pCallback(&addrRemote, sockRemote, dwUser);
				}
			}
		}
		else
		{
			//�ȴ���ʱ�����¿�ʼ
			continue;
		}
	}

	return true;
}

//------------------------------------------------------------------------------
// ���ӵ�Զ�̣������������޲ε��ã���ʾ����
//------------------------------------------------------------------------------
bool CSocketBase::ConnectRemote(const char* pcRemoteIP, SHORT sPort, USHORT nTimeOutSec)
{
    //���socket��Ч���½���Ϊ�˿����ظ�����
    if(m_socket == NULL)
    {
        m_socket = CreateTCPSocket();
    }

    //�趨Զ��IP
    SOCKADDR_IN addrRemote;
    memset(&addrRemote, 0, sizeof(addrRemote));

    //�����Ҫ�����²����������ʾ�õ�ǰIP����
    if(pcRemoteIP != NULL && sPort != 0)
    {
        memset(m_pcRemoteIP, 0, SOB_IP_LENGTH);
        strcpy(m_pcRemoteIP, pcRemoteIP);
        m_sRemotePort = sPort;
    }
    
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_addr.S_un.S_addr = inet_addr(m_pcRemoteIP);
    addrRemote.sin_port = htons(m_sRemotePort);

    //ע�������¼�
    WSAResetEvent(m_SocketWriteEvent);           //���֮ǰ��δ������¼�
    WSAEventSelect(m_socket, m_SocketWriteEvent, FD_CONNECT | FD_CLOSE);

    //��������
    int nRet = connect(m_socket, (SOCKADDR*)&addrRemote, sizeof(addrRemote));

    if(nRet == SOCKET_ERROR)
    {
        m_nLastWSAError = WSAGetLastError();

        //�����������
        if(m_nLastWSAError == WSAEWOULDBLOCK)
        {
            DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec*1000, FALSE);

            //��������¼�����
            WSANETWORKEVENTS wsaEvents;
            memset(&wsaEvents, 0, sizeof(wsaEvents));

            if(dwRet == WSA_WAIT_EVENT_0)
            {
                WSAResetEvent(m_SocketWriteEvent);
                WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

                //������ӽ��ܲ���û�д�����
                if((wsaEvents.lNetworkEvents & FD_CONNECT) &&
                    (wsaEvents.iErrorCode[FD_CONNECT_BIT] == 0))
                {
                    //ʵ�ʵĵ��Ծ��������ң����������ͣ�٣��ᱨ�����˷ѽ��10022����
                    //����ͬһsocket������connect���̫�̿����ǲ��е�,���������Magic Number��50������ʵ�����Ľ����Sleep(1)��һ��ʱ��Ƭ�ƺ�����
                    Sleep(50);
                    nRet = connect(m_socket, (SOCKADDR*)&addrRemote, sizeof(addrRemote));

                    if(nRet == SOCKET_ERROR)
                    {
                        m_nLastWSAError = WSAGetLastError();

                        //�������ʵ�����Ѿ�����
                        if(m_nLastWSAError == WSAEISCONN)
                        {
                            m_bIsConnected = true;
                        }
                    }
                    else    //���������ӳɹ�
                    {
                        m_bIsConnected = true;
                    }
                }
            }
        }
        else
        {
           m_bIsConnected = false;
        }

    }
    else
    {
        //��һ�γ��Ա����ӳɹ�
        m_bIsConnected = true;
    }

    //�������ʧ��
    if(!m_bIsConnected)
    {
        closesocket(m_socket);
        m_socket = NULL;
    }

    //���ؽ������
    return m_bIsConnected;
}

//------------------------------------------------------------------------------
// ��Զ�̶Ͽ�����
//------------------------------------------------------------------------------
void CSocketBase::Disconnect()
{
    if(m_socket == NULL)
    {
        return;
    }

    //�����������״̬��ر�����
    if(m_bIsConnected)
    {
        shutdown(m_socket, SD_BOTH);

        m_bIsConnected = false;
    }

    closesocket(m_socket);
    m_socket = NULL;
}

//------------------------------------------------------------------------------
// ��������
//------------------------------------------------------------------------------
bool CSocketBase::Reconnect()
{
    Disconnect();

    Sleep(100);

    return ConnectRemote();    
}

bool CSocketBase::IsConnected()
{
    return m_bIsConnected;
}

const char* CSocketBase::getRemoteIP()
{
    if(m_pcRemoteIP[0] == 0)
    {
        return NULL;
    }

    return m_pcRemoteIP;
}

ULONG CSocketBase::getRemoteULIP()
{
	return inet_addr(m_pcRemoteIP);
}

SHORT CSocketBase::getRemotePort()
{
    return m_sRemotePort;    
}

//------------------------------------------------------------------------------
// �����ı�����һ������
//------------------------------------------------------------------------------
int CSocketBase::SendMsg(const char* pcMsg, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

    //�������״̬
    if(!m_bIsConnected)
    {
        return SOB_RET_FAIL;
    }

    //����ǰע���¼�
    WSAResetEvent(m_SocketWriteEvent);
    WSAEventSelect(m_socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

    //���Է���
    int nRet = send(m_socket, pcMsg, (int)strlen(pcMsg), NULL);

    if(nRet == SOCKET_ERROR)
    {
        m_nLastWSAError = WSAGetLastError();

        //��������
        if(m_nLastWSAError == WSAEWOULDBLOCK)
        {
            DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec*1000, FALSE);

            //��������¼�����
            WSANETWORKEVENTS wsaEvents;
            memset(&wsaEvents, 0, sizeof(wsaEvents));

            if(dwRet == WSA_WAIT_EVENT_0)
            {
                WSAResetEvent(m_SocketWriteEvent);
                WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

                //������Ϳ��Խ��в���û�д�����
                if((wsaEvents.lNetworkEvents & FD_WRITE) &&
                    (wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
                {
                    //�ٴη����ı�
                    nRet = (int)send(m_socket, pcMsg, (int)strlen(pcMsg), NULL);

                    if(nRet > 0)
                    {
                        //��������ֽڴ���0���������ͳɹ�
                        return SOB_RET_OK;
                    }
                }
            }
			else
			{
				//��ʱ
				bIsTimeOut = true;
			}
        }
        else
        {
          m_bIsConnected = false;
        }
    }
    else
    {
        //��һ�α㷢�ͳɹ�
        return SOB_RET_OK;
    }

	//�����ʱ
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

    //�����������ʧ��
    m_nLastWSAError = WSAGetLastError();
    return false;
}

//------------------------------------------------------------------------------
// ���ͻ�����
//------------------------------------------------------------------------------
int CSocketBase::SendBuffer(char* pBuffer, UINT uiBufferSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

    //�������״̬
    if(!m_bIsConnected)
    {
        return SOB_RET_FAIL;
    }

    //����ǰע���¼�
    WSAResetEvent(m_SocketWriteEvent);
    WSAEventSelect(m_socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

    //����������
    int nSent = 0;

    //�ܷ��ʹ�����Ϊ���ʹ�����һ���޶�
    int nSendTimes = 0;
    int nSendLimitTimes = (int)((float)uiBufferSize / 500 + 1.5);      //�ٶ���ǰÿ�η��Ϳ϶�������500�ֽ�
    UINT uiLeftBuffer = uiBufferSize;									//δ������Ļ����С

    //�����α�
    char* pcSentPos = pBuffer;

    //ֱ�����еĻ��嶼�������
    while(nSent < (int)uiBufferSize)
    {
        //��鷢�ʹ����Ƿ���
        if(nSendTimes > nSendLimitTimes)
        {
            break;
        }

        int nRet = send(m_socket, pcSentPos, uiLeftBuffer, NULL);

        if(nRet == SOCKET_ERROR)
        {
            m_nLastWSAError = WSAGetLastError();

            //��������
            if(m_nLastWSAError == WSAEWOULDBLOCK)
            {
                DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec*1000, FALSE);

                //��������¼�����
                WSANETWORKEVENTS wsaEvents;
                memset(&wsaEvents, 0, sizeof(wsaEvents));

                if(dwRet == WSA_WAIT_EVENT_0)
                {
                    WSAResetEvent(m_SocketWriteEvent);
                    WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

                    //������Ϳ��Խ��в���û�д�����
                    if((wsaEvents.lNetworkEvents & FD_WRITE) &&
                        (wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
                    {
                        //�ٴη����ı�
                        nRet = send(m_socket, pcSentPos, uiLeftBuffer, NULL);

                        if(nRet > 0)
                        {
                            //��������ֽڴ���0���������ͳɹ�
                            nSendTimes++;

                            nSent += nRet;
                            uiLeftBuffer -= nRet;
                            pcSentPos += nRet;
                        }
                        else
                        {
                            m_nLastWSAError = WSAGetLastError();

                            //������յ��¼�����ʱҲ�����ˣ���ȴ�һ�����ں�����
                            if(m_nLastWSAError == WSAEWOULDBLOCK)
                            {
                                Sleep(1);        
                            }
                            else
                            {
                                //������������ֱ���˳�
                                break;
                            }
                        }
                    }
                }
				else
				{
					//��ʱ
					bIsTimeOut = true;
					break;
				}
            }
            else
            {
                //��������֮��Ĵ���ֱ���˳�
                m_bIsConnected = false;
                break;
            }
        }
        else
        {
            //���ͳɹ����ۼӷ������������α�
            nSendTimes++;
            
            nSent += nRet;
            uiLeftBuffer -= nRet;
            pcSentPos += nRet;
        }
    }

    //����������
    if(nSent == uiBufferSize)
    {
        return SOB_RET_OK;
    }

	//�����ʱ
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

    //û�ܳɹ�����
    m_nLastWSAError = WSAGetLastError();
    return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// ����UDP���壬�����ڵײ����ݱ���Χ��
//------------------------------------------------------------------------------
int CSocketBase::SendUDPBuffer(const char* pcIP, SHORT sPort, char* pBuffer, UINT uiBufferSize, USHORT nTimeOutSec)
{
	if(m_socket == NULL)
	{
		m_socket = CreateUDPSocket();
	}

	bool bIsTimeOut = false;

	//����������
	int nSent = 0;

	//�ܷ��ʹ�����Ϊ���ʹ�����һ���޶�
	int nSendTimes = 0;
	int nSendLimitTimes = nTimeOutSec * 1000 / 100;						//��������������ȴ�100ms���ط�
	UINT uiLeftBuffer = uiBufferSize;									//δ������Ļ����С

	//ת��Զ�̵�ַ
	SOCKADDR_IN addrRemote;
	memset(&addrRemote, 0, sizeof(addrRemote));

	addrRemote.sin_family = AF_INET;
	addrRemote.sin_addr.s_addr = inet_addr(pcIP);
	addrRemote.sin_port = htons(sPort);

	//�����α�
	char* pcSentPos = pBuffer;

	//ֱ�����еĻ��嶼�������
	while(nSent < (int)uiBufferSize)
	{
		//��鷢�ʹ����Ƿ���
		if(nSendTimes > nSendLimitTimes)
		{
			bIsTimeOut = true;
			break;
		}

		int nRet = sendto(m_socket, pcSentPos, uiLeftBuffer, NULL, (PSOCKADDR)&addrRemote, sizeof(addrRemote));

		if(nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();
			break;
		}
		else
		{
			//���ͳɹ����ۼӷ������������α�
			nSendTimes++;

			nSent += nRet;
			uiLeftBuffer -= nRet;
			pcSentPos += nRet;
		}
	}

	//����������
	if(nSent == uiBufferSize)
	{
		return SOB_RET_OK;
	}

	//�����ʱ
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();
	return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// ����UDP����
//------------------------------------------------------------------------------
int CSocketBase::RecvUDPMsg(char* pBuffer, UINT uiBufferSize, UINT& uiRecv, char* pcIP, USHORT& uPort, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	//����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ);

	//Զ����Ϣ
	SOCKADDR_IN addrRemote;
	int nAddrLen = sizeof(addrRemote);
	memset(&addrRemote, 0, nAddrLen);

	//���Խ���
	int nRet = recvfrom(m_socket, pBuffer, uiBufferSize, NULL, (PSOCKADDR)&addrRemote, &nAddrLen);

	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		//��������
		if(m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec*1000, FALSE);

			//��������¼�����
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if(dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketReadEvent);
				int nEnum = WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

				//������ܿ��Խ��в���û�д�����
				if((wsaEvents.lNetworkEvents & FD_READ) &&
					(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
				{
					//�ٴν����ı�
					nRet = recvfrom(m_socket, pBuffer, uiBufferSize, NULL, (PSOCKADDR)&addrRemote, &nAddrLen);

					if(nRet > 0)
					{
						//����ֽڴ���0���������ճɹ�
						uiRecv = nRet;

						//����IP�Ͷ˿�
						strcpy(pcIP, inet_ntoa(addrRemote.sin_addr));
						uPort = ntohs(addrRemote.sin_port);

						return SOB_RET_OK;
					}
				}
			}
			else
			{
				bIsTimeOut = true;
			}
		}
	}
	else
	{
		//��һ�α���ճɹ�
		uiRecv = nRet;

		//����IP�Ͷ˿�
		strcpy(pcIP, inet_ntoa(addrRemote.sin_addr));
		uPort = ntohs(addrRemote.sin_port);

		return SOB_RET_OK;
	}

	//�����ʱ
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//�����������ʧ��
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// �����ı�����һ�ν���
//------------------------------------------------------------------------------
int CSocketBase::RecvOnce(char* pRecvBuffer, UINT uiBufferSize, UINT& uiRecv, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

    //�������״̬
    if(!m_bIsConnected)
    {
        return SOB_RET_FAIL;
    }

    //����ǰע���¼�
    WSAResetEvent(m_SocketReadEvent);
    WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

    //���Խ���
    int nRet = recv(m_socket, pRecvBuffer, uiBufferSize, NULL);

    if(nRet == SOCKET_ERROR)
    {
        m_nLastWSAError = WSAGetLastError();

        //��������
        if(m_nLastWSAError == WSAEWOULDBLOCK)
        {
            DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec*1000, FALSE);

            //��������¼�����
            WSANETWORKEVENTS wsaEvents;
            memset(&wsaEvents, 0, sizeof(wsaEvents));

            if(dwRet == WSA_WAIT_EVENT_0)
            {
                WSAResetEvent(m_SocketReadEvent);
                int nEnum = WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

                //������ܿ��Խ��в���û�д�����
                if((wsaEvents.lNetworkEvents & FD_READ) &&
                    (wsaEvents.iErrorCode[FD_READ_BIT] == 0))
                {
                    //�ٴν����ı�
                    nRet = recv(m_socket, pRecvBuffer, uiBufferSize, NULL);

                    if(nRet > 0)
                    {
                        //��������ֽڴ���0���������ͳɹ�
                        uiRecv = nRet;
                        return SOB_RET_OK;
                    }
                }
            }
			else
			{
				bIsTimeOut = true;
			}
        }
        else
        {
           m_bIsConnected = false;
        }
    }
    else
    {
        //��һ�α���ճɹ�
        uiRecv = nRet;
        return SOB_RET_OK;
    }

	//�����ʱ
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

    //�����������ʧ��
    m_nLastWSAError = WSAGetLastError();
    return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// ���ջ�����������Ӧ�ñȴ�������Ҫ��һ��Ű�ȫ
//------------------------------------------------------------------------------
int CSocketBase::RecvBuffer(char* pRecvBuffer, UINT uiBufferSize, UINT& uiRecvSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	//�������״̬
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	//����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	//����������
	int nReceived = 0;

	//�ܽ��մ�����Ϊ���մ�����һ���޶�
	int nRecvTimes = 0;
	int nRecvLimitTimes = (int)((float)uiRecvSize / 500 + 1.5);      //�ٶ���ǰÿ�ν��տ϶�������500�ֽ�

	//�����α�
	char* pcRecvPos = pRecvBuffer;

	//ֱ�����յ��㹻ָ�����Ļ���
	while (nReceived < (int)uiRecvSize)
	{
		//�����մ����Ƿ���
		if (nRecvTimes > nRecvLimitTimes)
		{
			break;
		}

		int nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			//��������
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

				//��������¼�����
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketReadEvent);
					WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

					//������տ��Խ��в���û�д�����
					if ((wsaEvents.lNetworkEvents & FD_READ) &&
						(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
					{
						//�ٴν���
						nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

						if (nRet > 0)
						{
							//��������ֽڴ���0���������ͳɹ�
							nRecvTimes++;

							nReceived += nRet;
							uiBufferSize -= nRet;
							pcRecvPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							//������յ��¼�����ʱҲ�����ˣ���ȴ�һ�����ں�����
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								//������������ֱ���˳�
								break;
							}
						}
					}
				}
				else
				{
					//��ʱ
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				//��������֮��Ĵ���ֱ���˳�
				m_bIsConnected = false;
				break;
			}
		}
		else if (nRet == 0)		//�Է������ر�����
		{
			m_bIsConnected = false;
			break;
		}
		else
		{
			//���ճɹ����ۼӽ������������α�
			nRecvTimes++;

			nReceived += nRet;
			uiBufferSize -= nRet;
			pcRecvPos += nRet;
		}
	}

	//����������
	if (nReceived == uiRecvSize)
	{
		return SOB_RET_OK;
	}

	//�����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();
	return SOB_RET_FAIL;
}
int CSocketBase::RecvHttpBuff(char** ppRecvBuffer, UINT& uiRecvSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	//�������״̬
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	//����ǰע���¼�
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	bool bFinished = false;

	// ʵ�ʽ����ֽ���
	int nRecievedSize = 0;
	*ppRecvBuffer = (char*)je_malloc(RECV_REQUEST_BUFFSIZE_4K);	// �˴�new���ڴ����ⲿ�ͷţ���Ϊ������ܻ�ȡ����С��ֻ������new�������ͷţ�
	UINT uiBufferSize = RECV_REQUEST_BUFFSIZE_4K;

	// ��Ϣ��Ҫ���С
	int nActualBodySize = 0;
	int nRecvBodySize = 0;
	int nNeedSize = 0;

	//�����α�
	char* pcRecvPos = *ppRecvBuffer;

	//ֱ�����յ��㹻ָ�����Ļ���
	while (bFinished == false)
	{
		//�����մ����Ƿ���

		int nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			//��������
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

				//��������¼�����
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketReadEvent);
					WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

					//������տ��Խ��в���û�д�����
					if ((wsaEvents.lNetworkEvents & FD_READ) &&
						(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
					{
						//�ٴν���
						nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

						if (nRet > 0)
						{
							//��������ֽڴ���0���������ͳɹ�
							nRecievedSize += nRet;
							uiBufferSize -= nRet;
							pcRecvPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							//������յ��¼�����ʱҲ�����ˣ���ȴ�һ�����ں�����
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								//������������ֱ���˳�
								break;
							}
						}
					}
				}
				else
				{
					//��ʱ
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				//��������֮��Ĵ���ֱ���˳�
				m_bIsConnected = false;
				break;
			}
		}
		else if (nRet == 0)		//�Է������ر�����
		{
			m_bIsConnected = false;
			break;
		}
		else
		{
			//���ճɹ����ۼӽ������������α�
			nRecievedSize += nRet;
			uiBufferSize -= nRet;
			pcRecvPos += nRet;
		}
		if (CheckHttpIsFinish(*ppRecvBuffer, nRecievedSize, nRecvBodySize) == TRUE)
		{
			bFinished = true;
		}
		if (nNeedSize == 0)
		{
			nNeedSize = nRecvBodySize + 512;
			int nNeedSizeTmp = 0;
			void* pTmp = NULL;
			if (nNeedSize >= RECV_REQUEST_BUFFSIZE_4K && nNeedSize < RECV_REQUEST_BUFFSIZE_512K)
				nNeedSizeTmp = RECV_REQUEST_BUFFSIZE_512K;
			if (nNeedSize >= RECV_REQUEST_BUFFSIZE_512K && nNeedSize < RECV_REQUEST_BUFFSIZE_1M)
				nNeedSizeTmp = RECV_REQUEST_BUFFSIZE_1M;
			if (nNeedSize >= RECV_REQUEST_BUFFSIZE_1M && nNeedSize < RECV_REQUEST_BUFFSIZE_4M)
				nNeedSizeTmp = RECV_REQUEST_BUFFSIZE_4M;
			if (nNeedSize >= RECV_REQUEST_BUFFSIZE_4M && nNeedSize < RECV_REQUEST_BUFFSIZE_16M)
				nNeedSizeTmp = RECV_REQUEST_BUFFSIZE_16M;
			if (nNeedSize >= RECV_REQUEST_BUFFSIZE_16M && nNeedSize < RECV_REQUEST_BUFFSIZE_64M)
				nNeedSizeTmp = RECV_REQUEST_BUFFSIZE_64M;
			if (nNeedSize >= RECV_REQUEST_BUFFSIZE_64M && nNeedSize < RECV_REQUEST_BUFFSIZE_128M)
				nNeedSizeTmp = RECV_REQUEST_BUFFSIZE_128M;
			if (nNeedSizeTmp > 0)
			{
				pTmp = je_malloc(nNeedSizeTmp);
				memset(pTmp, 0, nNeedSizeTmp);
				memcpy(pTmp, *ppRecvBuffer, nRecievedSize);
				je_free(*ppRecvBuffer);
				*ppRecvBuffer = (char*)pTmp;
				pcRecvPos = (*ppRecvBuffer) + nRecievedSize;
				uiBufferSize = nNeedSizeTmp - nRecievedSize;
			}
		}
	}

	//����������
	if (bFinished)
	{
		uiRecvSize = nRecievedSize;
		return SOB_RET_OK;
	}

	//�����ʱ
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//û�ܳɹ�����
	m_nLastWSAError = WSAGetLastError();
	return SOB_RET_FAIL;
}
//------------------------------------------------------------------------------
// ת����ַΪIP
//------------------------------------------------------------------------------
bool CSocketBase::ResolveAddressToIp(const char* pcAddress, char* pcIp)
{
	addrinfo adiHints, *padiResult;
	int	nRet;

	memset(&adiHints, 0, sizeof(addrinfo));

	//������IPV4�ĵ�ַ
	adiHints.ai_flags = AI_CANONNAME;
	adiHints.ai_family = AF_INET;
	adiHints.ai_socktype = SOCK_STREAM;
	adiHints.ai_protocol = IPPROTO_TCP;

	//ת����ַ
	nRet = ::getaddrinfo(pcAddress, NULL, &adiHints, &padiResult);
	
	//�����
	if(nRet != 0) 
	{
		freeaddrinfo(padiResult);
		return false;
	}
	
	//�������,ֻ������һ��
	if(padiResult->ai_addr != NULL)
	{
		::strcpy(pcIp, inet_ntoa(((sockaddr_in*)padiResult->ai_addr)->sin_addr));
	}
	
	freeaddrinfo(padiResult);
	return true;
}
//------------------------------------------------------------------------------
// �ж�http�����Ƿ�������
//------------------------------------------------------------------------------
bool CSocketBase::CheckHttpIsFinish(char* pBuff, int nSize, int &nNeedSize)
{
	bool bRet = false;

	//�������
	if (NULL == pBuff || strlen(pBuff) < 8)
	{
		return bRet;
	}

	bool bIsHeaderEnd = false;
	size_t nLength = strlen(pBuff);

	int nPos = 0;
	int nConnentLen = 0;
	bool IsHttpBuf = false;
	size_t i = 0;
	for (i = 0; i < (nLength - 4); i++)
	{
		//������ɣ������˳�
		if (bIsHeaderEnd)
		{
			break;
		}

		//�ҵ�����
		if (pBuff[i] == '\r' && pBuff[i + 1] == '\n')
		{
			//��ͨ�м��
			pBuff[i] = '\0';

			char* pcLine = &pBuff[nPos];

			if (strstr(pcLine, "HTTP/"))
			{
				IsHttpBuf = true;
			}
			else if (strstr(pcLine, "boundary="))
			{
				char* pBoundary = strstr(pcLine, "boundary=");
				WaitForSingleObject(g_hBoundaryLock, INFINITE);
				BOOL bExist = FALSE;
				for (int i = 0; i < g_vecBoundary.size(); i++)
				{
					if (g_vecBoundary[i] == pBoundary + strlen("boundary="))
					{
						bExist = TRUE;
						break;
					}
				}
				if (bExist == FALSE) g_vecBoundary.push_back(pBoundary + strlen("boundary="));
				SetEvent(g_hBoundaryLock);
				//strncpy_s(g_szBoundary, sizeof(g_szBoundary), pBoundary + strlen("boundary="), _TRUNCATE);
			}
			else if (strstr(pcLine, "Content-Length:"))
			{
				char* pcPos = strstr(pcLine, "Content-Length:");
				char szContLen[64] = { 0 };

				if (pcPos)
				{
					char* pcStart = &pcPos[strlen("Content-Length:")];

					//�Ƴ�ǰ���ո�
					while (*pcStart == ' ')
					{
						pcStart++;
					}

					strncpy_s(szContLen, sizeof(szContLen), pcStart, _TRUNCATE);
				}

				nConnentLen = atoi(szContLen);
				nNeedSize = nConnentLen;
			}

			//��ԭ
			pBuff[i] = '\r';
			nPos = i + 2;

			//�Ƿ�header��β
			if (pBuff[i + 2] == '\r' && pBuff[i + 3] == '\n')
			{
				//���header�����ı�����Ϊpost����
				if (nSize >= nConnentLen + i + 4)	// ���������ܴ�С
				{
					bRet = true;
				}
				//������header
				bIsHeaderEnd = true;
			}
		}
	}
	if (i >= (nLength - 4))
		bRet = true;

	return bRet;
}
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
	//初始化SOCKET环境
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
	//清理SOCKET环境
	WSACleanup();
}

CSocketBase::CSocketBase()
{
    m_socket = NULL;
    m_SocketWriteEvent = NULL;
	m_SocketReadEvent = NULL;

    m_bIsConnected = false;

    //初始化远程参数
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
	//托管SOCKET
	m_socket = s;

	//对于托管的SOCKET，默认其连接
	m_bIsConnected = bIsConnected;

	//创建异步事件
	if(m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if(m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	//获取远端信息
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
// 创建socket
//------------------------------------------------------------------------------
SOCKET CSocketBase::CreateTCPSocket()
{
    //创建socket，这是所有的前提
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //如果创建出错，我们置其为空
    if(s == INVALID_SOCKET)
    {
        s = NULL;
        m_nLastWSAError = WSAGetLastError();
    }

	//创建异步事件
	if(m_SocketWriteEvent == NULL)
	{
		m_SocketWriteEvent = WSACreateEvent();
	}

	if(m_SocketReadEvent == NULL)
	{
		m_SocketReadEvent = WSACreateEvent();
	}

	//设置SOCKET属性
	const char chOpt = 1;
	int nRet = setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &chOpt, sizeof(char));

    return s;    
}

//------------------------------------------------------------------------------
// 创建socket
//------------------------------------------------------------------------------
SOCKET CSocketBase::CreateUDPSocket()
{
	//创建socket，这是所有的前提
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//如果创建出错，我们置其为空
	if(s == INVALID_SOCKET)
	{
		s = NULL;
		m_nLastWSAError = WSAGetLastError();
	}

	//创建异步事件
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
// 设置接受超时
//------------------------------------------------------------------------------
void CSocketBase::SetRecvTimeOut(UINT uiMSec)
{
    UINT uiMseTimeOut = uiMSec;

    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

//------------------------------------------------------------------------------
// 设置发送超时
//------------------------------------------------------------------------------
void CSocketBase::SetSendTimeOut(UINT uiMSec)
{
    UINT uiMseTimeOut = uiMSec;

    setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&uiMseTimeOut, sizeof(uiMseTimeOut));
}

//------------------------------------------------------------------------------
// 设置接受缓冲
//------------------------------------------------------------------------------
void CSocketBase::SetRecvBufferSize(UINT uiByte)
{
    UINT uiBufferSize = uiByte;

    setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&uiBufferSize, sizeof(uiBufferSize));    
}

//------------------------------------------------------------------------------
// 设置发送缓冲
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
// 绑定到TCP端口
//------------------------------------------------------------------------------
bool CSocketBase::BindOnPort(UINT uiPort)
{
	//如果socket无效则新建，为了可以重复调用
	if(m_socket == NULL)
	{
		m_socket = CreateTCPSocket();
	}	

	//开启本地监听
	SOCKADDR_IN addrLocal;
	memset(&addrLocal, 0, sizeof(addrLocal));

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);
	addrLocal.sin_port = htons(uiPort);

	//绑定
	int nRet = bind(m_socket, (PSOCKADDR)&addrLocal, sizeof(addrLocal));

	//唯一的原因是端口被占用
	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
// 绑定到UDP端口
//------------------------------------------------------------------------------
bool CSocketBase::BindOnUDPPort(UINT uiPort)
{
	//如果socket无效则新建，为了可以重复调用
	if(m_socket == NULL)
	{
		m_socket = CreateUDPSocket();
	}	

	//开启本地监听
	SOCKADDR_IN addrLocal;
	memset(&addrLocal, 0, sizeof(addrLocal));

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);
	addrLocal.sin_port = htons(uiPort);

	//绑定
	int nRet = bind(m_socket, (PSOCKADDR)&addrLocal, sizeof(addrLocal));

	//唯一的原因是端口被占用
	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	return true;	
}

//------------------------------------------------------------------------------
// 开始接受连接，本方法阻塞
//------------------------------------------------------------------------------
bool CSocketBase::StartListenAndAccept(HANDLE_ACCEPT_THREAD pThreadFunc, HANDLE_ACCEPT_CALLBACK pCallback, DWORD dwUser, BOOL* pExitFlag, USHORT nLoopTimeOutSec)
{
	//注册连接事件
	WSAResetEvent(m_SocketReadEvent);           //清除之前尚未处理的事件
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_ACCEPT | FD_CLOSE);

	//监听
	int nRet = listen(m_socket, 8);

	//唯一的原因是端口被占用
	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();
		return false;
	}

	//等待连接
	while((pExitFlag == NULL ? TRUE : !(*pExitFlag)))
	{
		DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nLoopTimeOutSec*1000, FALSE);

		if(dwRet == WSA_WAIT_EVENT_0)
		{
			//如果网络事件发生
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			WSAResetEvent(m_SocketReadEvent);
			WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

			if((wsaEvents.lNetworkEvents & FD_ACCEPT) &&
				(wsaEvents.iErrorCode[FD_ACCEPT_BIT] == 0))
			{
				//记录远程地址
				SOCKADDR_IN addrRemote;
				memset(&addrRemote, 0, sizeof(addrRemote));
				int nAddrSize = sizeof(addrRemote);

				SOCKET sockRemote = accept(m_socket, (PSOCKADDR)&addrRemote, &nAddrSize);

				//无效连接
				if(sockRemote == INVALID_SOCKET)
				{
					m_nLastWSAError = WSAGetLastError();

					//本线程持续监听
					continue;
				}

				//如果定义线程函数则开启新线程
				if (pThreadFunc)
				{
					//开启新处理线程
					HANDLE hThread;
					unsigned unThreadID;

					hThread = (HANDLE)_beginthreadex(NULL, 0, pThreadFunc, (void*)sockRemote, 0, &unThreadID);

					//已经不需要此HANDLE
					CloseHandle(hThread);
				}
				else if (pCallback)		//如果定义回调则进行回调
				{
					pCallback(&addrRemote, sockRemote, dwUser);
				}
			}
		}
		else
		{
			//等待超时，重新开始
			continue;
		}
	}

	return true;
}

//------------------------------------------------------------------------------
// 连接到远程，本方法可以无参调用，表示重连
//------------------------------------------------------------------------------
bool CSocketBase::ConnectRemote(const char* pcRemoteIP, SHORT sPort, USHORT nTimeOutSec)
{
    //如果socket无效则新建，为了可以重复调用
    if(m_socket == NULL)
    {
        m_socket = CreateTCPSocket();
    }

    //设定远程IP
    SOCKADDR_IN addrRemote;
    memset(&addrRemote, 0, sizeof(addrRemote));

    //如果需要，更新参数，否则表示用当前IP重连
    if(pcRemoteIP != NULL && sPort != 0)
    {
        memset(m_pcRemoteIP, 0, SOB_IP_LENGTH);
        strcpy(m_pcRemoteIP, pcRemoteIP);
        m_sRemotePort = sPort;
    }
    
    addrRemote.sin_family = AF_INET;
    addrRemote.sin_addr.S_un.S_addr = inet_addr(m_pcRemoteIP);
    addrRemote.sin_port = htons(m_sRemotePort);

    //注册连接事件
    WSAResetEvent(m_SocketWriteEvent);           //清除之前尚未处理的事件
    WSAEventSelect(m_socket, m_SocketWriteEvent, FD_CONNECT | FD_CLOSE);

    //进行连接
    int nRet = connect(m_socket, (SOCKADDR*)&addrRemote, sizeof(addrRemote));

    if(nRet == SOCKET_ERROR)
    {
        m_nLastWSAError = WSAGetLastError();

        //如果发生阻塞
        if(m_nLastWSAError == WSAEWOULDBLOCK)
        {
            DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec*1000, FALSE);

            //如果网络事件发生
            WSANETWORKEVENTS wsaEvents;
            memset(&wsaEvents, 0, sizeof(wsaEvents));

            if(dwRet == WSA_WAIT_EVENT_0)
            {
                WSAResetEvent(m_SocketWriteEvent);
                WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

                //如果连接接受并且没有错误发生
                if((wsaEvents.lNetworkEvents & FD_CONNECT) &&
                    (wsaEvents.iErrorCode[FD_CONNECT_BIT] == 0))
                {
                    //实际的调试经历告诉我，这里如果不停顿，会报出令人费解的10022错误
                    //对于同一socket，两次connect间隔太短看来是不行的,至于这里的Magic Number的50，这是实践出的结果，Sleep(1)的一个时间片似乎不够
                    Sleep(50);
                    nRet = connect(m_socket, (SOCKADDR*)&addrRemote, sizeof(addrRemote));

                    if(nRet == SOCKET_ERROR)
                    {
                        m_nLastWSAError = WSAGetLastError();

                        //此种情况实际上已经连接
                        if(m_nLastWSAError == WSAEISCONN)
                        {
                            m_bIsConnected = true;
                        }
                    }
                    else    //阻塞后连接成功
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
        //第一次尝试便连接成功
        m_bIsConnected = true;
    }

    //如果连接失败
    if(!m_bIsConnected)
    {
        closesocket(m_socket);
        m_socket = NULL;
    }

    //返回接续结果
    return m_bIsConnected;
}

//------------------------------------------------------------------------------
// 和远程断开连接
//------------------------------------------------------------------------------
void CSocketBase::Disconnect()
{
    if(m_socket == NULL)
    {
        return;
    }

    //如果尚在连接状态则关闭连接
    if(m_bIsConnected)
    {
        shutdown(m_socket, SD_BOTH);

        m_bIsConnected = false;
    }

    closesocket(m_socket);
    m_socket = NULL;
}

//------------------------------------------------------------------------------
// 尝试重连
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
// 发送文本串，一次送完
//------------------------------------------------------------------------------
int CSocketBase::SendMsg(const char* pcMsg, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

    //检查连接状态
    if(!m_bIsConnected)
    {
        return SOB_RET_FAIL;
    }

    //发送前注册事件
    WSAResetEvent(m_SocketWriteEvent);
    WSAEventSelect(m_socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

    //尝试发送
    int nRet = send(m_socket, pcMsg, (int)strlen(pcMsg), NULL);

    if(nRet == SOCKET_ERROR)
    {
        m_nLastWSAError = WSAGetLastError();

        //遭遇阻塞
        if(m_nLastWSAError == WSAEWOULDBLOCK)
        {
            DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec*1000, FALSE);

            //如果网络事件发生
            WSANETWORKEVENTS wsaEvents;
            memset(&wsaEvents, 0, sizeof(wsaEvents));

            if(dwRet == WSA_WAIT_EVENT_0)
            {
                WSAResetEvent(m_SocketWriteEvent);
                WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

                //如果发送可以进行并且没有错误发生
                if((wsaEvents.lNetworkEvents & FD_WRITE) &&
                    (wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
                {
                    //再次发送文本
                    nRet = (int)send(m_socket, pcMsg, (int)strlen(pcMsg), NULL);

                    if(nRet > 0)
                    {
                        //如果发送字节大于0，表明发送成功
                        return SOB_RET_OK;
                    }
                }
            }
			else
			{
				//超时
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
        //第一次便发送成功
        return SOB_RET_OK;
    }

	//如果超时
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

    //如果上述发送失败
    m_nLastWSAError = WSAGetLastError();
    return false;
}

//------------------------------------------------------------------------------
// 发送缓冲区
//------------------------------------------------------------------------------
int CSocketBase::SendBuffer(char* pBuffer, UINT uiBufferSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

    //检查连接状态
    if(!m_bIsConnected)
    {
        return SOB_RET_FAIL;
    }

    //发送前注册事件
    WSAResetEvent(m_SocketWriteEvent);
    WSAEventSelect(m_socket, m_SocketWriteEvent, FD_WRITE | FD_CLOSE);

    //发送量计数
    int nSent = 0;

    //总发送次数，为发送次数定一个限额
    int nSendTimes = 0;
    int nSendLimitTimes = (int)((float)uiBufferSize / 500 + 1.5);      //假定当前每次发送肯定不少于500字节
    UINT uiLeftBuffer = uiBufferSize;									//未发送完的缓存大小

    //发送游标
    char* pcSentPos = pBuffer;

    //直到所有的缓冲都发送完毕
    while(nSent < (int)uiBufferSize)
    {
        //检查发送次数是否超限
        if(nSendTimes > nSendLimitTimes)
        {
            break;
        }

        int nRet = send(m_socket, pcSentPos, uiLeftBuffer, NULL);

        if(nRet == SOCKET_ERROR)
        {
            m_nLastWSAError = WSAGetLastError();

            //遭遇阻塞
            if(m_nLastWSAError == WSAEWOULDBLOCK)
            {
                DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketWriteEvent, FALSE, nTimeOutSec*1000, FALSE);

                //如果网络事件发生
                WSANETWORKEVENTS wsaEvents;
                memset(&wsaEvents, 0, sizeof(wsaEvents));

                if(dwRet == WSA_WAIT_EVENT_0)
                {
                    WSAResetEvent(m_SocketWriteEvent);
                    WSAEnumNetworkEvents(m_socket, m_SocketWriteEvent, &wsaEvents);

                    //如果发送可以进行并且没有错误发生
                    if((wsaEvents.lNetworkEvents & FD_WRITE) &&
                        (wsaEvents.iErrorCode[FD_WRITE_BIT] == 0))
                    {
                        //再次发送文本
                        nRet = send(m_socket, pcSentPos, uiLeftBuffer, NULL);

                        if(nRet > 0)
                        {
                            //如果发送字节大于0，表明发送成功
                            nSendTimes++;

                            nSent += nRet;
                            uiLeftBuffer -= nRet;
                            pcSentPos += nRet;
                        }
                        else
                        {
                            m_nLastWSAError = WSAGetLastError();

                            //如果接收到事件发送时也阻塞了，则等待一个周期后重试
                            if(m_nLastWSAError == WSAEWOULDBLOCK)
                            {
                                Sleep(1);        
                            }
                            else
                            {
                                //对于其他错误，直接退出
                                break;
                            }
                        }
                    }
                }
				else
				{
					//超时
					bIsTimeOut = true;
					break;
				}
            }
            else
            {
                //遇到阻塞之外的错误，直接退出
                m_bIsConnected = false;
                break;
            }
        }
        else
        {
            //发送成功，累加发送量，更新游标
            nSendTimes++;
            
            nSent += nRet;
            uiLeftBuffer -= nRet;
            pcSentPos += nRet;
        }
    }

    //如果发送完成
    if(nSent == uiBufferSize)
    {
        return SOB_RET_OK;
    }

	//如果超时
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

    //没能成功返回
    m_nLastWSAError = WSAGetLastError();
    return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// 发送UDP缓冲，必须在底层数据报范围内
//------------------------------------------------------------------------------
int CSocketBase::SendUDPBuffer(const char* pcIP, SHORT sPort, char* pBuffer, UINT uiBufferSize, USHORT nTimeOutSec)
{
	if(m_socket == NULL)
	{
		m_socket = CreateUDPSocket();
	}

	bool bIsTimeOut = false;

	//发送量计数
	int nSent = 0;

	//总发送次数，为发送次数定一个限额
	int nSendTimes = 0;
	int nSendLimitTimes = nTimeOutSec * 1000 / 100;						//如果遇到阻塞，等待100ms后重发
	UINT uiLeftBuffer = uiBufferSize;									//未发送完的缓存大小

	//转换远程地址
	SOCKADDR_IN addrRemote;
	memset(&addrRemote, 0, sizeof(addrRemote));

	addrRemote.sin_family = AF_INET;
	addrRemote.sin_addr.s_addr = inet_addr(pcIP);
	addrRemote.sin_port = htons(sPort);

	//发送游标
	char* pcSentPos = pBuffer;

	//直到所有的缓冲都发送完毕
	while(nSent < (int)uiBufferSize)
	{
		//检查发送次数是否超限
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
			//发送成功，累加发送量，更新游标
			nSendTimes++;

			nSent += nRet;
			uiLeftBuffer -= nRet;
			pcSentPos += nRet;
		}
	}

	//如果发送完成
	if(nSent == uiBufferSize)
	{
		return SOB_RET_OK;
	}

	//如果超时
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//没能成功返回
	m_nLastWSAError = WSAGetLastError();
	return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// 接收UDP缓冲
//------------------------------------------------------------------------------
int CSocketBase::RecvUDPMsg(char* pBuffer, UINT uiBufferSize, UINT& uiRecv, char* pcIP, USHORT& uPort, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	//发送前注册事件
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ);

	//远程信息
	SOCKADDR_IN addrRemote;
	int nAddrLen = sizeof(addrRemote);
	memset(&addrRemote, 0, nAddrLen);

	//尝试接收
	int nRet = recvfrom(m_socket, pBuffer, uiBufferSize, NULL, (PSOCKADDR)&addrRemote, &nAddrLen);

	if(nRet == SOCKET_ERROR)
	{
		m_nLastWSAError = WSAGetLastError();

		//遭遇阻塞
		if(m_nLastWSAError == WSAEWOULDBLOCK)
		{
			DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec*1000, FALSE);

			//如果网络事件发生
			WSANETWORKEVENTS wsaEvents;
			memset(&wsaEvents, 0, sizeof(wsaEvents));

			if(dwRet == WSA_WAIT_EVENT_0)
			{
				WSAResetEvent(m_SocketReadEvent);
				int nEnum = WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

				//如果接受可以进行并且没有错误发生
				if((wsaEvents.lNetworkEvents & FD_READ) &&
					(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
				{
					//再次接受文本
					nRet = recvfrom(m_socket, pBuffer, uiBufferSize, NULL, (PSOCKADDR)&addrRemote, &nAddrLen);

					if(nRet > 0)
					{
						//如果字节大于0，表明接收成功
						uiRecv = nRet;

						//更新IP和端口
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
		//第一次便接收成功
		uiRecv = nRet;

		//更新IP和端口
		strcpy(pcIP, inet_ntoa(addrRemote.sin_addr));
		uPort = ntohs(addrRemote.sin_port);

		return SOB_RET_OK;
	}

	//如果超时
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//如果上述接收失败
	m_nLastWSAError = WSAGetLastError();

	return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// 接受文本串，一次接受
//------------------------------------------------------------------------------
int CSocketBase::RecvOnce(char* pRecvBuffer, UINT uiBufferSize, UINT& uiRecv, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

    //检查连接状态
    if(!m_bIsConnected)
    {
        return SOB_RET_FAIL;
    }

    //发送前注册事件
    WSAResetEvent(m_SocketReadEvent);
    WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

    //尝试接收
    int nRet = recv(m_socket, pRecvBuffer, uiBufferSize, NULL);

    if(nRet == SOCKET_ERROR)
    {
        m_nLastWSAError = WSAGetLastError();

        //遭遇阻塞
        if(m_nLastWSAError == WSAEWOULDBLOCK)
        {
            DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec*1000, FALSE);

            //如果网络事件发生
            WSANETWORKEVENTS wsaEvents;
            memset(&wsaEvents, 0, sizeof(wsaEvents));

            if(dwRet == WSA_WAIT_EVENT_0)
            {
                WSAResetEvent(m_SocketReadEvent);
                int nEnum = WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

                //如果接受可以进行并且没有错误发生
                if((wsaEvents.lNetworkEvents & FD_READ) &&
                    (wsaEvents.iErrorCode[FD_READ_BIT] == 0))
                {
                    //再次接受文本
                    nRet = recv(m_socket, pRecvBuffer, uiBufferSize, NULL);

                    if(nRet > 0)
                    {
                        //如果发送字节大于0，表明发送成功
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
        //第一次便接收成功
        uiRecv = nRet;
        return SOB_RET_OK;
    }

	//如果超时
	if(bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

    //如果上述接收失败
    m_nLastWSAError = WSAGetLastError();
    return SOB_RET_FAIL;
}

//------------------------------------------------------------------------------
// 接收缓冲区，缓冲应该比待接受量要大一点才安全
//------------------------------------------------------------------------------
int CSocketBase::RecvBuffer(char* pRecvBuffer, UINT uiBufferSize, UINT& uiRecvSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	//检查连接状态
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	//接收前注册事件
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	//接收量计数
	int nReceived = 0;

	//总接收次数，为接收次数定一个限额
	int nRecvTimes = 0;
	int nRecvLimitTimes = (int)((float)uiRecvSize / 500 + 1.5);      //假定当前每次接收肯定不少于500字节

	//接收游标
	char* pcRecvPos = pRecvBuffer;

	//直到接收到足够指定量的缓冲
	while (nReceived < (int)uiRecvSize)
	{
		//检查接收次数是否超限
		if (nRecvTimes > nRecvLimitTimes)
		{
			break;
		}

		int nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			//遭遇阻塞
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

				//如果网络事件发生
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketReadEvent);
					WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

					//如果接收可以进行并且没有错误发生
					if ((wsaEvents.lNetworkEvents & FD_READ) &&
						(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
					{
						//再次接收
						nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

						if (nRet > 0)
						{
							//如果接收字节大于0，表明发送成功
							nRecvTimes++;

							nReceived += nRet;
							uiBufferSize -= nRet;
							pcRecvPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							//如果接收到事件发送时也阻塞了，则等待一个周期后重试
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								//对于其他错误，直接退出
								break;
							}
						}
					}
				}
				else
				{
					//超时
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				//遇到阻塞之外的错误，直接退出
				m_bIsConnected = false;
				break;
			}
		}
		else if (nRet == 0)		//对方主动关闭连接
		{
			m_bIsConnected = false;
			break;
		}
		else
		{
			//接收成功，累加接受量，更新游标
			nRecvTimes++;

			nReceived += nRet;
			uiBufferSize -= nRet;
			pcRecvPos += nRet;
		}
	}

	//如果接收完成
	if (nReceived == uiRecvSize)
	{
		return SOB_RET_OK;
	}

	//如果超时
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//没能成功返回
	m_nLastWSAError = WSAGetLastError();
	return SOB_RET_FAIL;
}
int CSocketBase::RecvHttpBuff(char** ppRecvBuffer, UINT& uiRecvSize, USHORT nTimeOutSec)
{
	bool bIsTimeOut = false;

	//检查连接状态
	if (!m_bIsConnected)
	{
		return SOB_RET_FAIL;
	}

	//接收前注册事件
	WSAResetEvent(m_SocketReadEvent);
	WSAEventSelect(m_socket, m_SocketReadEvent, FD_READ | FD_CLOSE);

	bool bFinished = false;

	// 实际接收字节数
	int nRecievedSize = 0;
	*ppRecvBuffer = (char*)je_malloc(RECV_REQUEST_BUFFSIZE_4K);	// 此处new的内存需外部释放（因为这里才能获取到大小，只能里面new，外面释放）
	UINT uiBufferSize = RECV_REQUEST_BUFFSIZE_4K;

	// 消息体要求大小
	int nActualBodySize = 0;
	int nRecvBodySize = 0;
	int nNeedSize = 0;

	//接收游标
	char* pcRecvPos = *ppRecvBuffer;

	//直到接收到足够指定量的缓冲
	while (bFinished == false)
	{
		//检查接收次数是否超限

		int nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

		if (nRet == SOCKET_ERROR)
		{
			m_nLastWSAError = WSAGetLastError();

			//遭遇阻塞
			if (m_nLastWSAError == WSAEWOULDBLOCK)
			{
				DWORD dwRet = WSAWaitForMultipleEvents(1, &m_SocketReadEvent, FALSE, nTimeOutSec * 1000, FALSE);

				//如果网络事件发生
				WSANETWORKEVENTS wsaEvents;
				memset(&wsaEvents, 0, sizeof(wsaEvents));

				if (dwRet == WSA_WAIT_EVENT_0)
				{
					WSAResetEvent(m_SocketReadEvent);
					WSAEnumNetworkEvents(m_socket, m_SocketReadEvent, &wsaEvents);

					//如果接收可以进行并且没有错误发生
					if ((wsaEvents.lNetworkEvents & FD_READ) &&
						(wsaEvents.iErrorCode[FD_READ_BIT] == 0))
					{
						//再次接收
						nRet = recv(m_socket, pcRecvPos, uiBufferSize, NULL);

						if (nRet > 0)
						{
							//如果接收字节大于0，表明发送成功
							nRecievedSize += nRet;
							uiBufferSize -= nRet;
							pcRecvPos += nRet;
						}
						else
						{
							m_nLastWSAError = WSAGetLastError();

							//如果接收到事件发送时也阻塞了，则等待一个周期后重试
							if (m_nLastWSAError == WSAEWOULDBLOCK)
							{
								Sleep(1);
							}
							else
							{
								//对于其他错误，直接退出
								break;
							}
						}
					}
				}
				else
				{
					//超时
					bIsTimeOut = true;
					break;
				}
			}
			else
			{
				//遇到阻塞之外的错误，直接退出
				m_bIsConnected = false;
				break;
			}
		}
		else if (nRet == 0)		//对方主动关闭连接
		{
			m_bIsConnected = false;
			break;
		}
		else
		{
			//接收成功，累加接受量，更新游标
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

	//如果接收完成
	if (bFinished)
	{
		uiRecvSize = nRecievedSize;
		return SOB_RET_OK;
	}

	//如果超时
	if (bIsTimeOut)
	{
		return SOB_RET_TIMEOUT;
	}

	//没能成功返回
	m_nLastWSAError = WSAGetLastError();
	return SOB_RET_FAIL;
}
//------------------------------------------------------------------------------
// 转换网址为IP
//------------------------------------------------------------------------------
bool CSocketBase::ResolveAddressToIp(const char* pcAddress, char* pcIp)
{
	addrinfo adiHints, *padiResult;
	int	nRet;

	memset(&adiHints, 0, sizeof(addrinfo));

	//仅解析IPV4的地址
	adiHints.ai_flags = AI_CANONNAME;
	adiHints.ai_family = AF_INET;
	adiHints.ai_socktype = SOCK_STREAM;
	adiHints.ai_protocol = IPPROTO_TCP;

	//转换地址
	nRet = ::getaddrinfo(pcAddress, NULL, &adiHints, &padiResult);
	
	//检查结果
	if(nRet != 0) 
	{
		freeaddrinfo(padiResult);
		return false;
	}
	
	//拷贝结果,只拷贝第一个
	if(padiResult->ai_addr != NULL)
	{
		::strcpy(pcIp, inet_ntoa(((sockaddr_in*)padiResult->ai_addr)->sin_addr));
	}
	
	freeaddrinfo(padiResult);
	return true;
}
//------------------------------------------------------------------------------
// 判断http请求是否接收完成
//------------------------------------------------------------------------------
bool CSocketBase::CheckHttpIsFinish(char* pBuff, int nSize, int &nNeedSize)
{
	bool bRet = false;

	//参数检查
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
		//分析完成，可以退出
		if (bIsHeaderEnd)
		{
			break;
		}

		//找到分行
		if (pBuff[i] == '\r' && pBuff[i + 1] == '\n')
		{
			//普通行检查
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

					//移除前方空格
					while (*pcStart == ' ')
					{
						pcStart++;
					}

					strncpy_s(szContLen, sizeof(szContLen), pcStart, _TRUNCATE);
				}

				nConnentLen = atoi(szContLen);
				nNeedSize = nConnentLen;
			}

			//还原
			pBuff[i] = '\r';
			nPos = i + 2;

			//是否到header结尾
			if (pBuff[i + 2] == '\r' && pBuff[i + 3] == '\n')
			{
				//如果header后有文本，则为post内容
				if (nSize >= nConnentLen + i + 4)	// 内容满足总大小
				{
					bRet = true;
				}
				//分析完header
				bIsHeaderEnd = true;
			}
		}
	}
	if (i >= (nLength - 4))
		bRet = true;

	return bRet;
}
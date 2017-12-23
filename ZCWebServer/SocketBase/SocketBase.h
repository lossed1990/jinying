//*****************************************************//
//                                                     //
//      创建人: wanyi                                  //
//      创建日期: 2010-11-05                           //
//                                                     //
//*****************************************************//

//==============================================================================
// 版本历史
// 2010-11-05 wanyi. TCP连接socket的基类
// 2011-11-25 wanyi. 增加对UDP与TCP监听的支持
// 2012-11-15 wanyi. 增加托管socket时对端信息的获取
// 2013-04-16 wanyi. 修正同时收发可能产生的问题
//==============================================================================

#ifndef SocketBaseH
#define SocketBaseH

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define _WINSOCKAPI_								//我们不需要旧的winsock.h
#define SOB_IP_LENGTH				128             //IP字符串长度
#define SOB_UDP_RECV_BUFFER			32*1024			//接收缓冲32K
#define SOB_DEFAULT_TIMEOUT_SEC		5000				//默认的超时时间

#define SOB_RET_OK					1				//正常
#define SOB_RET_FAIL				0				//错误
#define SOB_RET_TIMEOUT				-1				//超时

typedef unsigned (__stdcall *HANDLE_ACCEPT_THREAD)(void*);		//定义接受连接线程函数
typedef void(__stdcall *HANDLE_ACCEPT_CALLBACK)(SOCKADDR_IN* pRemoteAddr, SOCKET s, DWORD dwUser);		//定义接受连接线程函数


#define RECV_REQUEST_BUFFSIZE_4K	(1<<12)
#define RECV_REQUEST_BUFFSIZE_512K  (1<<19)
#define RECV_REQUEST_BUFFSIZE_1M	(1<<20)
#define RECV_REQUEST_BUFFSIZE_4M	(1<<22)
#define RECV_REQUEST_BUFFSIZE_16M	(1<<24)
#define RECV_REQUEST_BUFFSIZE_64M	(1<<26)
#define RECV_REQUEST_BUFFSIZE_128M	(1<<28)

class CSocketBase
{
public:
    CSocketBase();
    ~CSocketBase();

public:
	static void InitSocketLib();
	static void ReleaseSocketLib();

    void SetRecvTimeOut(UINT uiMSec);
    void SetSendTimeOut(UINT uiMsec);

    void SetRecvBufferSize(UINT uiByte);
    void SetSendBufferSize(UINT uiByte);

    bool ConnectRemote(const char* pcRemoteIP = NULL, SHORT sPort = 0, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
	bool BindOnPort(UINT uiPort);
	bool BindOnUDPPort(UINT uiPort);
	bool StartListenAndAccept(HANDLE_ACCEPT_THREAD pThreadFunc, HANDLE_ACCEPT_CALLBACK pCallback, DWORD dwUser, BOOL* pExitFlag = NULL, USHORT nLoopTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
    bool Reconnect();
    void Disconnect();

    int SendMsg(const char* pcMsg, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
    int SendBuffer(char* pBuffer, UINT uiBufferSize, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
	int SendUDPBuffer(const char* pcIP, SHORT sPort, char* pBuffer, UINT uiBufferSize, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
	int RecvUDPMsg(char* pBuffer, UINT uiBufferSize, UINT& uiRecv, char* pcIP, USHORT& uPort, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
    int RecvOnce(char* pRecvBuffer, UINT uiBufferSize, UINT& uiRecv, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
    int RecvBuffer(char* pRecvBuffer, UINT uiBufferSize, UINT& uiRecvSize, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);
	int RecvHttpBuff(char** ppRecvBuffer, UINT& uiRecvSize, USHORT nTimeOutSec = SOB_DEFAULT_TIMEOUT_SEC);

	static bool ResolveAddressToIp(const char* pcAddress, char* pcIp);
public:
    SOCKET getRawSocket();
	bool attachRawSocket(SOCKET s, bool bIsConnected);
	void detachSocket();
    const char* getRemoteIP();
	ULONG getRemoteULIP();
    SHORT getRemotePort();
    bool IsConnected();
	void Destroy();

    int m_nLastWSAError;

private:
    SOCKET CreateTCPSocket();
	SOCKET CreateUDPSocket();

	bool CheckHttpIsFinish(char* pBuff, int nSize, int &nNeedSize);
private:
    SOCKET m_socket;
    WSAEVENT m_SocketWriteEvent;
	WSAEVENT m_SocketReadEvent;

    bool m_bIsConnected;
    char m_pcRemoteIP[SOB_IP_LENGTH];
    SHORT m_sRemotePort;
};

#endif

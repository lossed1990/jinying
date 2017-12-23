//*****************************************************//
//                                                     //
//      ������: wanyi                                  //
//      ��������: 2010-11-05                           //
//                                                     //
//*****************************************************//

//==============================================================================
// �汾��ʷ
// 2010-11-05 wanyi. TCP����socket�Ļ���
// 2011-11-25 wanyi. ���Ӷ�UDP��TCP������֧��
// 2012-11-15 wanyi. �����й�socketʱ�Զ���Ϣ�Ļ�ȡ
// 2013-04-16 wanyi. ����ͬʱ�շ����ܲ���������
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

#define _WINSOCKAPI_								//���ǲ���Ҫ�ɵ�winsock.h
#define SOB_IP_LENGTH				128             //IP�ַ�������
#define SOB_UDP_RECV_BUFFER			32*1024			//���ջ���32K
#define SOB_DEFAULT_TIMEOUT_SEC		5000				//Ĭ�ϵĳ�ʱʱ��

#define SOB_RET_OK					1				//����
#define SOB_RET_FAIL				0				//����
#define SOB_RET_TIMEOUT				-1				//��ʱ

typedef unsigned (__stdcall *HANDLE_ACCEPT_THREAD)(void*);		//������������̺߳���
typedef void(__stdcall *HANDLE_ACCEPT_CALLBACK)(SOCKADDR_IN* pRemoteAddr, SOCKET s, DWORD dwUser);		//������������̺߳���


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

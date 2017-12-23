#include "TaskHttpClientRequest.h"
#include "SimpleHttpRequest.h"
#include "SimpleHttpResponse.h"
#include "StaticFileInfo.h"

#include "TaskHttpGetFileHandler.h"
#include "TaskHttpPostHandler.h"

CTaskHttpClientRequest::CTaskHttpClientRequest(CStaticHttpServer* pServer, CSocketBase* pSocket, SOCKADDR_IN* pRemoteAddr)
{
	m_pRecvReuqestBuff = NULL;
	m_psClientSocket = pSocket;

	m_pServer = pServer;

	if (pRemoteAddr)
	{
		memcpy(&m_addrClient, pRemoteAddr, sizeof(SOCKADDR_IN));
	}
}

CTaskHttpClientRequest::~CTaskHttpClientRequest()
{
	//���ٿͻ���socket
	if (m_psClientSocket)
	{
		m_psClientSocket->Disconnect();

		delete m_psClientSocket;
		m_psClientSocket = NULL;
	}
}

void CTaskHttpClientRequest::prepareTask()
{
	//׼��һ����ҳ
// 	m_pRecvReuqestBuff = je_malloc(RECV_REQUEST_BUFFSIZE_4K);
// 	memset(m_pRecvReuqestBuff, 0, RECV_REQUEST_BUFFSIZE_4K);
}

void CTaskHttpClientRequest::cleanupTask()
{
	//�����
	if (NULL != m_pRecvReuqestBuff)
	{
		je_free(m_pRecvReuqestBuff);
		m_pRecvReuqestBuff = NULL;
	}
}

void CTaskHttpClientRequest::executeTask()
{
	//�������
	if (NULL == m_psClientSocket)
	{
		return;
	}

	//����client��post��get������
	UINT uiRecved = 0;
	int nRet = m_psClientSocket->RecvHttpBuff((char**)&m_pRecvReuqestBuff, uiRecved);
		
	if (nRet != SOB_RET_OK)
	{
		return;
	}
	char* pcBuff = (char*)m_pRecvReuqestBuff;
	//���ַ���Ϊ����ֹ
	pcBuff[uiRecved] = '\0';

	//����header
	CSimpleHttpRequest sRequest(pcBuff);

	//�޷�ʶ���request
	if (sRequest.GetPaseStatus() != 0)
	{
		//Bad request
		CSimpleHttpResponse respBR(400);
		string strOutput = respBR.BuildResponseHeader();
		m_psClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//�Ƿ������ļ�
	if (sRequest.GetMethod().compare("GET") == 0)
	{
		HandleGET(sRequest);
	}
	else if (sRequest.GetMethod().compare("POST") == 0)
	{
		HandlePOST(sRequest);
	}
}

void CTaskHttpClientRequest::HandleGET(CSimpleHttpRequest& sReq)
{
	//����Ƿ��ǺϷ����ļ����󣬷��򷵻�forbiden
	string strURI = sReq.GetURI();

	if (!CStaticFileInfo::IsValideFilePath(strURI))
	{
		//forbiden
		CSimpleHttpResponse respBR(403);
		string strOutput = respBR.BuildResponseHeader();
		m_psClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//�ļ���ʽ�Ϸ��������ļ�ϵͳ����
	m_pServer->AddTaskToServerPool(new CTaskHttpGetFileHandler(m_pServer,
		m_psClientSocket, strURI, sReq.GetParam()));

	//socket����ʹ�ã��˴��������
	m_psClientSocket = NULL;
}

void CTaskHttpClientRequest::HandlePOST(CSimpleHttpRequest& sReq)
{
	//POST������Ծ����ļ�����Ҫ��������̬����
	char* pBuf = new char[sReq.GetRequestBodyLen()];
	sReq.GetRequestBodyBuf(pBuf, sReq.GetRequestBodyLen());
	m_pServer->AddTaskToServerPool(new CTaskHttpPostHandler(m_pServer, m_psClientSocket, sReq.GetURI(), pBuf, sReq.GetRequestBodyLen()));

	//socket����ʹ�ã��˴��������
	m_psClientSocket = NULL;
	delete pBuf;
}
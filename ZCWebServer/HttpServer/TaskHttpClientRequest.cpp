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
	//销毁客户端socket
	if (m_psClientSocket)
	{
		m_psClientSocket->Disconnect();

		delete m_psClientSocket;
		m_psClientSocket = NULL;
	}
}

void CTaskHttpClientRequest::prepareTask()
{
	//准备一个分页
// 	m_pRecvReuqestBuff = je_malloc(RECV_REQUEST_BUFFSIZE_4K);
// 	memset(m_pRecvReuqestBuff, 0, RECV_REQUEST_BUFFSIZE_4K);
}

void CTaskHttpClientRequest::cleanupTask()
{
	//清理堆
	if (NULL != m_pRecvReuqestBuff)
	{
		je_free(m_pRecvReuqestBuff);
		m_pRecvReuqestBuff = NULL;
	}
}

void CTaskHttpClientRequest::executeTask()
{
	//检查条件
	if (NULL == m_psClientSocket)
	{
		return;
	}

	//接收client的post，get等内容
	UINT uiRecved = 0;
	int nRet = m_psClientSocket->RecvHttpBuff((char**)&m_pRecvReuqestBuff, uiRecved);
		
	if (nRet != SOB_RET_OK)
	{
		return;
	}
	char* pcBuff = (char*)m_pRecvReuqestBuff;
	//置字符串为空终止
	pcBuff[uiRecved] = '\0';

	//分析header
	CSimpleHttpRequest sRequest(pcBuff);

	//无法识别的request
	if (sRequest.GetPaseStatus() != 0)
	{
		//Bad request
		CSimpleHttpResponse respBR(400);
		string strOutput = respBR.BuildResponseHeader();
		m_psClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//是否请求文件
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
	//检查是否是合法的文件请求，否则返回forbiden
	string strURI = sReq.GetURI();

	if (!CStaticFileInfo::IsValideFilePath(strURI))
	{
		//forbiden
		CSimpleHttpResponse respBR(403);
		string strOutput = respBR.BuildResponseHeader();
		m_psClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//文件格式合法，交由文件系统处理
	m_pServer->AddTaskToServerPool(new CTaskHttpGetFileHandler(m_pServer,
		m_psClientSocket, strURI, sReq.GetParam()));

	//socket继续使用，此处解除引用
	m_psClientSocket = NULL;
}

void CTaskHttpClientRequest::HandlePOST(CSimpleHttpRequest& sReq)
{
	//POST请求不针对具体文件，需要服务器动态处理
	char* pBuf = new char[sReq.GetRequestBodyLen()];
	sReq.GetRequestBodyBuf(pBuf, sReq.GetRequestBodyLen());
	m_pServer->AddTaskToServerPool(new CTaskHttpPostHandler(m_pServer, m_psClientSocket, sReq.GetURI(), pBuf, sReq.GetRequestBodyLen()));

	//socket继续使用，此处解除引用
	m_psClientSocket = NULL;
	delete pBuf;
}
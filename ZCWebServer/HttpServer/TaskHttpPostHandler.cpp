#include "TaskHttpPostHandler.h"

#include "SimpleHttpResponse.h"
#include "TaskPostConfigVerify.h"
#include "../JsonRequest/EncodingToolsClass.h"

CTaskHttpPostHandler::CTaskHttpPostHandler(CStaticHttpServer* pServer, CSocketBase* pClientSocket,
	string& strURI, char* pReqBody, int nBodyLen)
{
	m_pServer = pServer;
	m_pClientSocket = pClientSocket;

	m_strURI = strURI;
	
	m_pReqBodyBuf = (char*)je_malloc(nBodyLen);
	memcpy(m_pReqBodyBuf, pReqBody, nBodyLen);
	m_nReqBodyLen = nBodyLen;
}


CTaskHttpPostHandler::~CTaskHttpPostHandler()
{
	//�Ͽ�ͬ�ͻ�������
	if (m_pClientSocket)
	{
		m_pClientSocket->Disconnect();

		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
	if (m_pReqBodyBuf)
	{
		je_free(m_pReqBodyBuf);
		m_pReqBodyBuf = NULL;
	}
}

void CTaskHttpPostHandler::prepareTask()
{
}

void CTaskHttpPostHandler::executeTask()
{
	//����URI
	size_t tPos = m_strURI.find_last_of('/');

	if (tPos == string::npos)
	{
		return;
	}

	m_strURI = m_strURI.substr(tPos);

	//����Ǹ�Ŀ¼�������
	if (m_strURI.length() < 2)
	{
		return;
	}

	//���postû�����ݣ�Ҳ���Ե�
	//if (/*m_pReqBodyBuf.length() < 1*/m_nReqBodyLen <= 0)
	//{
	//	return;
	//}
	
	//�ҵ���Ӧ�Ĵ�������
	CServerTask* pTask = createPostHandlerTask(m_pServer, m_pClientSocket, m_strURI, string(m_pReqBodyBuf));
	if (pTask)
	{
		m_pServer->AddTaskToServerPool(pTask);

		//socket���ڼ���ͨ��
		m_pClientSocket = NULL;
		return;
	}

	//ִ�нӿ�
	string strReturn;
	m_pServer->ExecuteInterface(m_strURI, m_pReqBodyBuf, m_nReqBodyLen, strReturn);

	//ת��������Է�json����
	int nLen = strlen(strReturn.c_str());
	char* pcUTF = new char[nLen*2];
	memset(pcUTF,0,nLen*2);
	CEncodingTools::ConvertGBToUTF8(strReturn.c_str(), pcUTF, nLen*2);

	//����json����
	CSimpleHttpResponse respBR(200);
	respBR.SetContentLength(strlen(pcUTF));
	string chExt = "json";
	respBR.SetContentType(respBR.GetContentTypeFromExtensionLowCase(chExt));

	string strOutput = respBR.BuildResponseHeader();
	m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());
	m_pClientSocket->SendBuffer(pcUTF, strlen(pcUTF));
	 
	delete[] pcUTF;
	pcUTF = NULL;
}

void CTaskHttpPostHandler::cleanupTask()
{

}

//������Ӧ�Ĵ������
CServerTask* CTaskHttpPostHandler::createPostHandlerTask(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strURI, string& strReqBody)
{
	CServerTask* pTask = NULL;

	//�����û���֤
	if (strURI == "/configverify.json")
	{
		pTask = new CTaskPostConfigVerify(pServer, pClientSocket, strReqBody);
	}
	

	return pTask;
}
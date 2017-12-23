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
	//断开同客户的连接
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
	//修正URI
	size_t tPos = m_strURI.find_last_of('/');

	if (tPos == string::npos)
	{
		return;
	}

	m_strURI = m_strURI.substr(tPos);

	//如果是根目录，则忽略
	if (m_strURI.length() < 2)
	{
		return;
	}

	//如果post没有内容，也忽略掉
	//if (/*m_pReqBodyBuf.length() < 1*/m_nReqBodyLen <= 0)
	//{
	//	return;
	//}
	
	//找到对应的处理任务
	CServerTask* pTask = createPostHandlerTask(m_pServer, m_pClientSocket, m_strURI, string(m_pReqBodyBuf));
	if (pTask)
	{
		m_pServer->AddTaskToServerPool(pTask);

		//socket用于继续通信
		m_pClientSocket = NULL;
		return;
	}

	//执行接口
	string strReturn;
	m_pServer->ExecuteInterface(m_strURI, m_pReqBodyBuf, m_nReqBodyLen, strReturn);

	//转化结果，以防json乱码
	int nLen = strlen(strReturn.c_str());
	char* pcUTF = new char[nLen*2];
	memset(pcUTF,0,nLen*2);
	CEncodingTools::ConvertGBToUTF8(strReturn.c_str(), pcUTF, nLen*2);

	//返回json数据
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

//创建对应的处理对象
CServerTask* CTaskHttpPostHandler::createPostHandlerTask(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strURI, string& strReqBody)
{
	CServerTask* pTask = NULL;

	//配置用户认证
	if (strURI == "/configverify.json")
	{
		pTask = new CTaskPostConfigVerify(pServer, pClientSocket, strReqBody);
	}
	

	return pTask;
}
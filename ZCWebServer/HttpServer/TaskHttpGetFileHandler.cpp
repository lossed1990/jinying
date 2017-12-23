#include "TaskHttpGetFileHandler.h"
#include "SimpleHttpResponse.h"
#include "string.h"
#define HTTP_RESPONSE_PERREAD	16*1024		//每次发送的字节数量

CTaskHttpGetFileHandler::CTaskHttpGetFileHandler(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strURI, string &strParam) 
	: m_fileInfo(strURI, strParam)
{
	m_pServer = pServer;
	m_pClientSocket = pClientSocket;
}


CTaskHttpGetFileHandler::~CTaskHttpGetFileHandler()
{
	//断开同客户的连接
	if (m_pClientSocket)
	{
		m_pClientSocket->Disconnect();

		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}

void CTaskHttpGetFileHandler::prepareTask()
{

}

void CTaskHttpGetFileHandler::executeTask()
{
	bool bRet = m_pServer->GetFileInfoByURI(m_fileInfo);

	//如果文件不存在，则返回404
	int nNamePos = m_fileInfo.GetFileFullPath().rfind("\\") + 1;
	if (!bRet || nNamePos <= 0)
	{
		//not found
		CSimpleHttpResponse respBR(404);
		string strOutput = respBR.BuildResponseHeader();
		m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//准备内存，	
	void* pBuff = je_malloc(HTTP_RESPONSE_PERREAD);

	//没有足够的内存
	if (NULL == pBuff)
	{
		//server error
		CSimpleHttpResponse respBR(500);
		string strOutput = respBR.BuildResponseHeader();
		m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//准备文件
	string strFileName = "attachment; filename=";	// 设置目标保存文件名
	strFileName += m_fileInfo.GetFileFullPath().substr(nNamePos);

	void* pFile = m_pServer->PrepareFile(m_fileInfo);
	CSimpleHttpResponse respOK(200);
	respOK.SetContentLength(m_fileInfo.GetFileSize());
	if (m_fileInfo.GetParam().length() > 0)
		respOK.SetContentDisposition(strFileName);	// 有参数则为下载模式，增加ContentDisposition字段
	respOK.SetContentType(respOK.GetContentTypeFromExtensionLowCase(m_fileInfo.GetFileExtensionLowcase()));
	
	//准备resp header
	string strRespHeader = respOK.BuildResponseHeader();

	//发送resp
	m_pClientSocket->SendBuffer((char*)strRespHeader.c_str(), strRespHeader.length());

	size_t szReaded = 0;
	int nHaveLeft = 1;
	while (nHaveLeft > 0)
	{
		nHaveLeft = m_pServer->ReadFileOnce(m_fileInfo, pFile, pBuff, HTTP_RESPONSE_PERREAD, szReaded);
		m_pClientSocket->SendBuffer((char*)pBuff, szReaded);
	}

	//发送完毕，清理文件
	m_pServer->ReleaseFile(m_fileInfo, pFile);

	//清理buffer
	je_free(pBuff);
	pBuff = NULL;
}

void CTaskHttpGetFileHandler::cleanupTask()
{

}

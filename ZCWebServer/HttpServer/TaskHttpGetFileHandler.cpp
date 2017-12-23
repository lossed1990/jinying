#include "TaskHttpGetFileHandler.h"
#include "SimpleHttpResponse.h"
#include "string.h"
#define HTTP_RESPONSE_PERREAD	16*1024		//ÿ�η��͵��ֽ�����

CTaskHttpGetFileHandler::CTaskHttpGetFileHandler(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strURI, string &strParam) 
	: m_fileInfo(strURI, strParam)
{
	m_pServer = pServer;
	m_pClientSocket = pClientSocket;
}


CTaskHttpGetFileHandler::~CTaskHttpGetFileHandler()
{
	//�Ͽ�ͬ�ͻ�������
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

	//����ļ������ڣ��򷵻�404
	int nNamePos = m_fileInfo.GetFileFullPath().rfind("\\") + 1;
	if (!bRet || nNamePos <= 0)
	{
		//not found
		CSimpleHttpResponse respBR(404);
		string strOutput = respBR.BuildResponseHeader();
		m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//׼���ڴ棬	
	void* pBuff = je_malloc(HTTP_RESPONSE_PERREAD);

	//û���㹻���ڴ�
	if (NULL == pBuff)
	{
		//server error
		CSimpleHttpResponse respBR(500);
		string strOutput = respBR.BuildResponseHeader();
		m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());

		return;
	}

	//׼���ļ�
	string strFileName = "attachment; filename=";	// ����Ŀ�걣���ļ���
	strFileName += m_fileInfo.GetFileFullPath().substr(nNamePos);

	void* pFile = m_pServer->PrepareFile(m_fileInfo);
	CSimpleHttpResponse respOK(200);
	respOK.SetContentLength(m_fileInfo.GetFileSize());
	if (m_fileInfo.GetParam().length() > 0)
		respOK.SetContentDisposition(strFileName);	// �в�����Ϊ����ģʽ������ContentDisposition�ֶ�
	respOK.SetContentType(respOK.GetContentTypeFromExtensionLowCase(m_fileInfo.GetFileExtensionLowcase()));
	
	//׼��resp header
	string strRespHeader = respOK.BuildResponseHeader();

	//����resp
	m_pClientSocket->SendBuffer((char*)strRespHeader.c_str(), strRespHeader.length());

	size_t szReaded = 0;
	int nHaveLeft = 1;
	while (nHaveLeft > 0)
	{
		nHaveLeft = m_pServer->ReadFileOnce(m_fileInfo, pFile, pBuff, HTTP_RESPONSE_PERREAD, szReaded);
		m_pClientSocket->SendBuffer((char*)pBuff, szReaded);
	}

	//������ϣ������ļ�
	m_pServer->ReleaseFile(m_fileInfo, pFile);

	//����buffer
	je_free(pBuff);
	pBuff = NULL;
}

void CTaskHttpGetFileHandler::cleanupTask()
{

}

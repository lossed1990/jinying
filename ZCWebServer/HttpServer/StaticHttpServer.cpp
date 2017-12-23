#include "StaticHttpServer.h"
#include "TaskHttpClientRequest.h"

#include "../ToolFuncs/ToolFuncs.h"
#include "../JsonRequest/EncodingToolsClass.h"

#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
using namespace rapidjson;

CStaticHttpServer::CStaticHttpServer()
{
	CSocketBase::InitSocketLib();

	//���������̳߳�
	m_poolServer.InitThreadPool(8);
}


CStaticHttpServer::~CStaticHttpServer()
{
	//ֹͣ�̳߳�
	m_poolServer.MarkPoolShouldEnd();

	CSocketBase::ReleaseSocketLib();
}

void __stdcall CStaticHttpServer::AcceptConnectCallback(SOCKADDR_IN* pRemoteAddr, SOCKET s, DWORD dwUser)
{
	CStaticHttpServer* pThis = (CStaticHttpServer*)dwUser;

	//������ӦSocket
	CSocketBase* pSocket = new CSocketBase();
	pSocket->attachRawSocket(s, true);

	//�����̳߳ش���
	pThis->AddTaskToServerPool(new CTaskHttpClientRequest(pThis, pSocket, pRemoteAddr));
}

int CStaticHttpServer::StartHttpServer(int nListenPort)
{
	int nRet = m_netServer.StartListenThread(nListenPort, AcceptConnectCallback, (DWORD)this);

	//�˿ڰ�ʧ�ܣ����ܱ�ռ��
	if (nRet != 0)
	{
		return -1;
	}

	return 0;
}

int CStaticHttpServer::StopHttpServer()
{
	//ֹͣ�����߳�
	m_netServer.StopServer();

	//�̳߳ش˴���ͣ�ã������������ظ��ɶԵ���start��stop
	return 0;
}

int CStaticHttpServer::UpdateConfigString(string& strConfig)
{
	//����ļ�
	Document doc;
	doc.Parse(strConfig.c_str());

	if (doc.IsObject())
	{
		StringBuffer outBuff;
		//PrettyWriter<StringBuffer> jsonWriter(outBuff); //��ʽ��д��
		Writer<StringBuffer> jsonWriter(outBuff);   //ѹ��д��
		doc.Accept(jsonWriter);

		//ֱ��д���ļ�
		TCHAR tpcPath[260] = { 0 };
		GetExeFileSubDirectory(tpcPath, 260, "html");

		//config file
		strcat(tpcPath, "\\config.json");

		FILE* pFile = fopen(tpcPath, "wb");
		if (pFile)
		{
			fwrite(outBuff.GetString(), 1, outBuff.GetSize(), pFile);

			fclose(pFile);
			pFile = NULL;

			return 0;
		}
	}

	return -1;
}

int CStaticHttpServer::UpdateAppsString(string& strConfig)
{
	//����ļ�
	Document doc;
	doc.Parse(strConfig.c_str());

	if (doc.IsObject())
	{
		StringBuffer outBuff;
		Writer<StringBuffer> jsonWriter(outBuff);
		doc.Accept(jsonWriter);

		//ֱ��д���ļ�
		TCHAR tpcPath[260] = { 0 };
		GetExeFileSubDirectory(tpcPath, 260, "html");

		//config file
		strcat(tpcPath, "\\apps.json");

		FILE* pFile = fopen(tpcPath, "wb");
		if (pFile)
		{
			fwrite(outBuff.GetString(), 1, outBuff.GetSize(), pFile);

			fclose(pFile);
			pFile = NULL;

			return 0;
		}
	}

	return -1;
}

string CStaticHttpServer::GetConfigBufferString()
{
	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");

	//config file
	strcat(tpcPath, "\\config.json");

	string strContent;

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//�����ļ����ᳬ��4K
		char pcBuff[4096] = { 0 };
		fread(pcBuff, 1, 4096, pFile);

		strContent = pcBuff;
		fclose(pFile);
		pFile = NULL;
	}

	return strContent;
}

string CStaticHttpServer::GetAppsBufferString()
{
	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");

	//config file
	strcat(tpcPath, "\\apps.json");

	string strContent;

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//�����ļ����ᳬ��4K
		char pcBuff[4096] = { 0 };
		fread(pcBuff, 1, 4096, pFile);

		strContent = pcBuff;
		fclose(pFile);
		pFile = NULL;
	}

	return strContent;
}

string CStaticHttpServer::GetConfigPass()
{
	//parse file
	Document doc;
	doc.Parse(GetConfigBufferString().c_str());

	if (doc.IsObject())
	{
		return doc["pass"].GetString();
	}

	//���δ���ã����ؿ�����
	return string("");
}

bool CStaticHttpServer::GetConfigOthers(string& strEleIP, int& nPort, string& strServerName, 
											string& strTypename)
{
	//parse file
	Document doc;
	doc.Parse(GetConfigBufferString().c_str());

	if (doc.IsObject())
	{
		strEleIP = doc["elastic_server"].GetString();
		nPort = doc["elastic_port"].GetInt();
		strServerName = doc["servername"].GetString();
		strTypename = doc["typename"].GetString();

		return true;
	}

	return false;
}

int CStaticHttpServer::GetConfigHttpPort()
{
	Document doc;
	doc.Parse(GetConfigBufferString().c_str());

	if (doc.IsObject())
	{
		return doc["httpport"].GetInt();
	}

	//Ĭ�Ϸ���80�˿�
	return 80;
}

bool CStaticHttpServer::GetAppsStrings(vector<wstring>& strVecApps)
{
	//parse file
	Document doc;
	doc.Parse(GetAppsBufferString().c_str());

	if (doc.IsObject())
	{
		if (doc["apps"].IsArray())
		{
			for (SizeType i = 0; i < doc["apps"].Size(); i++)
			{
				WCHAR pcTemp[256] = { 0 };
				CEncodingTools::ConvertUTF8ToUNICODE(doc["apps"][i].GetString(), pcTemp, 256);
				strVecApps.push_back(wstring(pcTemp));
			}

			return true;
		}
	}

	return false;
}

void CStaticHttpServer::AddTaskToServerPool(CServerTask* pTask)
{
	if (pTask)
	{
		m_poolServer.AddTaskToPool(pTask);
	}
}

bool CStaticHttpServer::GetFileInfoByURI(CStaticFileInfo& strURIFile)
{
	if (strURIFile.IsFileInMem())
	{
		return m_memFiles.GetFileInfoByPath(strURIFile);
	}

	return m_diskFiles.GetFileInfoByPath(strURIFile);
}

void* CStaticHttpServer::PrepareFile(CStaticFileInfo& fileInfo)
{
	if (fileInfo.IsFileInMem())
	{
		return m_memFiles.PrepareFile(fileInfo);
	}

	return m_diskFiles.PrepareFile(fileInfo);
}

int CStaticHttpServer::ReadFileOnce(CStaticFileInfo& fileInfo, void* pSourcePtr, void* pRecvBuff, size_t szRecvBuffSize, size_t& szReaded)
{
	if (fileInfo.IsFileInMem())
	{
		return m_memFiles.ReadFileOnce(fileInfo, pSourcePtr, pRecvBuff, szRecvBuffSize, szReaded);
	}

	return m_diskFiles.ReadFileOnce(fileInfo, pSourcePtr, pRecvBuff, szRecvBuffSize, szReaded);
}

void CStaticHttpServer::ReleaseFile(CStaticFileInfo& fileInfo, void* pSourcePtr)
{
	if (fileInfo.IsFileInMem())
	{
		m_memFiles.ReleaseFile(fileInfo, pSourcePtr);
		return;
	}

	m_diskFiles.ReleaseFile(fileInfo, pSourcePtr);
}

void CStaticHttpServer::ExecuteInterface(string& strURI, char* pReqBody, int nReqBodyLen, string& strReturn)
{
	m_interfaceManger.ExecuteInterface(strURI, pReqBody, nReqBodyLen, strReturn);
}
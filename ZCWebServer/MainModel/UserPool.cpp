#include "UserPool.h"
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"
using namespace rapidjson;

TUserPool::TUserPool()
{
	m_csList.Enter();
	LoadUserFromConfig();
	m_csList.Leave();
}

TUserPool::~TUserPool()
{
	m_csList.Enter();
	for (auto& item : m_vecUserList)
	{
		delete item;
		item = nullptr;
	}
	m_vecUserList.clear();
	m_csList.Leave();
}

bool TUserPool::CheckUserPassword(string& strUserName, string& strUserPw)
{
	m_csList.Enter();
	for (auto& item : m_vecUserList)
	{
		if (item->GetName() == strUserName)
		{
			bool bRet = item->CheckPassword(strUserPw);
			m_csList.Leave();
			return bRet;
		}
	}
	m_csList.Leave();
	return false;
}

int TUserPool::AddUser(string& strUserName, string& strUserPw, int nType)
{
	m_csList.Enter();
	for (auto& item : m_vecUserList)
	{
		if (item->GetName() == strUserName)
		{
			m_csList.Leave();
			return -1;  //�����û�ʧ�ܣ��û����Ѵ���
		}
	}

	TUserEntity* pNewEntity = new TUserEntity(strUserName, strUserPw, nType);
	m_vecUserList.push_back(pNewEntity);
	SaveUserToConfig();
	m_csList.Leave();
	return 0;
}

int TUserPool::DeleteUser(string& strUserName)
{
	m_csList.Enter();
	for (auto iter = m_vecUserList.begin(); iter != m_vecUserList.end(); ++iter)
	{
		if ((*iter)->GetName() == strUserName)
		{
			delete (*iter);
			*iter = nullptr;
			m_vecUserList.erase(iter);
			SaveUserToConfig();
			m_csList.Leave();
			return 0;
		}
	}

	m_csList.Leave();
	return -1;  //ɾ���û�ʧ�ܣ��û���������
}

int TUserPool::ChangeUserPassword(string& strUserName, string& strUserOldPw, string& strUserNewPw)
{
	m_csList.Enter();
	for (auto& item : m_vecUserList)
	{
		if (item->GetName() == strUserName)
		{
			bool bRet = item->ChangePassword(strUserOldPw, strUserNewPw);
			if (!bRet)
			{
				m_csList.Leave();
				return -2; //ԭʼ�������
			}

			SaveUserToConfig();
			m_csList.Leave();
			return 0;
		}
	}

	m_csList.Leave();
	return -1;  //�޸�����ʧ�ܣ��û���������
}

int TUserPool::GetUserTypeByName(string& strUserName)
{
	m_csList.Enter();
	for (auto& item : m_vecUserList)
	{
		if (item->GetName() == strUserName)
		{
			int nType = item->GetType();
			m_csList.Leave();
			return nType;
		}
	}
	m_csList.Leave();
	return 0;
}

void TUserPool::EachUsers(function<void(TUserEntity*)> pCallBack)
{
	m_csList.Enter();
	for (auto& item : m_vecUserList)
	{
		pCallBack(item);
	}
	m_csList.Leave();
}

int TUserPool::GetUserCount()
{
	m_csList.Enter();
	int nCount = m_vecUserList.size();
	m_csList.Leave();
	return nCount;
}

bool TUserPool::LoadUserFromConfig()
{
	m_vecUserList.clear();

	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "html");
	strcat(tpcPath, "\\user.json");

	FILE* pFile = fopen(tpcPath, "rb");
	if (pFile)
	{
		//�����ļ����ᳬ��8K
		char pcBuff[8192] = { 0 };
		fread(pcBuff, 1, 8192, pFile);

		Document doc;
		doc.Parse(pcBuff);

		if (doc.IsObject())
		{
			if (doc["users"].IsArray())
			{
				for (SizeType i = 0; i < doc["users"].Size(); i++)
				{
					//char pcTemp[256] = { 0 };
					//CEncodingTools::ConvertUTF8ToGB(doc["users"][i]["name"].GetString(), pcTemp, 256);
					//string strName(pcTemp);
					string strName = doc["users"][i]["name"].GetString();

					//CEncodingTools::ConvertUTF8ToGB(doc["users"][i]["pw"].GetString(), pcTemp, 256);
					//string strPw(pcTemp);
					string strPw = doc["users"][i]["pw"].GetString();

					int nType = doc["users"][i]["type"].GetInt();

					TUserEntity* pNewEntity = new TUserEntity(strName, strPw, nType);
					m_vecUserList.push_back(pNewEntity);
				}
			}
		}

		fclose(pFile);
		pFile = NULL;
		return true;
	}

	return false;
}

int TUserPool::SaveUserToConfig()
{
	//����json��
	string strReturn = "{\"users\":[";
	for (int i = 0; i != m_vecUserList.size(); ++i)
	{
		char pcSubTemp[256] = { 0 };
		const char* SUBTEMPLATE = "{\"name\":\"%s\",\"pw\":\"%s\",\"type\":%d}";
		sprintf_s(pcSubTemp, 256, SUBTEMPLATE, m_vecUserList[i]->GetName().c_str(), m_vecUserList[i]->GetPassword().c_str(), m_vecUserList[i]->GetType());
		strReturn = strReturn + pcSubTemp;
		if (i != (m_vecUserList.size() - 1))
		{
			strReturn = strReturn + ",";
		}
	}
	strReturn = strReturn + "]}";

	//����ļ�
	Document doc;
	doc.Parse(strReturn.c_str());
	if (doc.IsObject())
	{
		StringBuffer outBuff;
		//PrettyWriter<StringBuffer> jsonWriter(outBuff);  //��ʽ��д��
		Writer<StringBuffer> jsonWriter(outBuff);          //ѹ��д��
		doc.Accept(jsonWriter);

		//ֱ��д���ļ�
		TCHAR tpcPath[260] = { 0 };
		GetExeFileSubDirectory(tpcPath, 260, "html");

		strcat(tpcPath, "\\user.json");

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


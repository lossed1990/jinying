#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "../JsonRequest/EncodingToolsClass.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "CommonFunction.h"
#include "InterfaceGetPathList.h"

CInterfaceGetPathList::CInterfaceGetPathList()
{
	//����ʱ��ȡ��װ·�����ύ�ӿڷ������ٶ�
	//todo:�����պ�����ڶ�ʱˢ�°�װ·����Ŀǰ����������ʱ��ȡһ��
	RefreshZoneeForlderPath();
}


CInterfaceGetPathList::~CInterfaceGetPathList()
{
}

string CInterfaceGetPathList::GetUrl()
{
	return I_BASE_GET_PATH;
}

void CInterfaceGetPathList::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	if (m_vecPaths.size() > 0)
	{
		tDoc.AddMember("ok", 0, allocator);
		tDoc.AddMember("errorinfo", "", allocator);

		Value array(kArrayType);
		for (int i = 0; i < m_vecPaths.size(); i++)
		{
			Value object(kObjectType);
			Value vPath(kStringType);
			int nSize = m_vecPaths[i].size();
			vPath.SetString(m_vecPaths[i].c_str(), m_vecPaths[i].size(), allocator);
			object.AddMember("path", vPath, allocator);
			array.PushBack(object, allocator);
		}

		tDoc.AddMember("data", array, allocator);
	}
	else
	{
		tDoc.AddMember("ok", 1, allocator);
		tDoc.AddMember("errorinfo", "û���ҵ��ͻ��˰�װ·��", allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
}

void CInterfaceGetPathList::RefreshZoneeForlderPath()
{
	//��ȡע���װ·��
	string strRegPath;
	CCommonFunction::GetRegString(HKEY_LOCAL_MACHINE, "Software\\TYProtech\\ZONEE\\Client\\Settings", strRegPath);
	if (strRegPath.length() > 0)
	{
		m_vecPaths.push_back(strRegPath);
	}

	//������װ·��
	vector<string> vecPathTmp;
	GetZoneeForlderPath(vecPathTmp);

	for (auto iter : vecPathTmp)
	{
		if (iter != strRegPath)
		{
			m_vecPaths.push_back(iter);
		}
	}
}

void CInterfaceGetPathList::GetZoneeForlderPath(vector<string> &vecForder)
{
	TCHAR buf[100] = { 0 };
	DWORD len = GetLogicalDriveStrings(sizeof(buf) / sizeof(TCHAR), buf);
	string strDisks;
	TCHAR* s = buf;
	for (; *s; s += _tcslen(s) + 1)
	{
		vector<string> vecTmp1;
		vector<string> vecTmp2;
		CCommonFunction::FindFolder(s, "TYProtech", vecTmp1);
		for (int i = 0; i < vecTmp1.size(); i++)
		{
			CCommonFunction::FindFolder(vecTmp1[i], "ZONEE", vecTmp2);
			for (int i = 0; i < vecTmp2.size(); i++)
			{
				CCommonFunction::FindFolder(vecTmp2[i], "Client", vecForder);
			}
		}
	}
}


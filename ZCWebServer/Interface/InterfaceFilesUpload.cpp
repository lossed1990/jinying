#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "../ToolFuncs/UrlTransCode.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "InterfaceFilesUpload.h"

HANDLE g_hBoundaryLock = CreateEvent(NULL, TRUE, TRUE, NULL);
vector<string> g_vecBoundary;

CInterfaceFilesUpload::CInterfaceFilesUpload()
{
}

CInterfaceFilesUpload::~CInterfaceFilesUpload()
{
}

string CInterfaceFilesUpload::GetUrl()
{
	return I_FILE_UPLOAD_FILE;
}

void CInterfaceFilesUpload::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	//while ()
	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();

	string strBoundary;
	WaitForSingleObject(g_hBoundaryLock, INFINITE);
	vector<string>::iterator iter = g_vecBoundary.begin();
	for (; iter != g_vecBoundary.end(); iter++)
	{
		if (strstr(pReqBody, (*iter).c_str()))
		{
			strBoundary = (*iter);
			g_vecBoundary.erase(iter);
			break;
		}
	}
	SetEvent(g_hBoundaryLock);

	if (strBoundary.length() > 0 && pReqBody != NULL && nReqBodyLen > 0)
	{
		int nRet = GetUploadFileInfo(pReqBody, nReqBodyLen, strBoundary);
		if (nRet == 1)
		{
			tDoc.AddMember("ok", 1, allocator);
			tDoc.AddMember("errorinfo", "文件创建失败 或 正在使用中", allocator);
		}
		else
		{
			tDoc.AddMember("ok", 0, allocator);
			tDoc.AddMember("errorinfo", "", allocator);
		}
	}
	else
	{
		tDoc.AddMember("ok", 1, allocator);
		tDoc.AddMember("errorinfo", "上传文件失败", allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
}

int CInterfaceFilesUpload::GetUploadFileInfo(char* pBuff, int nSize, string strBoundary)
{
	int nRet = 0;
	vector<char*> vecBoundary;
	GetAllBoundary(pBuff, nSize, strBoundary, vecBoundary);
	char* pData = NULL;
	string strFileName;
	string strFilePath;
	char* pFileData = NULL;
	int nFileDataSize = 0;
	for (int nBoundaryIndex = 0; nBoundaryIndex < vecBoundary.size(); nBoundaryIndex++)
	{
		int nHeadEndPos = 0;
		pData = vecBoundary[nBoundaryIndex] + strBoundary.length() + 2;	// 加2，因为分隔符后面会跟一组 \r\n

		// 先将boundary后的head分离开
		while (nHeadEndPos <= strlen(pData) - 4)
		{
			if (pData[nHeadEndPos] == '\r' && pData[nHeadEndPos + 1] == '\n' &&
				pData[nHeadEndPos + 2] == '\r' && pData[nHeadEndPos + 3] == '\n')
				pData[nHeadEndPos] = '\0';		// 与后面分离
			else
			{
				nHeadEndPos++;
				continue;
			}
		}

		if (strstr(pData, "name=\"upfile\""))	// 找到上传的文件
		{
			char* pFileName = strstr(pData, "; filename=\"");
			if (pFileName) pFileName += strlen("; filename=\"");
			for (int i = 0; i < strlen(pFileName); i++)
			{
				if (pFileName[i] == '\"')
				{
					pFileName[i] = '\0';
					CUrlTransCode cCode;
					strFileName = cCode.UrlUTF8Decode(pFileName);
					pFileName[i] = '\"';
				}
			}
			pData[nHeadEndPos] = '\r';		// 还原

			pFileData = GetBodyData(pData, nSize - (pData - pBuff), strBoundary, nFileDataSize);
		}
		else if (strstr(pData, "name=\"path\""))	// 找到上传的文件
		{
			pData[nHeadEndPos] = '\r';		// 还原
			int nBodySize = 0;
			char* pBody = GetBodyData(pData, nSize - (pData - pBuff), strBoundary, nBodySize);
			if (pBody)
			{
				char* pPath = new char[nBodySize + 1];
				memset(pPath, 0, nBodySize + 1);
				memcpy(pPath, pBody, nBodySize);
				strFilePath = pPath;
				if (pPath[strlen(pPath) - 1] != '\\')
				{
					strFilePath += "\\";
				}
			}
		}
		else
		{
			pData[nHeadEndPos] = '\r';		// 还原
		}
	}
	if (pFileData != NULL && nFileDataSize > 0 && strFileName.length() > 0)
	{
		string strFileFullName = strFilePath + strFileName;
		FILE *pFile = fopen(strFileFullName.c_str(), "wb");
		if (pFile)
		{
			fwrite(pFileData, nFileDataSize, 1, pFile);
			fclose(pFile);
		}
		else
		{
			nRet = 1;	// 文件正在使用
		}
	}
	return nRet;
}
void CInterfaceFilesUpload::GetAllBoundary(char* pBuff, int nSize, string strBoundary, vector<char*> &vecBoundary)
{
	char* pPos = NULL;
	int i = 0;
	while (i < nSize - 4)
	{
		pPos = strstr(pBuff + i, strBoundary.c_str());
		if (pPos && pPos[strBoundary.length()] == '\r')
		{
			vecBoundary.push_back(pPos);
			i = pPos - pBuff + strBoundary.length();
		}
		else
		{
			i += strlen(pBuff + i) ? strlen(pBuff + i) : 1;
		}
	}
}
char* CInterfaceFilesUpload::GetBodyData(char* pBuff, int nSize, string strBoundary, int &nBodySize)
{
	char* pBody = NULL;

	pBody = strstr(pBuff, "\r\n\r\n");
	if (pBody)
	{
		pBody += 4;
		char* pBodyTmp = pBody;
		char* pBoundary = strstr(pBodyTmp, strBoundary.c_str());
		while (pBoundary == NULL && pBodyTmp - pBoundary <= nSize - strBoundary.length())
		{
			if (strlen(pBodyTmp) > 0) pBodyTmp += strlen(pBodyTmp);
			else pBodyTmp += 1;
			pBoundary = strstr(pBodyTmp, strBoundary.c_str());
		}
		if (pBoundary)
		{
			nBodySize = pBoundary - pBody - 2 - 2;	// Boundary前会多两个--
		}
		else
		{
			nBodySize = nSize - (pBody - pBuff);
		}
	}
	return pBody;
}
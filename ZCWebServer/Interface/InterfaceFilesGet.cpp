#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "InterfaceFilesGet.h"

CInterfaceFilesGet::CInterfaceFilesGet()
{
}

CInterfaceFilesGet::~CInterfaceFilesGet()
{
}

string CInterfaceFilesGet::GetUrl()
{
	return I_FILE_FOLDER_FILES;
}

void CInterfaceFilesGet::ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn)
{
	Document tReqDoc;
	tReqDoc.Parse(pReqBody);

	vector<T_Win32FileData> vecFiles;
	if (tReqDoc.FindMember("path") != tReqDoc.MemberEnd())
	{
		string strReqPath = tReqDoc["path"].GetString();
		char* pTemp = NULL;
		int nSize = strReqPath.size() + 1;
		pTemp = new char[nSize];
		memset(pTemp, 0, nSize);
		CEncodingTools::ConvertUTF8ToGB(strReqPath.c_str(), pTemp, nSize);
		FolderFiles(pTemp, "", vecFiles);
		delete pTemp;
	}
	//FolderFiles("D:\\TYProtech\\ZONEE\\Client", "", vecFiles);

	Document tDoc;
	tDoc.SetObject();
	Document::AllocatorType& allocator = tDoc.GetAllocator();
	if (vecFiles.size() > 0)
	{
		tDoc.AddMember("ok", 0, allocator);
		tDoc.AddMember("errorinfo", "", allocator);

		Value array(kArrayType);
		for (int i = 0; i < vecFiles.size(); i++)
		{
			Value object(kObjectType);
			Value vName(kStringType);
			Value vNotifyTime(kStringType);
			Value vTypeName(kStringType);
			Value vSize(kStringType);
			vName.SetString(vecFiles[i].strName.c_str(), vecFiles[i].strName.size(), allocator);
			object.AddMember("name", vName, allocator);
			vNotifyTime.SetString(vecFiles[i].strNotifyTime.c_str(), vecFiles[i].strNotifyTime.size(), allocator);
			object.AddMember("time", vNotifyTime, allocator);
			vTypeName.SetString(vecFiles[i].strTypeName.c_str(), vecFiles[i].strTypeName.size(), allocator);
			object.AddMember("type", vTypeName, allocator);
			vSize.SetString(vecFiles[i].strSize.c_str(), vecFiles[i].strSize.size(), allocator);
			object.AddMember("size", vSize, allocator);
			array.PushBack(object, allocator);
		}

		tDoc.AddMember("data", array, allocator);
	}
	else
	{
		tDoc.AddMember("ok", 1, allocator);
		tDoc.AddMember("errorinfo", "没有找到文件或文件夹", allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	tDoc.Accept(writer);
	string strData = buffer.GetString();

	strReturn = strData;
}

void CInterfaceFilesGet::FolderFiles(const char* chPath, const char* chExtension, vector<T_Win32FileData>& vecFiles)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char chDirSpec[MAX_PATH];
	DWORD dwError;

	if (strcmp(chExtension, "") == 0)
	{
		sprintf_s(chDirSpec, MAX_PATH, "%s\\*", chPath);
	}
	else
	{
		sprintf_s(chDirSpec, MAX_PATH, "%s\\*%s", chPath, chExtension);
	}

	hFind = FindFirstFileA(chDirSpec, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	else
	{
		T_Win32FileData tFileData;
		if (!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			ConvertFileDataToStruct(FindFileData, tFileData);
			vecFiles.push_back(tFileData);
		}

		while (FindNextFileA(hFind, &FindFileData) != 0)
		{
			//if (!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))  //不排除文件夹
			{
				ConvertFileDataToStruct(FindFileData, tFileData);
				vecFiles.push_back(tFileData);
			}
		}

		dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES)
		{
			return;
		}
	}
}

void CInterfaceFilesGet::ConvertFileDataToStruct(const WIN32_FIND_DATAA& tWin32Data, T_Win32FileData& tFileData)
{
	tFileData.Reset();

	SHFILEINFO shfi;
	memset(&shfi, 0, sizeof(SHFILEINFO));
	SHGetFileInfo(tWin32Data.cFileName,
		tWin32Data.dwFileAttributes, &shfi, sizeof(SHFILEINFO), SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME);

	// 文件名称
	tFileData.strName = tWin32Data.cFileName;

	// 文件类型
	if (strcmp(tWin32Data.cFileName, "..") != 0) tFileData.strTypeName = shfi.szTypeName;

	char szTmp[128] = {0};

	// 修改时间
	FILETIME tLocalFileTime;
	SYSTEMTIME tSysTime;
	FileTimeToLocalFileTime(&tWin32Data.ftLastWriteTime, &tLocalFileTime);
	FileTimeToSystemTime(&tLocalFileTime, &tSysTime);
	_snprintf_s(szTmp, sizeof(szTmp), _TRUNCATE, "%04d-%02d-%02d %02d:%02d:%02d",
		tSysTime.wYear, tSysTime.wMonth, tSysTime.wDay, tSysTime.wHour, tSysTime.wMinute, tSysTime.wSecond);
	tFileData.strNotifyTime = szTmp;

	// 文件大小
	string strUnits = "B";
	DWORD dwSize = tWin32Data.nFileSizeLow;	// B
	if (tWin32Data.nFileSizeLow >= 1024)
	{
		dwSize = tWin32Data.nFileSizeLow / 1024;	// KB
		strUnits = "KB";
	}
	if (dwSize < 1000)
		_snprintf_s(szTmp, sizeof(szTmp), _TRUNCATE, "%d %s", dwSize, strUnits.c_str());
	else if (dwSize < 1000000)
		_snprintf_s(szTmp, sizeof(szTmp), _TRUNCATE, "%d,%03d %s", dwSize/1000, dwSize%1000, strUnits.c_str());
	else if (dwSize < 1000000000)
		_snprintf_s(szTmp, sizeof(szTmp), _TRUNCATE, "%d,%03d,%03d %s", dwSize / 1000000, (dwSize%1000000)/1000, dwSize % 1000, strUnits.c_str());

	if (dwSize > 0) tFileData.strSize = szTmp;
}

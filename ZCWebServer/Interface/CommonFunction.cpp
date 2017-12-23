#include "CommonFunction.h"

#pragma comment(lib, "version.lib")

typedef struct
{
	HWND     hWnd;   //进程窗口句柄
	DWORD   dwThreadPid;  //进程
}T_WNDINFO;

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	T_WNDINFO* pInfo = (T_WNDINFO*)lParam;
	DWORD dwProcessId = 0;
	DWORD dwId = GetWindowThreadProcessId(hWnd, &dwProcessId);
	if (dwId == pInfo->dwThreadPid)
	{
		pInfo->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

DWORD CCommonFunction::m_nSerialNo = 0;

string CCommonFunction::GetLocalPath()
{
	char exeFullPath[MAX_PATH];
	memset(exeFullPath,0,MAX_PATH);
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	char *p = strrchr(exeFullPath, '\\');
	if(p)
	{
		*p = '\0';
	}
	string strPath = exeFullPath;
	return strPath;
}

string CCommonFunction::GetParentPath(int nDeep)
{
	char exeFullPath[MAX_PATH];
	memset(exeFullPath,0,MAX_PATH);
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);

	for(int deep = 0; deep < nDeep; ++deep)
	{
		char *p = strrchr(exeFullPath, '\\');
		if(p)
		{
			*p = '\0';
		}
	}
	
	string strPath = exeFullPath;
	return strPath;
}

BOOL CCommonFunction::IsAlphabet(char c)
{
	if( (c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z'))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CCommonFunction::IsNum(char c)
{
	if(c >= '0' && c <= '9')
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CCommonFunction::IsNumString(string strStr)
{
	for (unsigned int i = 0; i < strStr.size(); i++)
	{
		if ((strStr.at(i) > '9') || (strStr.at(i) < '0'))
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CCommonFunction::IsSpaceOrTab(char c)
{
	if(c == ' ' || c == '	')
	{
		return TRUE;
	}
	return FALSE;
}

string CCommonFunction::IntToString(int nNum)
{
	char szTmp[16] = { 0 };
	_snprintf_s(szTmp, sizeof(szTmp), _TRUNCATE, "%d", nNum);
	return szTmp;
}
string& CCommonFunction::StringReplace(string& str, const string& old_value, const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else
		{
			break;
		}
	}
	return str;
}
BOOL CCommonFunction::GetIntKeyValue(string strFileName, string strSection, string strKey, int nDefault, int &nValue)
{
	nValue = nDefault;
	string strValue;
	if(GetStringKeyValue(strFileName, strSection, strKey, "", strValue))
	{
		nValue = atoi(strValue.c_str());
		return TRUE;
	}
	return FALSE;
}

BOOL CCommonFunction::GetStringKeyValue(string strFileName, string strSection, string strKey, string strDefault, string &strValue)
{
	BOOL bRet = FALSE;
	strValue = strDefault;
	char szData[MAX_PATH] = {0};
 	DWORD dwCount = GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(),
 		szData, sizeof(szData) - 1, strFileName.c_str());
	if(dwCount > 0)
	{
		strValue = szData;
		bRet = TRUE;
	}
	return bRet;
}

BOOL CCommonFunction::SetIntKeyValue(string strFileName, string strSection, string strKey, int nValue)
{
	char szValue[32] = {0};
	_snprintf_s(szValue, sizeof(szValue), _TRUNCATE, "%d", nValue);
	return SetStringKeyValue(strFileName, strSection, strKey, szValue);
}
BOOL CCommonFunction::SetStringKeyValue(string strFileName, string strSection, string strKey, string strValue)
{
	return WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), strFileName.c_str());
}

int CCommonFunction::GetNumFromHexChar(char c)
{
	int nNum = 0;
	if(c == '0')nNum = 0;
	else if(c == '0')nNum = 0;
	else if(c == '1')nNum = 1;
	else if(c == '2')nNum = 2;
	else if(c == '3')nNum = 3;
	else if(c == '4')nNum = 4;
	else if(c == '5')nNum = 5;
	else if(c == '6')nNum = 6;
	else if(c == '7')nNum = 7;
	else if(c == '8')nNum = 8;
	else if(c == '9')nNum = 9;
	else if(c == 'A' || c == 'a')nNum = 10;
	else if(c == 'B' || c == 'b')nNum = 11;
	else if(c == 'C' || c == 'c')nNum = 12;
	else if(c == 'D' || c == 'd')nNum = 13;
	else if(c == 'E' || c == 'e')nNum = 14;
	else if(c == 'F' || c == 'f')nNum = 15;
	return nNum;
}


BOOL CCommonFunction::DoEncrypt(string strData, string &strPass)
{
	const char *pData = strData.c_str();
	strPass = "";

	for(unsigned int i = 0; i < strlen(pData); i++)
	{
		unsigned long dwData = pData[i];
		dwData &= 0xFF;
		char szTmp[8] = {0};
		_snprintf_s(szTmp, sizeof(szTmp), _TRUNCATE, "%02X", dwData);
		strPass += szTmp;
	}

	const char* pPass = strPass.c_str();
	for(int i = 0, x = strPass.length()/2, y = strPass.length() - 1; i < strPass.length()/2; i++)
	{
		if(i%2 == 0)
		{
			char cI = pPass[i];
			char cX = pPass[x];
			strPass.replace(i, 1, 1, cX);
			strPass.replace(x, 1, 1, cI);
			x++;
		}
		else
		{
			char cI = pPass[i];
			char cY = pPass[y];
			strPass.replace(i, 1, 1, cY);
			strPass.replace(y, 1, 1, cI);
			y--;
		}
	}
	return TRUE;
}
BOOL CCommonFunction::DoDecrypt(string strPass, string &strData)
{
	strData = "";
	if(strPass.length() % 2 != 0)
		return FALSE;

	const char *pPass = strPass.c_str();

	for(int i = 0, x = strPass.length()/2, y = strPass.length() - 1; i < strPass.length()/2; i++)
	{
		if(i%2 == 0)
		{
			char cI = pPass[i];
			char cX = pPass[x];
			strPass.replace(i, 1, 1, cX);
			strPass.replace(x, 1, 1, cI);
			x++;
		}
		else
		{
			char cI = pPass[i];
			char cY = pPass[y];
			strPass.replace(i, 1, 1, cY);
			strPass.replace(y, 1, 1, cI);
			y--;
		}
	}

	for(unsigned int i = 0; i < strlen(pPass); i++)
	{
		char c1 = 0, c2 = 0;
		memcpy(&c1, pPass+2*i, 1);
		memcpy(&c2, pPass+2*i+1, 1);
		unsigned long dwPass = (GetNumFromHexChar(c1)<<4)+ (GetNumFromHexChar(c2));;
		char cTmp = (char)(dwPass);
		strData += cTmp;
	}

	return TRUE;
}
// 创建进程
BOOL CCommonFunction::CreateProcess(string strProcessName, string strParamString, DWORD &dwPid, HWND &hWnd)
{
	if(strProcessName.length() <= 0)
	{
		return FALSE;
	}

	BOOL bRet = TRUE;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	int nSize = strProcessName.length() + strParamString.length() + 16;
	char* pCommandLine = new char[nSize];
	memset(pCommandLine, 0, nSize);
	_snprintf_s(pCommandLine, nSize - 1, _TRUNCATE,
		"%s \"%s\"", strProcessName.c_str(), strParamString.c_str());
	bRet = ::CreateProcess(NULL, pCommandLine, NULL, NULL, FALSE,
		0, NULL, NULL, &startupInfo, &processInfo);
	if(bRet)
	{
		// 等待进程创建完毕
		::WaitForInputIdle(processInfo.hProcess, INFINITE);
		dwPid = processInfo.dwProcessId;
		hWnd = GetHwndByProcessId(processInfo.dwThreadId);

		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	if(pCommandLine)
	{
		delete[] pCommandLine;
	}
	return bRet;
}

HWND CCommonFunction::GetHwndByProcessId( DWORD dwThreadId )
{
	T_WNDINFO info = {0};
	info.hWnd = NULL;
	info.dwThreadPid = dwThreadId;
	DWORD dwStartTick = GetTickCount();
	while (1)
	{
		::EnumWindows(&EnumWindowsProc, (LPARAM)&info);//Iterate all windows

		if ((info.hWnd != NULL) ||(5000 < GetTickCount() - dwStartTick))
		{
			break;
		}		
	}
	return info.hWnd;
}

// 销毁进程
BOOL CCommonFunction::DestoryProcess(DWORD nPid)
{
	BOOL bRet = FALSE;
	HANDLE hHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	if(hHandle)
	{
		bRet = ::TerminateProcess(hHandle, 0);
		::CloseHandle(hHandle);
	}
	return bRet;
}

// 销毁进程
BOOL CCommonFunction::DestoryProcess(string strProcessName)
{
	BOOL bRet = TRUE;
	PROCESSENTRY32 pe32;  
	pe32.dwSize = sizeof(pe32);   
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return bRet;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	while(bMore)  
	{  
		//printf(" 进程名称：%s \n", pe32.szExeFile);  
		if(stricmp(strProcessName.c_str(), pe32.szExeFile) == 0 &&
			pe32.th32ProcessID != GetCurrentProcessId())  
		{  
			HANDLE hHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			if(hHandle)
			{
				bRet = ::TerminateProcess(hHandle, 0);
				::CloseHandle(hHandle);
			}
		}  
		bMore = ::Process32Next(hProcessSnap, &pe32);  
	}  
	CloseHandle(hProcessSnap);

	return bRet;
}

// 检查进程是否存在
BOOL CCommonFunction::CheckProcessExist(DWORD nPid)
{
 	BOOL bRet = FALSE;
	PROCESSENTRY32 pe32;  
	pe32.dwSize = sizeof(pe32);   
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return bRet;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	while(bMore)  
	{  
		//printf(" 进程名称：%s \n", pe32.szExeFile);  
		if(nPid == pe32.th32ProcessID)  
		{  
			bRet = TRUE;
			break;
		}  
		bMore = ::Process32Next(hProcessSnap, &pe32);  
	}  
	CloseHandle(hProcessSnap);

	return bRet;
}

// 检查进程是否存在
BOOL CCommonFunction::CheckProcessExist(string strProcessName)
{
	BOOL bRet = FALSE;
	PROCESSENTRY32 pe32;  
	pe32.dwSize = sizeof(pe32);   
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return bRet;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);  
	while(bMore)  
	{  
		//printf(" 进程名称：%s \n", pe32.szExeFile);  
		if (stricmp(strProcessName.c_str(), pe32.szExeFile) == 0 &&
			pe32.th32ProcessID != GetCurrentProcessId())  
		{  
			bRet = TRUE;
			break;
		}  
		bMore = ::Process32Next(hProcessSnap, &pe32);  
	}  
	CloseHandle(hProcessSnap);

	return bRet;
}
//获取进程版本号
string CCommonFunction::GetVersion()
{
	string ver = "unknown";

	char fileName[MAX_PATH];
	GetModuleFileName(NULL, fileName, sizeof(fileName));

	DWORD dwHandle;
	int size = GetFileVersionInfoSize(fileName, &dwHandle);
	if (size > 0)
	{
		byte *pBuffer = new byte[size];
		if (GetFileVersionInfo((LPCSTR)fileName, dwHandle, size, pBuffer))
		{
			VS_FIXEDFILEINFO *pVI = NULL;
			if (VerQueryValue(pBuffer, (LPCSTR)"\\", (LPVOID*)&pVI, (PUINT)&size))
			{
				char szVer[64] = {0};
				_snprintf_s(szVer, sizeof(szVer), _TRUNCATE,
					"v%d.%d.%d.%d", HIWORD(pVI->dwFileVersionMS), LOWORD(pVI->dwFileVersionMS),
					HIWORD(pVI->dwFileVersionLS), LOWORD(pVI->dwFileVersionLS));
				ver = szVer;
			}
		}
		delete[] pBuffer;
	}

	return ver;
}

BOOL CCommonFunction::GetVersion(char* filepath)
{
	DWORD dwHandle = NULL;
	DWORD dwFileVersionInfoSize = GetFileVersionInfoSize(filepath, &dwHandle);

	if (NULL == dwFileVersionInfoSize)
		return FALSE;

	// Allocate space to retrieve version information using vector to prevent
	// memory leaks
	std::vector<BYTE> pData(dwFileVersionInfoSize);

	// Retrieves version information for the specified file.
	if (false == GetFileVersionInfo(filepath
		, dwHandle
		, dwFileVersionInfoSize
		, static_cast<LPVOID>(&pData[0])))
		return FALSE; // Can't retrieve version information.

	// The memory of ptFileInfo is freed when pData is freed.
	VS_FIXEDFILEINFO *ptFileInfo;
	UINT	uintSize;

	// Retrieves version information from the version-information resource
	if (false == VerQueryValue(static_cast<LPVOID>(&pData[0])
		, _T("\\")
		, reinterpret_cast<LPVOID*>(&ptFileInfo)
		, &uintSize))
		return FALSE; // Can't retrieve version information

	USHORT musMajorVersion, musMinorVersion, musReleaseNumber, musBuildNumber;
	// Resolve major, minor, release and build number.
	musMajorVersion = static_cast<unsigned short>(
		(ptFileInfo->dwFileVersionMS >> 16) & 0xffff);

	musMinorVersion = static_cast<unsigned short>(
		ptFileInfo->dwFileVersionMS & 0xffff);

	musReleaseNumber = static_cast<unsigned short>(
		(ptFileInfo->dwFileVersionLS >> 16) & 0xffff);

	musBuildNumber = static_cast<unsigned short>(
		ptFileInfo->dwFileVersionLS & 0xffff);

	wsprintfA(filepath, "%d.%d.%d.%d", musMajorVersion, musMinorVersion, musReleaseNumber, musBuildNumber);

	return true;
}

// 创建线程
void CCommonFunction::CreateThread(LPTHREAD_START_ROUTINE pFun, void* pUser)
{
	DWORD dwThreadId = 0;
	HANDLE hHandle = ::CreateThread(NULL, 0, pFun, pUser, 0, &dwThreadId);
	::CloseHandle(hHandle);
}

// 创建信号量
HANDLE CCommonFunction::CreateEvent(string strName, BOOL bEnable, BOOL bManual)
{
	if(strName.length() > 0)
	{
		HANDLE hHandle = ::OpenEvent(EVENT_ALL_ACCESS , FALSE, strName.c_str());
		if(hHandle == NULL)
		{
			hHandle = ::CreateEvent(NULL, bManual, bEnable, strName.c_str());
		}
		return hHandle;
	}
	else
	{
		return ::CreateEvent(NULL, bManual, bEnable, NULL);
	}
}

// 打开信号量
HANDLE CCommonFunction::OpenEvent(string strName)
{
	HANDLE hHandle = NULL;
	if(strName.length() > 0)
	{
		hHandle = ::OpenEvent(EVENT_ALL_ACCESS , FALSE, strName.c_str());
	}
	return hHandle;
}

DWORD CCommonFunction::GetSerialNo()
{
	return m_nSerialNo++;
}

//结束单个进程
BOOL CCommonFunction::TerminateProcessByName(const TCHAR* pcProcessName)
{
	BOOL bFound = FALSE;
	BOOL bSuccess = TRUE;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if(Process32First(hSnapshot, &entry) == TRUE)
	{
		while(Process32Next(hSnapshot, &entry) == TRUE)
		{
			if(_tcsicmp(entry.szExeFile, pcProcessName) == 0)
			{  
				bFound = TRUE;

				HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, entry.th32ProcessID);

				BOOL bRet = ::TerminateProcess(hProcess, 0);

				//尝试终结进程，如果失败，暂不处理
				if(!bRet)
				{
					/*::Sleep(5000);
					bRet = ::TerminateProcess(hProcess, 0);

					bSuccess = bRet;*/
				}

				::CloseHandle(hProcess);
				//break;
			}
		}
	}

	CloseHandle(hSnapshot);

	return bSuccess;
}
BOOL CCommonFunction::FindFolder(const string strPath, const string pcSubStr, vector<string> &vecPath)
{
	string strCmd = "dir /b /ad ";
	strCmd += strPath;
	strCmd += " > ";
	char szFile[64] = { 0 };
	_snprintf_s(szFile, sizeof(szFile), _TRUNCATE, "FindFolder_%d", rand());
	FILE* pFile = fopen(szFile, "r");
	while (pFile != NULL)
	{
		fclose(pFile);
		_snprintf_s(szFile, sizeof(szFile), _TRUNCATE, "FindFolder_%d", rand());
		pFile = fopen(szFile, "r");
	}
	strCmd += szFile;

	system(strCmd.c_str());
	char szTmp[1024] = { 0 };
	strncpy(szTmp, strCmd.c_str(), 1023);
	pFile = fopen(szFile, "rb");
	if (pFile == NULL)
		return FALSE;

	char szData[1024] = { 0 };
	string strTmpPath = strPath;
	string strData;
	if (strPath[strPath.length() - 1] != '\\') strTmpPath += "\\";
	while (feof(pFile) == FALSE)
	{
		memset(szData, 0, sizeof(szData));
		fgets(szData, sizeof(szData), pFile);
		if (szData[strlen(szData) - 1] == '\n' || szData[strlen(szData) - 1] == '\r')
			szData[strlen(szData) - 1] = 0;
		if (szData[strlen(szData) - 1] == '\n' || szData[strlen(szData) - 1] == '\r')
			szData[strlen(szData) - 1] = 0;

		strData = strTmpPath;
		strData += szData;
		if (pcSubStr.length() == 0)
		{
			if (strData.length() > 0)
			{
				vecPath.push_back(strData);
			}
		}
		else if (strstr(szData, pcSubStr.c_str()) != NULL)
		{
			if (strData.length() > 0)
			{
				vecPath.push_back(strData);
			}
		}
	}

	fclose(pFile);
	remove(szFile);
	return TRUE;
}
BOOL CCommonFunction::GetRegString(HKEY hKey, const char* pcSubKey, string &strData)
{
	BOOL bRet = FALSE;
	long lRet = 0;
	HKEY hKeyHandle = NULL;
	TCHAR tchData[256] = { 0 };
	DWORD dwSize = 0;
	lRet = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		/*"Software\\TYProtech\\ZONEE\\Client\\Settings"*/pcSubKey,
		0,
		KEY_QUERY_VALUE,
		&hKeyHandle
		);    //打开注册表
	if (lRet == ERROR_SUCCESS)//读操作成功
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(
			hKeyHandle,
			"InstallPath",
			NULL,
			NULL,
			(LPBYTE)tchData,
			&dwSize
			);    //如果打开成功，则读
		if (lRet == ERROR_SUCCESS)
		{
			strData = tchData;
			bRet = TRUE;
		}

	}
	RegCloseKey(hKeyHandle);//记住，一定要关闭
	return bRet;
}
BOOL CCommonFunction::TakeProcessPrivilege()
{
	BOOL bEnablePrivilege = TRUE;

	HANDLE hToken;  
	BOOL bRet = OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken);  

	TOKEN_PRIVILEGES tp;  
	LUID luid;  

	if ( !LookupPrivilegeValue(   
		NULL,            // lookup privilege on local system   
		SE_DEBUG_NAME,   // privilege to lookup    
		&luid ) )        // receives LUID of privilege   
	{  
		//printf("LookupPrivilegeValue error: %u\n", GetLastError() );   
		return FALSE;   
	}  

	tp.PrivilegeCount = 1;  
	tp.Privileges[0].Luid = luid;  
	if (bEnablePrivilege)  
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
	else  
		tp.Privileges[0].Attributes = 0;  

	// Enable the privilege or disable all privileges.   

	if ( !AdjustTokenPrivileges(  
		hToken,   
		FALSE,   
		&tp,   
		sizeof(TOKEN_PRIVILEGES),   
		(PTOKEN_PRIVILEGES) NULL,   
		(PDWORD) NULL) )  
	{   
		//printf("AdjustTokenPrivileges error: %u\n", GetLastError() );   
		return FALSE;   
	}   

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)  

	{  
		//printf("The token does not have the specified privilege. \n");  
		return FALSE;  
	}   

	return TRUE;  
}  


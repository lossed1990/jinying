/**
* @file CommonFunctio.h 常用函数接口类
*/

#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <Tlhelp32.h>
#include <WinVer.h>
#include <tchar.h>

using namespace std;

class CCommonFunction
{
public:
	static string GetLocalPath();	           // 获取本地运行路径
	static string GetParentPath(int nDeep);    // 获取上级目录路径
	static BOOL IsAlphabet(char c);	// 判断是否是字母
	static BOOL IsNum(char c);			// 判断是否是数字
	static BOOL IsNumString(string strStr);		// 判断是否是纯数字字符串
	static BOOL IsSpaceOrTab(char c);	// 判断是否是空格或tab

	static string IntToString(int nNum);		// 整型转换为string
	static string& StringReplace(string& str, const string& old_value, const string& new_value);
	/************************************************************************/
	/* 读写配置文件                                                         */
	/************************************************************************/
	static BOOL GetIntKeyValue(string strFileName, string strSection, string strKey, int strDefault, int &strValue);
	static BOOL GetStringKeyValue(string strFileName, string strSection, string strKey, string strDefault, string &strValue);
	static BOOL SetIntKeyValue(string strFileName, string strSection, string strKey, int nValue);
	static BOOL SetStringKeyValue(string strFileName, string strSection, string strKey, string strValue);

	static int GetNumFromHexChar(char c);		// 按照16进制计算char的数值
	static BOOL DoEncrypt(string strData, string &strPass);		// 加密
	static BOOL DoDecrypt(string strPass, string &strData);		// 解密

	static BOOL FindFolder(const string pcPath, const string pcSubStr, vector<string> &vecPath);
	/************************************************************************/
	/* 注册表操作                                                           */
	/************************************************************************/
	static BOOL GetRegString(HKEY hKey, const char* pcSubKey, string &strData);		// 读取某个值

	/************************************************************************/
	/* 进程相关                                                             */
	/************************************************************************/
	static BOOL TakeProcessPrivilege();			// 获取权限，否则OpenProcess可能会失败（拒绝访问）
	static BOOL CreateProcess(string strProcessName, string strParamString, DWORD &dwPid, HWND &hWnd);// 创建进程
	static BOOL DestoryProcess(DWORD nPid);		// 销毁进程
	static BOOL DestoryProcess(string strProcessName);		// 销毁进程
	static BOOL CheckProcessExist(DWORD nPid);	// 检查进程是否存在
	static BOOL CheckProcessExist(string strProcessName);	// 检查进程是否存在
	static string GetVersion();                      // 获取进程版本号
	static BOOL GetVersion(char* filepath);          // 获取进程版本号
	static BOOL TerminateProcessByName(const TCHAR* pcProcessName);  // 销毁进程
	static HWND GetHwndByProcessId( DWORD dwThreadId );	// 获取窗口句柄

	static void CreateThread(LPTHREAD_START_ROUTINE pFun, void* pUser);		// 创建线程
	static HANDLE CreateEvent(string strName, BOOL bEnable = TRUE, BOOL bManual = FALSE);	// 创建信号量
	static HANDLE OpenEvent(string strName);	// 创建信号量

	static DWORD GetSerialNo();

private:
	static DWORD m_nSerialNo;	// 序列号
};

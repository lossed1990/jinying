/**
* @file CommonFunctio.h ���ú����ӿ���
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
	static string GetLocalPath();	           // ��ȡ��������·��
	static string GetParentPath(int nDeep);    // ��ȡ�ϼ�Ŀ¼·��
	static BOOL IsAlphabet(char c);	// �ж��Ƿ�����ĸ
	static BOOL IsNum(char c);			// �ж��Ƿ�������
	static BOOL IsNumString(string strStr);		// �ж��Ƿ��Ǵ������ַ���
	static BOOL IsSpaceOrTab(char c);	// �ж��Ƿ��ǿո��tab

	static string IntToString(int nNum);		// ����ת��Ϊstring
	static string& StringReplace(string& str, const string& old_value, const string& new_value);
	/************************************************************************/
	/* ��д�����ļ�                                                         */
	/************************************************************************/
	static BOOL GetIntKeyValue(string strFileName, string strSection, string strKey, int strDefault, int &strValue);
	static BOOL GetStringKeyValue(string strFileName, string strSection, string strKey, string strDefault, string &strValue);
	static BOOL SetIntKeyValue(string strFileName, string strSection, string strKey, int nValue);
	static BOOL SetStringKeyValue(string strFileName, string strSection, string strKey, string strValue);

	static int GetNumFromHexChar(char c);		// ����16���Ƽ���char����ֵ
	static BOOL DoEncrypt(string strData, string &strPass);		// ����
	static BOOL DoDecrypt(string strPass, string &strData);		// ����

	static BOOL FindFolder(const string pcPath, const string pcSubStr, vector<string> &vecPath);
	/************************************************************************/
	/* ע������                                                           */
	/************************************************************************/
	static BOOL GetRegString(HKEY hKey, const char* pcSubKey, string &strData);		// ��ȡĳ��ֵ

	/************************************************************************/
	/* �������                                                             */
	/************************************************************************/
	static BOOL TakeProcessPrivilege();			// ��ȡȨ�ޣ�����OpenProcess���ܻ�ʧ�ܣ��ܾ����ʣ�
	static BOOL CreateProcess(string strProcessName, string strParamString, DWORD &dwPid, HWND &hWnd);// ��������
	static BOOL DestoryProcess(DWORD nPid);		// ���ٽ���
	static BOOL DestoryProcess(string strProcessName);		// ���ٽ���
	static BOOL CheckProcessExist(DWORD nPid);	// �������Ƿ����
	static BOOL CheckProcessExist(string strProcessName);	// �������Ƿ����
	static string GetVersion();                      // ��ȡ���̰汾��
	static BOOL GetVersion(char* filepath);          // ��ȡ���̰汾��
	static BOOL TerminateProcessByName(const TCHAR* pcProcessName);  // ���ٽ���
	static HWND GetHwndByProcessId( DWORD dwThreadId );	// ��ȡ���ھ��

	static void CreateThread(LPTHREAD_START_ROUTINE pFun, void* pUser);		// �����߳�
	static HANDLE CreateEvent(string strName, BOOL bEnable = TRUE, BOOL bManual = FALSE);	// �����ź���
	static HANDLE OpenEvent(string strName);	// �����ź���

	static DWORD GetSerialNo();

private:
	static DWORD m_nSerialNo;	// ���к�
};

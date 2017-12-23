#pragma once

#include "InterfaceBase.h"

#include <process.h>
#include <windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <shellapi.h>

typedef struct
{
	string   base;     //ģ����
	string   current;  //�����
	int      status;   //�Ƚ�״̬
}CONFIG_DIFFINFO;

class CInterfaceGetConfigContent : public CInterfaceBase
{
public:
	CInterfaceGetConfigContent();
	virtual ~CInterfaceGetConfigContent();

	virtual void ExecuteInterface(char* pReqBody, int nReqBodyLen, string& strReturn);
	virtual string GetUrl();

private:
	void ReadFile(const char* chPath, vector<string>& vecFileLines);
	void CompareFile(vector<string>& vecBaseFileLines, vector<string>& vecCurrentFileLines, vector<CONFIG_DIFFINFO>& vecResult);
};
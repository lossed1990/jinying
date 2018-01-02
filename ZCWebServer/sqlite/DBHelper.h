#pragma once
#include "sqlite3.h"
#include <functional>

using std::function;

class CDBHelper
{
private:
	static CDBHelper* m_pDBHelperInst;
	CDBHelper(void);

public:
	~CDBHelper(void);

	static CDBHelper* Instance(){ return (m_pDBHelperInst == nullptr ? m_pDBHelperInst = new CDBHelper() : m_pDBHelperInst); }

	static void FreeInstance()
	{
		if (m_pDBHelperInst)
		{
			delete m_pDBHelperInst;
			m_pDBHelperInst = nullptr;
		}
	}

public:
	bool ExecSql(const char* chSql, int(*callback)(void*, int, char**, char**), void* pUser = NULL);
	bool ExecSearch(const char* chSql, function<void(sqlite3_stmt*)> pCallBack);
	void Log(const char* chUser, const char* chOperateType, const char* chContent);
	void LogUTF8(const char* chUser, const char* chOperateType, const char* chContent);

private:
	sqlite3* m_db;
	bool m_bOpenDBSuccess = false;
};
#include <iostream>
#include "DBHelper.h"
#include "../ToolFuncs/ToolFuncs.h"

#ifdef _DEBUG
#pragma comment (lib, "sqlite/sqlite3_d.lib")
#else
#pragma comment (lib, "sqlite/sqlite3.lib")
#endif

CDBHelper* CDBHelper::m_pDBHelperInst = nullptr;

CDBHelper::CDBHelper(void)
{
	TCHAR tpcPath[260] = { 0 };
	GetExeFileSubDirectory(tpcPath, 260, "data.db");

	char *zErrMsg = 0;
	int rc = sqlite3_open(tpcPath, &m_db);
	if (rc){
		fprintf(stderr, "CDBHelper>>Can't open database: %s\n", sqlite3_errmsg(m_db));
		m_bOpenDBSuccess = false;
	}
	else{
		fprintf(stderr, "CDBHelper>>Opened database successfully\n");
		m_bOpenDBSuccess = true;
	}
}

CDBHelper::~CDBHelper(void)
{
	sqlite3_close(m_db);
}

bool CDBHelper::ExecSql(const char* chSql, int(*callback)(void*, int, char**, char**), void* pUser)
{
	if (!m_bOpenDBSuccess)
	{
		return false;
	}

	char *zErrMsg = 0;
	int rc = sqlite3_exec(m_db, chSql, callback, pUser, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "CDBHelper::ExecSql>> error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}

	fprintf(stdout, "CDBHelper::ExecSql>> successfully\n");
	return true;
}

bool CDBHelper::ExecSearch(const char* chSql, function<void(sqlite3_stmt*)> pCallBack)
{
	if (!m_bOpenDBSuccess)
	{
		return false;
	}

	sqlite3_stmt * stmt = NULL;
	const char *zTail;

	if (sqlite3_prepare_v2(m_db, chSql, -1, &stmt, &zTail) == SQLITE_OK){
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			pCallBack(stmt);
		}
	}
	else
	{
		fprintf(stderr, "CDBHelper::ExecSearch>> failed\n");
	}

	sqlite3_finalize(stmt);
	return true;
}





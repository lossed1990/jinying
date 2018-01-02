#include <iostream>
#include "DBHelper.h"
#include "../ToolFuncs/ToolFuncs.h"
#include "../JsonRequest/EncodingToolsClass.h"

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

void CDBHelper::Log(const char* chUser,const char* chOperateType,const char* chContent)
{
	if (!m_bOpenDBSuccess)
	{
		return;
	}

	char chUTF8User[128] = { 0 };
	char chUTF8OperateType[256] = { 0 };
	char chUTF8Content[4096] = { 0 };
	CEncodingTools::ConvertGBToUTF8(chUser, chUTF8User, strlen(chUser) * 2);
	CEncodingTools::ConvertGBToUTF8(chOperateType, chUTF8OperateType, strlen(chOperateType) * 2);
	CEncodingTools::ConvertGBToUTF8(chContent, chUTF8Content, strlen(chContent) * 2);


	char chSql[8192] = { 0 };
	sprintf_s(chSql, 8192, "INSERT INTO log (user, type, condition, time) VALUES ('%s', '%s', '%s', datetime('now','localtime'));",
		chUTF8User, chUTF8OperateType, chUTF8Content);

	auto pCallBack = [](void *data, int argc, char **argv, char **azColName) -> int
	{
		int i;
		fprintf(stderr, "%s: ", (const char*)data);
		for (i = 0; i < argc; i++){
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	};

	CDBHelper::Instance()->ExecSql(chSql, pCallBack);
}

void CDBHelper::LogUTF8(const char* chUser, const char* chOperateType, const char* chContent)
{
	if (!m_bOpenDBSuccess)
	{
		return;
	}

	char chUTF8User[128] = { 0 };
	char chUTF8OperateType[256] = { 0 };
	CEncodingTools::ConvertGBToUTF8(chUser, chUTF8User, strlen(chUser) * 2);
	CEncodingTools::ConvertGBToUTF8(chOperateType, chUTF8OperateType, strlen(chOperateType) * 2);

	char chSql[8192] = { 0 };
	sprintf_s(chSql, 8192, "INSERT INTO log (user, type, condition, time) VALUES ('%s', '%s', '%s', datetime('now','localtime'));",
		chUTF8User, chUTF8OperateType, chContent);

	auto pCallBack = [](void *data, int argc, char **argv, char **azColName) -> int
	{
		int i;
		fprintf(stderr, "%s: ", (const char*)data);
		for (i = 0; i < argc; i++){
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	};

	CDBHelper::Instance()->ExecSql(chSql, pCallBack);
}




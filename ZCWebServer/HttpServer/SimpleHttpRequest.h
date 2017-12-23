#pragma once

#include <string>
using std::string;

class CSimpleHttpRequest
{
public:
	CSimpleHttpRequest(char* pcRequestString);
	~CSimpleHttpRequest();

public:
	int GetPaseStatus();

	string GetMethod();
	string GetURI();
	string GetParam();
	string GetContentType();
	char* GetRequestBodyBuf(char* pBuf, int nSize);
	int GetRequestBodyLen();
	int GetContentLenth();

	void SetRequestBody(char* pBuf, int nSize);

private:
	int paseRequest(char* pcRequestString);

	int paseMethodLine(char* pcLine);
	int paseHostLine(char* pcLine);
	int paseUALine(char* pcLine);
	int paseAccept(char* pcLine);
	int paseAcceptLan(char* pcLine);
	int paseAcceptEncoding(char* pcLine);
	int paseContentType(char* pcLine);
	int paseContentLength(char* pcLine);
	int paseConnection(char* pcLine);
	int paseFilename(char* pcLine);

	void fixURIRequest(string& strOrgnURI);

private:
	string m_strMethod;
	string m_strURI;
	string m_strGetParam;
	string m_strHttpProtocolVersion;
	string m_strHost;
	string m_strUserAgent;
	string m_strAcceptDocType;
	string m_strAcceptLanguage;
	string m_strAcceptEncoding;
	string m_strContentType;
	string m_strConnectionType;
	string m_strFilename;

	char* m_pRequestBodyBuf;
	int m_nRequestBodyLen;

	int m_nContentLength;
	int m_nPaseStatus;
};
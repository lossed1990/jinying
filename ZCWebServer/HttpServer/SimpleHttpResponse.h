#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

class CSimpleHttpResponse
{
public:
	CSimpleHttpResponse(int nStatusCode);
	~CSimpleHttpResponse();

public:
	string GetContentTypeFromExtensionLowCase(string& strExt);
	//void SetCookie(string& strCookieString, int nExpireSecs);
	void AddCookie(string& strCookieString, int nExpireSecs, bool bClear);
	void SetContentLength(int nLength);
	void SetContentType(string& strContentType);
	void SetLocation(string& strLocation);
	void SetContentDisposition(string& strContentDisposition);
	string BuildResponseHeader();

private:
	string m_strHttpVersion;
	string m_strStatusCode;
	string m_strServerType;
	string m_strDate;
	string m_strContentType;
	string m_strConnection;
	string m_strCacheControl;
	string m_strExpires;
	//string m_strCookie;
	string m_strLocation;
	string m_strContentDisposition;
	int m_nContentLength;

	vector<string> m_vecCookies;
};


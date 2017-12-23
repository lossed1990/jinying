#include "SimpleHttpResponse.h"

#include <stdio.h>
#include <time.h>

CSimpleHttpResponse::CSimpleHttpResponse(int nStatusCode)
{
	m_nContentLength = 0;
	m_strContentType = "text/html; charset=UTF-8";

	if (200 == nStatusCode)
	{
		m_strStatusCode = "200 OK";
	}
	else if (302 == nStatusCode)
	{
		m_strStatusCode = "302 Found";
	}
	else if (400 == nStatusCode)
	{
		m_strStatusCode = "400 Bad Request";
	}
	else if (401 == nStatusCode)
	{
		m_strStatusCode = "401 Unauthorized";
	}
	else if (403 == nStatusCode)
	{
		m_strStatusCode = "403 Forbidden";
	}
	else if (404 == nStatusCode)
	{
		m_strStatusCode = "404 Not Found";
	}
	else
	{
		m_strStatusCode = "500 Internal Server Error";
	}
}


CSimpleHttpResponse::~CSimpleHttpResponse()
{
}

void CSimpleHttpResponse::SetContentLength(int nLength)
{
	m_nContentLength = nLength;
}

void CSimpleHttpResponse::SetContentType(string& strContentType)
{
	m_strContentType = strContentType;
}

//void CSimpleHttpResponse::SetCookie(string& strCookieString, int nExpireSecs)
//{
//	m_strCookie = strCookieString;
//	m_strCookie += "expires=";
//
//	//Date
//	char pcBuffTemp[128] = { 0 };
//	time_t tNow = ::time(NULL) + nExpireSecs;
//	struct tm tmNow;
//
//	gmtime_s(&tmNow, &tNow);
//	::strftime(pcBuffTemp, sizeof(pcBuffTemp), "%a, %d %b %Y %H:%M:%S GMT", &tmNow);
//	m_strCookie += pcBuffTemp;
//	m_strCookie += ";";
//}

void CSimpleHttpResponse::AddCookie(string& strCookieString, int nExpireSecs, bool bClear)
{
	if (bClear)
	{
	    m_vecCookies.clear();
	}

	string strCookie = strCookieString;
	strCookie += "expires=";

	//Date
	char pcBuffTemp[128] = { 0 };
	time_t tNow = ::time(NULL) + nExpireSecs;
	struct tm tmNow;

	gmtime_s(&tmNow, &tNow);
	::strftime(pcBuffTemp, sizeof(pcBuffTemp), "%a, %d %b %Y %H:%M:%S GMT", &tmNow);
	strCookie += pcBuffTemp;
	strCookie += ";";
	m_vecCookies.push_back(strCookie);
}

void CSimpleHttpResponse::SetLocation(string& strLocation)
{
	m_strLocation = strLocation;
}
void CSimpleHttpResponse::SetContentDisposition(string& strContentDisposition)
{
	m_strContentDisposition = strContentDisposition;
}

//从扩展名转换ContentType，不包括点号
string CSimpleHttpResponse::GetContentTypeFromExtensionLowCase(string& strExt)
{
	if (strExt.compare("bmp") == 0)
	{
		return string("application/x-bmp");
	}
	else if (strExt.compare("css") == 0)
	{
		return string("text/css");
	}
	else if (strExt.compare("emf") == 0)
	{
		return string("application/x-emf");
	}
	else if (strExt.compare("gif") == 0)
	{
		return string("image/gif");
	}
	else if (strExt.compare("html") == 0 || strExt.compare("htm") == 0)
	{
		return string("text/html; charset=UTF-8");
	}
	else if (strExt.compare("ico") == 0)
	{
		return string("image/x-icon");
	}
	else if (strExt.compare("img") == 0)
	{
		return string("application/x-img");
	}
	else if (strExt.compare("json") == 0)
	{
		return string("application/json; charset=UTF-8");
	}
	else if (strExt.compare("jpe") == 0 || strExt.compare("jpeg") == 0 || strExt.compare("jpg") == 0)
	{
		return string("image/jpeg");
	}
	else if (strExt.compare("mp4") == 0)
	{
		return string("video/mpeg4");
	}
	else if (strExt.compare("png") == 0)
	{
		return string("image/png");
	}
	else if (strExt.compare("swf") == 0)
	{
		return string("application/x-shockwave-flash");
	}
	else if (strExt.compare("tif") == 0 || strExt.compare("tiff") == 0)
	{
		return string("image/tiff");
	}
	else if (strExt.compare("xml") == 0)
	{
		return string("application/xml; charset=UTF-8");
	}
	else if (strExt.compare("js") == 0)
	{
		return string("application/javascript");
	}
	else if (strExt.compare("mp3") == 0)
	{
		return string("audio/mp3");
	}
	else
	{
		//未知类型设置为二进制流
		return string("application/octet-stream");
	}
}

string CSimpleHttpResponse::BuildResponseHeader()
{
	//属性赋值
	m_strHttpVersion = "HTTP/1.1";
	m_strServerType = "SM.ST.Web 1.0.0.1";

	//Date
	char pcBuffTemp[128] = {0};
	time_t tNow = ::time(NULL);
	struct tm tmNow;

	gmtime_s(&tmNow, &tNow);
	::strftime(pcBuffTemp, sizeof(pcBuffTemp), "%a, %d %b %Y %H:%M:%S GMT", &tmNow);
	m_strDate = pcBuffTemp;
	m_strExpires = m_strDate;

	//Contentlength
	sprintf_s(pcBuffTemp, "%d", m_nContentLength);
	string strContentLength = pcBuffTemp;

	m_strConnection = "close";
	m_strCacheControl = "no-cache";

	//汇集为串
	string strOutput = "";

	strOutput += m_strHttpVersion + " " + m_strStatusCode + "\r\n";
	strOutput += "Server: " + m_strServerType + "\r\n";
	strOutput += "Date: " + m_strDate + "\r\n";
	strOutput += "Content-Type: " + m_strContentType + "\r\n";
	strOutput += "Connection: " + m_strConnection + "\r\n";
	strOutput += "Cache-Control: " + m_strCacheControl + "\r\n";
	strOutput += "Expires: " + m_strExpires + "\r\n";
	strOutput += "Access-Control-Allow-Origin: * \r\n";
	strOutput += "Content-Length: " + strContentLength + "\r\n";
	strOutput += "Content-Disposition: " + m_strContentDisposition + "\r\n";

	//是否有跳转302
	if (m_strLocation.length() > 1)
	{
		strOutput += "Location: " + m_strLocation + "\r\n";
	}

	//是否有cookie
	/*if (m_strCookie.length() > 1)
	{
		strOutput += "Set-Cookie: " + m_strCookie + "\r\n";
	}*/
	for (auto&item : m_vecCookies)
	{
		strOutput += "Set-Cookie: " + item + "\r\n";
	}

	strOutput += "\r\n";

	return strOutput;
}

#include "EncodingToolsClass.h"

#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>

#define CONNECT_TIME_OUT	5
#define HTTP_RECV_TIMEOUT	25

#include "curl.h"
#pragma comment(lib, "thirdpart/lib/libcurl.lib")




//转换URL
bool CEncodingTools::MyUrlEscape(const char* pcOrgn, char* pcEscaped, unsigned int* pdwCount)
{
	if (pcOrgn == NULL || pdwCount == NULL) return S_FALSE;

	DWORD nOrgnCount = strlen(pcOrgn);

	if (nOrgnCount >= (*pdwCount)) return S_FALSE;

	//逐个处理，目前只处理空格
	DWORD j = 0;
	for (DWORD i = 0; i <= nOrgnCount; i++)
	{
		if (pcOrgn[i] == ' ')
		{
			pcEscaped[j++] = '%';
			pcEscaped[j++] = '2';
			pcEscaped[j++] = '0';
		}
		else
		{
			pcEscaped[j++] = pcOrgn[i];
		}
	}

	*pdwCount = j;

	return S_OK;
}

unsigned long CEncodingTools::GetCurrentEpochTimestampNoMilsecs()
{
	return ::time(NULL);
}

unsigned long long CEncodingTools::GetCurrentEpochTimestamp()
{
	__timeb64 timeNow = { 0 };
	_ftime64_s(&timeNow);

	return timeNow.time * 1000 + timeNow.millitm;
}

void CEncodingTools::GetCurrentYearMonthString(char* pcYearMonth, int nBufferSize)
{
	time_t tNow = ::time(NULL);
	struct tm tmNow;

	localtime_s(&tmNow, &tNow);
	::strftime(pcYearMonth, nBufferSize, "%Y%m", &tmNow);
}

void CEncodingTools::GetCurrentTimeTZString(char* pcTimeString, int nBufferSize)
{
	time_t tNow = ::time(NULL);
	struct tm tmNow;

	gmtime_s(&tmNow, &tNow);
	::strftime(pcTimeString, nBufferSize, "%Y-%m-%dT%H:%M:%SZ", &tmNow);
}

void CEncodingTools::GetCurrentTimeTString(char* pcTimeString, int nBufferSize)
{
	time_t tNow = ::time(NULL);
	struct tm tmNow;

	localtime_s(&tmNow, &tNow);
	::strftime(pcTimeString, nBufferSize, "%Y-%m-%dT%H:%M:%S", &tmNow);
}

void CEncodingTools::GetCurrentTimeString(char* pcTimeString, int nBufferSize)
{
	time_t tNow = ::time(NULL);
	struct tm tmNow;

	localtime_s(&tmNow, &tNow);
	::strftime(pcTimeString, nBufferSize, "%Y-%m-%d %H:%M:%S", &tmNow);
}

void CEncodingTools::ConvertUTF8ToGB(const char* pcUTF8String, char* pcGBString, int nGBBuffSize)
{
	int nWideSize = MultiByteToWideChar(CP_UTF8, 0, pcUTF8String, -1, NULL, 0);
	WCHAR* pwcUTF8Value = new WCHAR[nWideSize + 1];
	MultiByteToWideChar(CP_UTF8, 0, pcUTF8String, -1, pwcUTF8Value, nWideSize + 1);
	WideCharToMultiByte(CP_ACP, 0, pwcUTF8Value, -1, pcGBString, nGBBuffSize, NULL, NULL);

	delete[] pwcUTF8Value;
}

void CEncodingTools::ConvertUTF8ToUNICODE(const char* pcUTF8String, wchar_t* pcUNString, int nUNBuffSize)
{
	MultiByteToWideChar(CP_UTF8, 0, pcUTF8String, -1, pcUNString, nUNBuffSize);
}

void CEncodingTools::ConvertUNICODEToUTF8(const wchar_t* pcUNString, char* pcUTFString, int nUTF8BuffSize)
{
	WideCharToMultiByte(CP_UTF8, 0, pcUNString, -1, pcUTFString, nUTF8BuffSize, NULL, NULL);
}

void CEncodingTools::ConvertGBToUTF8(const char* pcGBString, char* pcUTF8String, int nUTF8BuffSize)
{
	// GB2312转换成Unicode  
	int nWideSize = MultiByteToWideChar(CP_ACP, 0, pcGBString, -1, NULL, 0);  
	WCHAR* wstrSrc = new WCHAR[nWideSize + 1];  
	MultiByteToWideChar(CP_ACP, 0, pcGBString, -1, wstrSrc, nWideSize + 1);   
	WideCharToMultiByte(CP_UTF8, 0, wstrSrc, -1, pcUTF8String, nUTF8BuffSize, NULL, NULL);  

	delete[] wstrSrc;
}

int CEncodingTools::RequestingURLGET(const char* pcURL, const char* pcParamString, void* pfWriteFuc, void* pUserdata)
{
	CURL *curl;
	CURLcode res;
	char pcURLParams[256] = {0};

	curl = curl_easy_init();
	
	if(pcParamString)
	{
		//处理一些特殊字符，目前只处理空格
		DWORD dwEscSize = 256;
		char pcEscaped[256] = { 0 };
		HRESULT hr = MyUrlEscape(pcParamString, pcEscaped, (UINT*)&dwEscSize);

		if (hr == S_OK)
		{
			sprintf(pcURLParams, "%s?%s", pcURL, pcEscaped);
		}
		else
		{
			sprintf(pcURLParams, "%s?%s", pcURL, pcParamString);
		}
	}
	else
	{
		strcpy(pcURLParams, pcURL);
	}

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, pcURLParams);

		//超时与设置
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);  
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, HTTP_RECV_TIMEOUT);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");  

		//写入设置
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, pfWriteFuc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pUserdata);

		//GZIP相关问题
		curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); 

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
		{
			//TODO
		}

		curl_easy_cleanup(curl);
	}

	return res;
}

int CEncodingTools::RequestingURLPost(const char* pcURL, const char* pcContent, int nContSize, void* pfWriteFuc, void* pUserdata)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;

	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, pcURL);

		//超时与设置
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIME_OUT);  
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, HTTP_RECV_TIMEOUT);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, ""); 

		//写入header和内容
		headers = curl_slist_append(headers, "Content-Type: application/json;charset=utf-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pcContent);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, nContSize);

		//写入设置
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, pfWriteFuc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, pUserdata);

		//GZIP相关问题
		curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); 

		res = curl_easy_perform(curl);

		if(res != CURLE_OK)
		{
			//遇到特定错误，打印post详细信息
			if (res == CURLE_HTTP_RETURNED_ERROR ||
				res == CURLE_WRITE_ERROR ||
				res == CURLE_OPERATION_TIMEDOUT ||
				res == CURLE_HTTP_POST_ERROR ||
				res == CURLE_BAD_CONTENT_ENCODING)
			{
				//TODO: LOG
			}
		}

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}

	return res;
}

//十六进制字符串(不包含0x)转 十进制    
//long hex2dec(char* s){
//    int L = strlen(s);
//    char c;
//    long re = 0;
//    while (c = s++[0]){
//		if (c >= '0' && c <= '9'){
//            c -= 48;
//		}
//		else{
//		    c = c>'Z' ? c - 32 : c;
//		    c -= 'A' - 10;
//		}
//		re += c*pow(16, --L);
//	}
//    return re;
//}
//
//std::string CEncodingTools::unescape(char * str)
//{
//	char* re  = (char *)calloc(strlen(str) + 1, 1);
//	char *_str;
//	char * _re = re;
//	char code[5] = { 0 };
//	WCHAR wc;
//	int n;
//	while (str){
//		_str = strchr(str, '%');
//		if (!_str) break;
//		if (n = _str - str ){
//			memcpy(_re, str, n   );
//			_re += n;
//		}
//		strset(code, 0);
//		if (_str[1] == 'u'){
//			memcpy(code, _str + 2, 4);
//			str = _str + 6;
//			wc = hex2dec(code);
//			memset(code, 0, 5);
//			WideCharToMultiByte(CP_OEMCP, NULL, &wc, -1, (char*)code, 2, NULL, FALSE);
//			memcpy(_re, code, 2);
//			_re += 2;
//		}
//		else{
//			memcpy(code, _str + 1, 2);
//			str = _str + 3;
//			_re[0] = hex2dec(code);
//			_re++;
//		}
//	}
//	strcpy(_re, str);
//	return re;
//}
//
//std::string CEncodingTools::escape(std::string &str)
//{
//	setlocale(LC_CTYPE, "");
//	wchar_t aa[1 << 10];
//	mbstowcs(aa, str.c_str(), str.size());
//	std::wstring wcs(aa);
//	str  = "";
//	for (int i = 0; i<wcs.size(); i++){
//		if (wcs[i]>0xff){
//			char tmp[5];
//			sprintf(tmp, "%x", wcs[i]);
//			str  += "%u";
//			str  += tmp;
//		}
//		else{
//			if ((wcs[i] >= 'a' && wcs[i] <= 'z') || (wcs[i] >= 'A' && wcs[i] <= 'Z') || (wcs[i] >= '0' && wcs[i] <= '9')){
//				char tmp[2];
//				sprintf(tmp, "%c", wcs[i]);
//				str  += tmp;
//			}
//			else{
//				char tmp[3];
//				sprintf(tmp, "%02x", wcs[i]);
//				str  += "%";
//				str  += tmp;
//			}
//		}
//	}
//	return str;
//}
#pragma once

//#include <math.h>
//#include <iostream>
//#include <string>
//#include "windows.h"
//using namespace std;



class CEncodingTools
{
public:
	static bool MyUrlEscape(const char* pcOrgn, char* pcEscaped, unsigned int* pdwCount);
	static void ConvertUTF8ToGB(const char* pcUTF8String, char* pcGBString, int nGBBuffSize);
	static void ConvertUTF8ToUNICODE(const char* pcUTF8String, wchar_t* pcUNString, int nUNBuffSize);
	static void ConvertUNICODEToUTF8(const wchar_t* pcUNString, char* pcUTFString, int nUTF8BuffSize);
	static void ConvertGBToUTF8(const char* pcGBString, char* pcUTF8String, int nUTF8BuffSize);
	static int RequestingURLGET(const char* pcURL, const char* pcParamString, void* pfWriteFuc, void* pUserdata);
	static int RequestingURLPost(const char* pcURL, const char* pcContent, int nContSize, void* pfWriteFuc, void* pUserdata);
	static unsigned long GetCurrentEpochTimestampNoMilsecs();
	static unsigned long long GetCurrentEpochTimestamp();
	static void GetCurrentYearMonthString(char* pcYearMonth, int nBufferSize);
	static void GetCurrentTimeTZString(char* pcTimeString, int nBufferSize);
	static void GetCurrentTimeTString(char* pcTimeString, int nBufferSize);
	static void GetCurrentTimeString(char* pcTimeString, int nBufferSize);

	//static std::string unescape(char * str);
	//static std::string escape(std::string &str);



};


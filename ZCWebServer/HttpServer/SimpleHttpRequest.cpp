#include "SimpleHttpRequest.h"
#include <jemalloc/jemalloc.h>

#define PCPROTOCOL		"HTTP/"
#define PCHOST			"Host:"
#define PCUA			"User-Agent:"
#define PCACCPTFILE		"Accept:"
#define PCACCPTLAN		"Accept-Language:"
#define PCACCPTENCODING	"Accept-Encoding:"
#define PCCONTENTTYPE	"Content-Type:"
#define PCCONTENTLEN	"Content-Length:"
#define PCCONNECTION	"Connection:"
#define PCFILENAME		"Filename:"

CSimpleHttpRequest::CSimpleHttpRequest(char* pcRequestString)
{
	m_nContentLength = 0;
	m_nPaseStatus = 0;
	m_pRequestBodyBuf = NULL;
	m_nRequestBodyLen = 0;

	//���������0��ʾ����
	m_nPaseStatus = paseRequest(pcRequestString);
}

CSimpleHttpRequest::~CSimpleHttpRequest()
{
	if (m_pRequestBodyBuf)
	{
		je_free(m_pRequestBodyBuf);
		m_pRequestBodyBuf = NULL;
	}
}

int CSimpleHttpRequest::GetPaseStatus()
{
	return m_nPaseStatus;
}

string CSimpleHttpRequest::GetMethod()
{
	return m_strMethod;
}

string CSimpleHttpRequest::GetURI()
{
	return m_strURI;
}

string CSimpleHttpRequest::GetParam()
{
	return m_strGetParam;
}

string CSimpleHttpRequest::GetContentType()
{
	return m_strConnectionType;
}

char* CSimpleHttpRequest::GetRequestBodyBuf(char* pBuf, int nSize)
{
	memset(pBuf, 0, nSize);
	if (nSize <= m_nRequestBodyLen)
	{
		memcpy(pBuf, m_pRequestBodyBuf, nSize);
	}
	return pBuf;
}
int CSimpleHttpRequest::GetRequestBodyLen()
{
	return m_nRequestBodyLen;
}

int CSimpleHttpRequest::GetContentLenth()
{
	return m_nContentLength;
}
void CSimpleHttpRequest::SetRequestBody(char* pBuf, int nSize)
{
	if (pBuf == NULL || nSize <= 0)
		return;
	if (m_pRequestBodyBuf)
	{
		je_free(m_pRequestBodyBuf);
		m_pRequestBodyBuf = NULL;
	}
	m_pRequestBodyBuf = (char*)je_malloc(nSize);
	memcpy(m_pRequestBodyBuf, pBuf, nSize);
}
int CSimpleHttpRequest::paseMethodLine(char* pcLine)
{
	//Method URI Protocol
	size_t nLength = strlen(pcLine);
	int nPos = 0;
	int nIndex = 0;

	for (size_t i = 0; i < nLength; i++)
	{
		//˵���Ѿ���ȡ��ǰ����
		if (2 == nIndex)
		{
			m_strHttpProtocolVersion = &pcLine[nPos];
			break;
		}

		//���������Կո�ָ�
		if (pcLine[i] == ' ')
		{
			pcLine[i] = '\0';
			char* pcValue = &pcLine[nPos];

			//�����ȡ�������ݵĴ�
			if (strlen(pcValue) > 0)
			{
				//Method
				if (0 == nIndex)
				{
					m_strMethod = pcValue;	
				}
				else if (1 == nIndex)
				{
					m_strURI = pcValue;
					fixURIRequest(m_strURI);
				}

				nIndex++;
			}

			//�´ζ�ȡ�����ո�
			pcLine[i] = ' ';
			nPos = i + 1;
		}
	}

	return 0;
}

int CSimpleHttpRequest::paseHostLine(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCHOST);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCHOST)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strHost = pcStart;
	}

	return 0;
}

int CSimpleHttpRequest::paseUALine(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCUA);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCUA)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strUserAgent = pcStart;
	}

	return 0;
}

int CSimpleHttpRequest::paseAccept(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCACCPTFILE);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCACCPTFILE)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strAcceptDocType = pcStart;
	}

	return 0;
}

int CSimpleHttpRequest::paseAcceptLan(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCACCPTLAN);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCACCPTLAN)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strAcceptLanguage = pcStart;
	}

	return 0;
}

int CSimpleHttpRequest::paseAcceptEncoding(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCACCPTENCODING);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCACCPTENCODING)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strAcceptEncoding = pcStart;
	}

	return 0;
}

int CSimpleHttpRequest::paseContentType(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCCONTENTTYPE);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCCONTENTTYPE)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strContentType = pcStart;
	}

	return 0;
}

int CSimpleHttpRequest::paseContentLength(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCCONTENTLEN);
	string strContLen = "0";

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCCONTENTLEN)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		strContLen = pcStart;
	}

	m_nContentLength = atoi(strContLen.c_str());

	return 0;
}

int CSimpleHttpRequest::paseConnection(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCCONNECTION);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCCONNECTION)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strConnectionType = pcStart;
	}

	return 0;
}
int CSimpleHttpRequest::paseFilename(char* pcLine)
{
	char* pcPos = strstr(pcLine, PCFILENAME);

	if (pcPos)
	{
		char* pcStart = &pcPos[strlen(PCFILENAME)];

		//�Ƴ�ǰ���ո�
		while (*pcStart == ' ')
		{
			pcStart++;
		}

		m_strFilename = pcStart;
	}

	return 0;
}
void CSimpleHttpRequest::fixURIRequest(string& strOrgnURI)
{
	char pcPath[260] = { 0 };
	strcpy_s(pcPath, strOrgnURI.c_str());

	size_t szLength = strlen(pcPath);
	if (pcPath[szLength - 1] == '/')
	{
		strOrgnURI = strOrgnURI + "index.html";
	}

	//����Ƿ�������������Ƴ�����
	char* pcMarkPos = strchr(pcPath, '?');
	if (pcMarkPos != NULL)
	{
		*pcMarkPos = '\0';
		strOrgnURI = pcPath;

		m_strGetParam = &pcMarkPos[1];
	}
}

//����request
int CSimpleHttpRequest::paseRequest(char* pcRequestString)
{
	//�������
	if (NULL == pcRequestString || strlen(pcRequestString) < 8)
	{
		return -1;
	}

	bool bIsHeaderEnd = false;
	size_t nLength = strlen(pcRequestString);

	/*
	FILE* pFile = fopen("e:\\testt.TXT", "wb");
	if (pFile)
	{
		fwrite(pcRequestString, nLength, 1, pFile);
		fclose(pFile);
	}
	*/

	//Request��header��˫\r\n��β������ֻ�����ı�
	int nPos = 0;
	for (size_t i = 0; i < (nLength - 4); i++)
	{
		//������ɣ������˳�
		if (bIsHeaderEnd)
		{
			break;
		}

		//�ҵ�����
		if (pcRequestString[i] == '\r' && pcRequestString[i + 1] == '\n')
		{
			//��ͨ�м��
			pcRequestString[i] = '\0';

			char* pcLine = &pcRequestString[nPos];

			if (strstr(pcLine, PCPROTOCOL))
			{
				paseMethodLine(pcLine);
			}
			else if (strstr(pcLine, PCHOST))
			{
				paseHostLine(pcLine);
			}
			else if (strstr(pcLine, PCUA))
			{
				paseUALine(pcLine);
			}
			else if (strstr(pcLine, PCACCPTFILE))
			{
				paseAccept(pcLine);
			}
			else if (strstr(pcLine, PCACCPTLAN))
			{
				paseAcceptLan(pcLine);
			}
			else if (strstr(pcLine, PCACCPTENCODING))
			{
				paseAcceptEncoding(pcLine);
			}
			else if (strstr(pcLine, PCCONTENTTYPE))
			{
				paseContentType(pcLine);
			}
			else if (strstr(pcLine, PCCONTENTLEN))
			{
				paseContentLength(pcLine);
			}
			else if (strstr(pcLine, PCCONNECTION))
			{
				paseConnection(pcLine);
			}
			else if (strstr(pcLine, PCFILENAME))
			{
				paseFilename(pcLine);
			}

			//��ԭ
			pcRequestString[i] = '\r';
			nPos = i + 2;

			//�Ƿ�header��β
			if (pcRequestString[i + 2] == '\r' && pcRequestString[i + 3] == '\n')
			{
				//���header�����ı�����Ϊpost����
				m_nRequestBodyLen = m_nContentLength + 1;
				if (m_nRequestBodyLen > 0)
				{
					if (m_pRequestBodyBuf)
						je_free(m_pRequestBodyBuf);

					m_pRequestBodyBuf = (char*)je_malloc(m_nRequestBodyLen);
					memset(m_pRequestBodyBuf, 0, m_nRequestBodyLen);
					memcpy(m_pRequestBodyBuf, &pcRequestString[i + 4], m_nContentLength);
				}

				//������header
				bIsHeaderEnd = true;
			}
		}
	}

	return 0;
}
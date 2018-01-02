#include "TaskPostConfigVerify.h"

#include "SimpleHttpResponse.h"
#include "../JsonRequest/EncodingToolsClass.h"
#include "../ToolFuncs/UrlTransCode.h"

static const char safe[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

std::string encode(const std::string &uri)
{
	std::string ret;
	const unsigned char *ptr = (const unsigned char *)uri.c_str();
	ret.reserve(uri.length());

	for (; *ptr; ++ptr)
	{
		if (!safe[*ptr])
		{
			char buf[5];
			memset(buf, 0, 5);
#ifdef WIN32  
			_snprintf_s(buf, 5, "%%%X", (*ptr));
#else  
			snprintf(buf, 5, "%%%X", (*ptr));
#endif  
			ret.append(buf);
		}
		else if (*ptr == ' ')
		{
			ret += '+';
		}
		else{
			ret += *ptr;
		}
	}
	return ret;
}

CTaskPostConfigVerify::CTaskPostConfigVerify(CStaticHttpServer* pServer, CSocketBase* pClientSocket, string& strReqBody)
{
	m_pServer = pServer;
	m_pClientSocket = pClientSocket;

	m_strReqBody = strReqBody;
}

CTaskPostConfigVerify::~CTaskPostConfigVerify()
{
	//�Ͽ�ͬ�ͻ�������
	if (m_pClientSocket)
	{
		m_pClientSocket->Disconnect();

		delete m_pClientSocket;
		m_pClientSocket = NULL;
	}
}

void CTaskPostConfigVerify::prepareTask()
{

}

void CTaskPostConfigVerify::executeTask()
{
	//���Ľ���
	CUrlTransCode cCode;
	string strParam = cCode.UrlUTF8Decode(m_strReqBody.c_str());

	string strUserName;
	string strPass;

	//����û��� user=11&pass=admin
	size_t szPos = strParam.find("user");
	if (szPos != string::npos)
	{
		size_t szEnd = strParam.find('&', szPos);
		if (szEnd == string::npos)
		{
			strUserName = strParam.substr(szPos + 5);
		}
		else
		{
			strUserName = strParam.substr(szPos + 5, szEnd - szPos - 5);
		}
	}

	//�������
	szPos = strParam.find("pass");
	if (szPos != string::npos)
	{
		size_t szEnd = strParam.find('&', szPos);
		if (szEnd == string::npos)
		{
			strPass = strParam.substr(szPos + 5);
		}
		else
		{
			strPass = strParam.substr(szPos + 5, szEnd - szPos - 5);
		}
	}

	bool bRet = CMainModel::Instance()->CheckUserPassword(strUserName, strPass);
	if (!bRet)
	{
		sendForbidden();
		return;
	}

	redirectConfig(strUserName,CMainModel::Instance()->GetUserTypeByName(strUserName));
	CDBHelper::Instance()->Log(strUserName.c_str(), "�û���½", "");
	return;
}

void CTaskPostConfigVerify::cleanupTask()
{
}

void CTaskPostConfigVerify::sendForbidden()
{
	//forbiden
	const char* pcMSG = "�������";
	char pcUTF[256] = { 0 };
	CEncodingTools::ConvertGBToUTF8(pcMSG, pcUTF, 256);

	CSimpleHttpResponse respBR(403);
	respBR.SetContentLength(strlen(pcUTF));

	//�Ƴ�cookie
	//respBR.AddCookie(string("login=0; path=/production; "), 1, true);
	//respBR.AddCookie(string("type=0; path=/production; "), 1, false);
	respBR.AddCookie(string("login=0; path=/; "), 1, true);
	respBR.AddCookie(string("type=0; path=/; "), 1, false);

	string strOutput = respBR.BuildResponseHeader();
	m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());
	m_pClientSocket->SendBuffer(pcUTF, strlen(pcUTF));
}

void CTaskPostConfigVerify::redirectConfig(string& strUserName, int nUserType)
{
	//redirect
	CSimpleHttpResponse respBR(302);

	//���û���ת��UTF-8�ַ����������������ַ����룬�Է�����
	char chUTFUserName[128] = { 0 };
	CEncodingTools::ConvertGBToUTF8(strUserName.c_str(), chUTFUserName, 128);
	string strEncodeUserName = encode(string(chUTFUserName));

	//���û���ƴ�ӵ����ص�URL�У��Ա������ʾ
	//string strLocation("/production/main.html?user=");
	string strLocation("/main.html?user=");
	strLocation = strLocation + strEncodeUserName;
	respBR.SetLocation(strLocation);

	//respBR.AddCookie(string("login=1; path=/production; "), 60 * 30, true);		    //cookie��Сʱ����Ч	
	respBR.AddCookie(string("login=1; path=/; "), 60 * 30, true);		    //cookie��Сʱ����Ч	
	if (nUserType == 1)
	{
		//respBR.AddCookie(string("type=1; path=/production; "), 60 * 30, false);		//cookie��Сʱ����Ч
		respBR.AddCookie(string("type=1; path=/; "), 60 * 30, false);		//cookie��Сʱ����Ч
	}
	else
	{
		//respBR.AddCookie(string("type=0; path=/production; "), 60 * 30, false);		//cookie��Сʱ����Ч
		respBR.AddCookie(string("type=0; path=/; "), 60 * 30, false);		//cookie��Сʱ����Ч
	}

	string strOutput = respBR.BuildResponseHeader();
	m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());
}

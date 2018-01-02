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
	//断开同客户的连接
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
	//中文解码
	CUrlTransCode cCode;
	string strParam = cCode.UrlUTF8Decode(m_strReqBody.c_str());

	string strUserName;
	string strPass;

	//拆分用户名 user=11&pass=admin
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

	//拆分密码
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
	CDBHelper::Instance()->Log(strUserName.c_str(), "用户登陆", "");
	return;
}

void CTaskPostConfigVerify::cleanupTask()
{
}

void CTaskPostConfigVerify::sendForbidden()
{
	//forbiden
	const char* pcMSG = "密码错误";
	char pcUTF[256] = { 0 };
	CEncodingTools::ConvertGBToUTF8(pcMSG, pcUTF, 256);

	CSimpleHttpResponse respBR(403);
	respBR.SetContentLength(strlen(pcUTF));

	//移除cookie
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

	//将用户名转成UTF-8字符集，并进行特殊字符编码，以防乱码
	char chUTFUserName[128] = { 0 };
	CEncodingTools::ConvertGBToUTF8(strUserName.c_str(), chUTFUserName, 128);
	string strEncodeUserName = encode(string(chUTFUserName));

	//将用户名拼接到返回的URL中，以便界面显示
	//string strLocation("/production/main.html?user=");
	string strLocation("/main.html?user=");
	strLocation = strLocation + strEncodeUserName;
	respBR.SetLocation(strLocation);

	//respBR.AddCookie(string("login=1; path=/production; "), 60 * 30, true);		    //cookie半小时内有效	
	respBR.AddCookie(string("login=1; path=/; "), 60 * 30, true);		    //cookie半小时内有效	
	if (nUserType == 1)
	{
		//respBR.AddCookie(string("type=1; path=/production; "), 60 * 30, false);		//cookie半小时内有效
		respBR.AddCookie(string("type=1; path=/; "), 60 * 30, false);		//cookie半小时内有效
	}
	else
	{
		//respBR.AddCookie(string("type=0; path=/production; "), 60 * 30, false);		//cookie半小时内有效
		respBR.AddCookie(string("type=0; path=/; "), 60 * 30, false);		//cookie半小时内有效
	}

	string strOutput = respBR.BuildResponseHeader();
	m_pClientSocket->SendBuffer((char*)strOutput.c_str(), strOutput.length());
}

#include "IJSONRequestHandler.h"
#include "EncodingToolsClass.h"

size_t IJSONRequestHandler::json_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	if(ptr && userdata)
	{
		IJSONRequestHandler* pThis = (IJSONRequestHandler*)userdata;

		int nSize = size*nmemb;

		pThis->m_sbRecvBuff.sputn(ptr, nSize);

		return nSize;
	}
	else
	{
		return 0;
	}
}

IJSONRequestHandler::IJSONRequestHandler(void) : m_szResultCount(0), m_szResultPos(0)
{}

int IJSONRequestHandler::DoRequest(const char* pcURL, bool bIsPost, const char* pcContentOrParam, int nContSize)
{
	//清空上次返回结果
	m_sbRecvBuff.consume(m_sbRecvBuff.size() + 1);

	char pcStopPos = '\0';
	char pcFullURL[256] = {0};
	sprintf(pcFullURL, "%s", pcURL);

	int nRet = 0;

	if(bIsPost)
	{
		nRet = CEncodingTools::RequestingURLPost(pcFullURL, pcContentOrParam, nContSize, json_write_callback, this);
	}
	else
	{
		nRet = CEncodingTools::RequestingURLGET(pcFullURL, pcContentOrParam, json_write_callback, this);
	}

	if(nRet != 0)
	{
		return ERR_HTTP_REQUEST;
	}

	m_sbRecvBuff.sputc(pcStopPos);
	m_doc.Parse(boost::asio::buffer_cast<const char*>(m_sbRecvBuff.data()));

	if(!m_doc.IsObject())
	{
		return ERR_BAD_RESPONSE;
	}

	return 0;
}
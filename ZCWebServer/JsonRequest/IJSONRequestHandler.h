#pragma once

#include <string>
using std::string;

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <boost/asio/streambuf.hpp>
using boost::asio::streambuf;

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
using namespace rapidjson;

#include "EncodingToolsClass.h"

#define CUR_MAX_VIDEO_CHANS			256			//���ͨ����

#define ERR_CODE_WRONG_PARAM		100			//��������
#define ERR_HTTP_REQUEST			101			//ͨ�Ŵ���
#define ERR_BAD_RESPONSE			102			//����������
#define ERR_EXCUTE_FAILED			103			//ִ��ʧ��
#define ERR_LIST_INDEX_OVER			104			//�±�Խ��
#define	ERR_INTERFACE_DEPRECATED	105			//�ӿ�������

class IJSONRequestHandler
{
public:
	IJSONRequestHandler(void);
	virtual ~IJSONRequestHandler(void) {}

protected:

	streambuf m_sbRecvBuff;
	static size_t json_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

	Document m_doc;
	SizeType m_szResultCount;
	SizeType m_szResultPos;

public:
	int DoRequest(const char* pcURL, bool bIsPost, const char* pcContentOrParam, int nContSize);
};

typedef IJSONRequestHandler* (*PF_CREATEINSTANCE)();


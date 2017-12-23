#pragma once

typedef enum _ENU_SERVER_THREAD_STATUS {
	SERVER_THREAD_SUSPEND = 0,	//�̹߳���
	SERVER_THREAD_RUNIDLE,		//�߳����У�������
	SERVER_THREAD_BUSY,			//�߳�����ִ������
	SERVER_THREAD_ENDED,		//�߳���ֹͣ

} ENU_SERVER_THREAD_STATUS;

//ǰ������
class CServerThreadPool;

class CServerThread
{
public:
	CServerThread(CServerThreadPool* pPool);
	~CServerThread();

private:
	int m_nStatus;
	unsigned int m_uiThreadID;
	void* m_hThreadHandle;

	CServerThreadPool* m_pPool;

public:
	static unsigned int __stdcall ThreadFunction(void *pParam);		//�̺߳���

public:
	void SetThreadInfo(void* hThreadHandle, unsigned int uiThreadID);
	void MarkThreadStatus(int nStatus);

	void* GetThreadHandle();
};


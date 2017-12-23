#include "ServicePRMonitor.h"

#include "../HttpServer/StaticHttpServer.h"
#include "../SystemCounter/SystemInfoCounter.h"
#include "../JsonRequest/IJSONRequestHandler.h"

#include "../MainModel/MainModel.h"

#define JSON_TEMPLATE "{\"appname\":\"\",\"servername\":\"\",\"logtime\":0,\"uptime\":0,\"cpuusage\":%s,\"threadcount\":0,\"handlecount\":0,\"workset\":0,\"worksetpeak\":0,\"worksetprivate\":0,\"timestamp\":\"\"}"

//void __stdcall ReportProcesStatusCallback(PROCESS_STATUS_INFO* pInfo, void* pUser)
//{
////获取设置
//CStaticHttpServer* pServer = (CStaticHttpServer*)pUser;
//string strEleServer, strServerName, strTypeName;
//int nElePort = 9200;

//pServer->GetConfigOthers(strEleServer, nElePort, strServerName, strTypeName);
//
////拼接ElasticSearch服务器地址
//char pcYearMonth[16] = { 0 };
//CEncodingTools::GetCurrentYearMonthString(pcYearMonth, 16);

//char pcEleServerURI[256] = { 0 };
//sprintf(pcEleServerURI, "http://%s:%d/zonee-%s/%s/", strEleServer.c_str(), nElePort, pcYearMonth, strTypeName.c_str());

////首先填充小数位
//char pcTemp[512] = { 0 };
//sprintf(pcTemp, JSON_TEMPLATE, pInfo->pcCPUUsage);

//Document doc;
//doc.Parse(pcTemp);

//doc["appname"].SetString(pInfo->pcProcessName, doc.GetAllocator());
//doc["servername"].SetString(strServerName.c_str(), doc.GetAllocator());
//doc["logtime"].SetUint64(CEncodingTools::GetCurrentEpochTimestamp());
//doc["uptime"].SetUint(pInfo->unEclapsedSecs);
//doc["threadcount"].SetUint(pInfo->unThreadCount);
//doc["handlecount"].SetUint(pInfo->unHandleCount);
//doc["workset"].SetUint(pInfo->unWorksetKB);
//doc["worksetpeak"].SetUint(pInfo->unWorksetPeakKB);
//doc["worksetprivate"].SetUint(pInfo->unWorksetPrivateKB);

//char pcTimeStamp[64] = { 0 };
//CEncodingTools::GetCurrentTimeTZString(pcTimeStamp, 64);
//doc["timestamp"].SetString(pcTimeStamp, doc.GetAllocator());

//StringBuffer outBuff;
//Writer<StringBuffer> jsonWriter(outBuff);
//doc.Accept(jsonWriter);

////发送请求并解析，不在意失败
//IJSONRequestHandler json;
//json.DoRequest(pcEleServerURI, TRUE, outBuff.GetString(), outBuff.GetSize());
//}

CServicePRMonitor::CServicePRMonitor(PWSTR pszServiceName,
	BOOL fCanStop,
	BOOL fCanShutdown,
	BOOL fCanPauseContinue)
	: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
	m_fStopping = FALSE;

	// Create a manual-reset event that is not signaled at first to indicate 
	// the stopped signal of the service.
	m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hStoppedEvent == NULL)
	{
		throw GetLastError();
	}
}


CServicePRMonitor::~CServicePRMonitor(void)
{
	if (m_hStoppedEvent)
	{
		CloseHandle(m_hStoppedEvent);
		m_hStoppedEvent = NULL;
	}
}

//   FUNCTION: CSampleService::OnStart(DWORD, LPWSTR *)
//
//   PURPOSE: The function is executed when a Start command is sent to the 
//   service by the SCM or when the operating system starts (for a service 
//   that starts automatically). It specifies actions to take when the 
//   service starts. In this code sample, OnStart logs a service-start 
//   message to the Application log, and queues the main service function for 
//   execution in a thread pool worker thread.
//
//   PARAMETERS:
//   * dwArgc   - number of command line arguments
//   * lpszArgv - array of command line arguments
//
//   NOTE: A service application is designed to be long running. Therefore, 
//   it usually polls or monitors something in the system. The monitoring is 
//   set up in the OnStart method. However, OnStart does not actually do the 
//   monitoring. The OnStart method must return to the operating system after 
//   the service's operation has begun. It must not loop forever or block. To 
//   set up a simple monitoring mechanism, one general solution is to create 
//   a timer in OnStart. The timer would then raise events in your code 
//   periodically, at which time your service could do its monitoring. The 
//   other solution is to spawn a new thread to perform the main service 
//   functions, which is demonstrated in this code sample.
//
void CServicePRMonitor::OnStart(DWORD dwArgc, PWSTR *pszArgv)
{
	// Log a service start message to the Application log.
	WriteEventLogEntry(L"客户端网页服务启动",
		EVENTLOG_INFORMATION_TYPE);

	// Queue the main service function for execution in a worker thread.
	CThreadPool::QueueUserWorkItem(&CServicePRMonitor::ServiceWorkerThread, this);
}

/**
 * @brief 服务入口程序
 *
 * It runs on a thread pool worker thread.
 */
void CServicePRMonitor::ServiceWorkerThread(void)
{
	CStaticHttpServer server;

	CMainModel::Instance();

	//获取web服务器端口配置
	int nHttpPort = server.GetConfigHttpPort();
	//启动web服务
	int nRet = server.StartHttpServer(nHttpPort);

	//监听是否成功，写入日志，此处失败不影响程序监视
	if (nRet == -1)
	{
		WCHAR pcLog[128] = { 0 };
		wsprintfW(pcLog, L"客户端网页服务在 %d 端口尝试监听失败，可能端口被占用，请重新指定HTTP端口", nHttpPort);
		WriteEventLogEntry(pcLog, EVENTLOG_ERROR_TYPE);
	}
	else
	{
		WCHAR pcLog[128] = { 0 };
		wsprintfW(pcLog, L"客户端网页服务在 %d 端口监听成功，可以通过HTTP访问", nHttpPort);
		WriteEventLogEntry(pcLog, EVENTLOG_INFORMATION_TYPE);
	}




	/*去除定期上报*/
	////检查需要监视的进程
	//vector<wstring> vecApps;
	//bool bRet = server.GetAppsStrings(vecApps);

	////监控实例
	//vector<CSystemInfoCounter*> vecMonitors;

	////开始监控程序状态
	//if (bRet)
	//{
	//	for (vector<wstring>::iterator it = vecApps.begin(); it != vecApps.end(); ++it)
	//	{
	//		CSystemInfoCounter* pCounter = new CSystemInfoCounter;

	//		//开始60秒监视
	//		pCounter->m_ctProcess.StartMonitorProcess(*it, ReportProcesStatusCallback, &server, 60);

	//		//计入数组
	//		vecMonitors.push_back(pCounter);
	//	}
	//}

	// Periodically check if the service is stopping.
	while (!m_fStopping)
	{
		//所有操作均在线程池里进行，此处空转即可
		Sleep(100);
	}

	server.StopHttpServer();

	//通知SCM服务未挂，此时处理可能比较长
	SetServiceStatus(SERVICE_STOP_PENDING);

	////停止所有计数器
	//for (vector<CSystemInfoCounter*>::iterator it = vecMonitors.begin(); it != vecMonitors.end(); ++it)
	//{
	//	(*it)->m_ctProcess.StopMonitorProcess();
	//	delete (*it);

	//	SetServiceStatus(SERVICE_STOP_PENDING);
	//}

	//vecMonitors.clear();

	// Signal the stopped event.
	SetEvent(m_hStoppedEvent);
}


//
//   FUNCTION: CSampleService::OnStop(void)
//
//   PURPOSE: The function is executed when a Stop command is sent to the 
//   service by SCM. It specifies actions to take when a service stops 
//   running. In this code sample, OnStop logs a service-stop message to the 
//   Application log, and waits for the finish of the main service function.
//
//   COMMENTS:
//   Be sure to periodically call ReportServiceStatus() with 
//   SERVICE_STOP_PENDING if the procedure is going to take long time. 
//
void CServicePRMonitor::OnStop()
{
	// Log a service stop message to the Application log.
	WriteEventLogEntry(L"客户端网页服务器停止",
		EVENTLOG_INFORMATION_TYPE);

	// Indicate that the service is stopping and wait for the finish of the 
	// main service function (ServiceWorkerThread).
	m_fStopping = TRUE;
	if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
	{
		throw GetLastError();
	}
}
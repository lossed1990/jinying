#pragma once

#include "ServiceBase.h"

class CServicePRMonitor : public CServiceBase
{
public:
	CServicePRMonitor(PWSTR pszServiceName,
		BOOL fCanStop = TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue = FALSE);

	virtual ~CServicePRMonitor();

	void doRunning() { ServiceWorkerThread(); }

protected:

	virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);
	virtual void OnStop();

	void ServiceWorkerThread(void);

private:

	BOOL m_fStopping;
	HANDLE m_hStoppedEvent;
};


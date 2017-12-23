/****************************** Module Header ******************************\
* Module Name:  ServiceBase.h
* Project:      CppWindowsService
* Copyright (c) Microsoft Corporation.
* 
* Provides a base class for a service that will exist as part of a service 
* application. CServiceBase must be derived from when creating a new service 
* class.
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <memory>

class CThreadPool
{
public:

	template <typename T>
	static void QueueUserWorkItem(void (T::*function)(void),
		T *object, ULONG flags = WT_EXECUTELONGFUNCTION)
	{
		typedef std::pair<void (T::*)(), T *> CallbackType;
		std::auto_ptr<CallbackType> p(new CallbackType(function, object));

		if (::QueueUserWorkItem(ThreadProc<T>, p.get(), flags))
		{
			// The ThreadProc now has the responsibility of deleting the pair.
			p.release();
		}
		else
		{
			throw GetLastError();
		}
	}

private:

	template <typename T>
	static DWORD WINAPI ThreadProc(PVOID context)
	{
		typedef std::pair<void (T::*)(), T *> CallbackType;

		std::auto_ptr<CallbackType> p(static_cast<CallbackType *>(context));

		(p->second->*p->first)();
		return 0;
	}
};

class CServiceBase
{
public:

	//   FUNCTION: InstallService
	//
	//   PURPOSE: Install the current application as a service to the local 
	//   service control manager database.
	//
	//   PARAMETERS:
	//   * pszServiceName - the name of the service to be installed
	//   * pszDisplayName - the display name of the service
	//   * dwStartType - the service start option. This parameter can be one of 
	//     the following values: SERVICE_AUTO_START, SERVICE_BOOT_START, 
	//     SERVICE_DEMAND_START, SERVICE_DISABLED, SERVICE_SYSTEM_START.
	//   * pszDependencies - a pointer to a double null-terminated array of null-
	//     separated names of services or load ordering groups that the system 
	//     must start before this service.
	//   * pszAccount - the name of the account under which the service runs.
	//   * pszPassword - the password to the account name.
	//
	//   NOTE: If the function fails to install the service, it prints the error 
	//   in the standard output stream for users to diagnose the problem.
	//
	static void InstallService(PWSTR pszServiceName,
		PWSTR pszDisplayName,
		DWORD dwStartType,
		PWSTR pszDependencies,
		PWSTR pszAccount,
		PWSTR pszPassword,
		PWSTR pszDescription);

	//
	//   FUNCTION: UninstallService
	//
	//   PURPOSE: Stop and remove the service from the local service control 
	//   manager database.
	//
	//   PARAMETERS: 
	//   * pszServiceName - the name of the service to be removed.
	//
	//   NOTE: If the function fails to uninstall the service, it prints the 
	//   error in the standard output stream for users to diagnose the problem.
	//
	static void UninstallService(PWSTR pszServiceName);

    // Register the executable for a service with the Service Control Manager 
    // (SCM). After you call Run(ServiceBase), the SCM issues a Start command, 
    // which results in a call to the OnStart method in the service. This 
    // method blocks until the service has stopped.
    static BOOL Run(CServiceBase &service);

    // Service object constructor. The optional parameters (fCanStop, 
    // fCanShutdown and fCanPauseContinue) allow you to specify whether the 
    // service can be stopped, paused and continued, or be notified when 
    // system shutdown occurs.
    CServiceBase(PWSTR pszServiceName, 
        BOOL fCanStop = TRUE, 
        BOOL fCanShutdown = TRUE, 
        BOOL fCanPauseContinue = FALSE);

    // Service object destructor. 
    virtual ~CServiceBase(void);

    // Stop the service.
    void Stop();

protected:

    // When implemented in a derived class, executes when a Start command is 
    // sent to the service by the SCM or when the operating system starts 
    // (for a service that starts automatically). Specifies actions to take 
    // when the service starts.
    virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);

    // When implemented in a derived class, executes when a Stop command is 
    // sent to the service by the SCM. Specifies actions to take when a 
    // service stops running.
    virtual void OnStop();

    // When implemented in a derived class, executes when a Pause command is 
    // sent to the service by the SCM. Specifies actions to take when a 
    // service pauses.
    virtual void OnPause();

    // When implemented in a derived class, OnContinue runs when a Continue 
    // command is sent to the service by the SCM. Specifies actions to take 
    // when a service resumes normal functioning after being paused.
    virtual void OnContinue();

    // When implemented in a derived class, executes when the system is 
    // shutting down. Specifies what should occur immediately prior to the 
    // system shutting down.
    virtual void OnShutdown();

    // Set the service status and report the status to the SCM.
    void SetServiceStatus(DWORD dwCurrentState, 
        DWORD dwWin32ExitCode = NO_ERROR, 
        DWORD dwWaitHint = 0);

    // Log a message to the Application event log.
    void WriteEventLogEntry(PWSTR pszMessage, WORD wType);

    // Log an error message to the Application event log.
    void WriteErrorLogEntry(PWSTR pszFunction, 
        DWORD dwError = GetLastError());

private:

    // Entry point for the service. It registers the handler function for the 
    // service and starts the service.
    static void WINAPI ServiceMain(DWORD dwArgc, LPWSTR *lpszArgv);

    // The function is called by the SCM whenever a control code is sent to 
    // the service.
    static void WINAPI ServiceCtrlHandler(DWORD dwCtrl);

    // Start the service.
    void Start(DWORD dwArgc, PWSTR *pszArgv);
    
    // Pause the service.
    void Pause();

    // Resume the service after being paused.
    void Continue();

    // Execute when the system is shutting down.
    void Shutdown();

    // The singleton service instance.
    static CServiceBase *s_service;

    // The name of the service
    PWSTR m_name;

    // The status of the service
    SERVICE_STATUS m_status;

    // The service status handle
    SERVICE_STATUS_HANDLE m_statusHandle;
};
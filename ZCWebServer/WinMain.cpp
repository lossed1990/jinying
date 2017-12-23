#include "public.h"

#include "Services/ServicePRMonitor.h"

// Internal name of the service
#define SERVICE_NAME             L"ZCWebServer"
// Displayed name of the service
#define SERVICE_DISPLAY_NAME     L"GXL Web Service"
// Displayed name of the service
#define SERVICE_DESCRIPTIONS     L"web服务软件"
// Service start options.
#define SERVICE_START_TYPE       SERVICE_AUTO_START
// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     L""
// The name of the account under which the service should run
#define SERVICE_ACCOUNT			NULL
// The password to the service account name
#define SERVICE_PASSWORD         NULL

//使用服务方式运行程序
int main(int argc, char *argv[])
{
	if ((argc > 1) && ((*argv[1] == '-' || (*argv[1] == '/'))))
	{
		if (strcmp("install", argv[1] + 1) == 0)
		{
			// Install the service when the command is 
			// "-install" or "/install".
			CServiceBase::InstallService(
				SERVICE_NAME,               // Name of service
				SERVICE_DISPLAY_NAME,       // Name to display
				SERVICE_START_TYPE,         // Service start type
				SERVICE_DEPENDENCIES,       // Dependencies
				SERVICE_ACCOUNT,            // Service running account
				SERVICE_PASSWORD,            // Password of the account
				SERVICE_DESCRIPTIONS
			);
		}
		else if (strcmp("remove", argv[1] + 1) == 0)
		{
			// Uninstall the service when the command is 
			// "-remove" or "/remove".
			CServiceBase::UninstallService(SERVICE_NAME);
		}
	}
	else
	{
		printf("Parameters:\n");
		printf(" -install  to install the service.\n");
		printf(" -remove   to remove the service.\n");

		CServicePRMonitor service(SERVICE_NAME);
		//service.doRunning();

#ifdef _DEBUG
		service.doRunning();
#else
		if (!CServiceBase::Run(service))
		{
			DWORD dwError;
			printf("Service failed to run w/err 0x%08lx or %ld \n", dwError = GetLastError(), dwError);
		}
#endif
	}

	return 0;
}
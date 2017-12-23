#pragma once

#include <vector>
#include <string>
using std::vector;
using std::wstring;

#include "ProcessorCounter.h"
#include "MemoryCounter.h"
#include "ProcessCounter.h"

class CSystemInfoCounter
{
public:
	CSystemInfoCounter();
	~CSystemInfoCounter();

public:
	static bool RefreshWindowsSystemCounters();
	static bool IsProcessRunning(wstring& wstrProcessName);

public:
	vector<wstring> m_vecSysObjects;		//系统可计数对象

	CProcessorCounter m_ctProcessor;
	CMemoryCounter m_ctMemory;
	CProcessCounter m_ctProcess;
};


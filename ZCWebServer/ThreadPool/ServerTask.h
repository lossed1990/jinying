#pragma once

class CServerTask
{
public:
	CServerTask();
	virtual ~CServerTask();

public:
	virtual void prepareTask(){};
	virtual void executeTask() = 0;
	virtual void cleanupTask(){};
};


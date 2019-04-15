#pragma once
#include "MyThread.h"
#include "ObjManager.h"

class WorkerThread : public MyThread
{
private:
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();
public:

	explicit WorkerThread();
	virtual ~WorkerThread();

	virtual void Init();
	virtual void Proc();

	void error_display(const char *msg, int err_no);


};


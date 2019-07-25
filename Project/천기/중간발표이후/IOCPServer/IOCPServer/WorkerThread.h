#pragma once
#include "MyThread.h"
#include "ObjManager.h"
#include <mutex>
class ObjManager;
class WorkerThread : public MyThread
{
private:
	std::mutex WorkerLock;
	ObjManager* objectManager ;
public:

	explicit WorkerThread();
	virtual ~WorkerThread();

	virtual void Init();
	virtual void Proc();

	void error_display(const char *msg, int err_no);


};


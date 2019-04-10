#pragma once
#include "MyThread.h"

class WorkerThread : public MyThread
{
public:
	explicit WorkerThread();
	virtual ~WorkerThread();

	virtual void Init();
	virtual void Proc();
private:

};


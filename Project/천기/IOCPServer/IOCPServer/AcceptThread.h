#pragma once
#include "MyThread.h"

class AcceptThread : public MyThread
{
public:
	explicit AcceptThread();
	virtual ~AcceptThread();

	virtual void Init();
	virtual void Proc();
private:

};


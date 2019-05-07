#pragma once
#include "MyThread.h"
#include "ObjManager.h"

class ObjManager;
class AcceptThread : public MyThread
{
private:
	ObjManager* objectManager;

	SOCKET listenSocket;
	SOCKADDR_IN serverAddr;
	SOCKET clientSocket;

	int USER_NUM = 0;
	bool first = true;
public:

	explicit AcceptThread();
	virtual ~AcceptThread();

	virtual void Init();
	virtual void Proc();

};


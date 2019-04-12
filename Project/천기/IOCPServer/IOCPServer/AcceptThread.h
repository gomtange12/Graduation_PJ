#pragma once
#include "MyThread.h"
#include "ObjManager.h"

class AcceptThread : public MyThread
{
private:
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();

	SOCKET listenSocket;
	SOCKADDR_IN serverAddr;
	SOCKET clientSocket;

	int USER_NUM = 0;
public:

	explicit AcceptThread();
	virtual ~AcceptThread();

	virtual void Init();
	virtual void Proc();

};


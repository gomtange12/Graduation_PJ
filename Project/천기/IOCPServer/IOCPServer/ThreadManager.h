#pragma once

#include "ObjManager.h"
class ThreadManager : public SingleTone<ThreadManager>
{
private:
	std::vector <std::thread> threads;
	
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();
	
	SOCKET listenSocket;
	SOCKADDR_IN serverAddr;
	SOCKET clientSocket;
	//std::vector <std::shared_ptr<std::thread>> threads;
public:
	int USER_NUM = 0;
	const int NUM_THREADS = 4;
public:
	explicit ThreadManager();
	virtual ~ThreadManager();

	void CreateThreads();
	void JoinThreads();
	void Worker_thread();
	void Accept_thread();
	void OverlappedRecv(int id);
	void error_display(const char *msg, int err_no);
};
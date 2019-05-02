#pragma once
#include "ObjManager.h"
#include "WorkerThread.h"
#include "AcceptThread.h"

class ThreadManager : public SingleTone<ThreadManager>
{
private:
	std::vector <MyThread*> threads;
	ObjManager* objectManager = OBJMANAGER->GetObjectManager();
	//std::vector <std::shared_ptr<std::thread>> threads;
	const int NUM_THREADS = 1;
public:
	explicit ThreadManager();
	virtual ~ThreadManager();
public:

	void CreateThreads();
	void AddThread(MyThread* myth);
	void JoinThreads();
	void OverlappedRecv(unsigned int id);
	
};
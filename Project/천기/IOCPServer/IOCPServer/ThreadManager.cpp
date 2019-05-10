#include "pch.h"
#include "ThreadManager.h"
#include "Player.h"

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
}

void ThreadManager::CreateThreads()
{
	/*threads.emplace_back(std::thread([&]() { Accept_thread(); }));

	for (int i = 0; i < NUM_THREADS; ++i)
		threads.emplace_back(std::thread([&]() { Worker_thread(); }));
	std::cout << "쓰레드 생성\n";*/
	std::cout << " :: Thereads ::\n";

	AddThread(new AcceptThread);  std::cout << "  AcceptTheread\n";

	for (int i = 0; i < NUM_THREADS; ++i) {
		AddThread(new WorkerThread);
		
	}
	std::cout << "  WorkerThreads\n";
	AddThread(new TimerThread);  //std::cout << "  TimerTheread\n";
}
void ThreadManager::AddThread(MyThread* myth)
{
	myth->Init();
	threads.emplace_back(myth);
}
void ThreadManager::JoinThreads() {
	for (auto &th : threads)
		th->Join();
}



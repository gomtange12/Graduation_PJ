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

	AddThread(new AcceptThread);
	for (int i = 0; i < NUM_THREADS; ++i)
		AddThread(new WorkerThread);
	std::cout << "쓰레드 생성\n";
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

void ThreadManager::OverlappedRecv(unsigned int id)
{
	DWORD flags = 0;
	ZeroMemory(&objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaOver, sizeof(WSAOVERLAPPED));
	WSARecv(objectManager->GetPlayer(id)->m_socket, &objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaBuf, 
		1, NULL, &flags, &(objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaOver), 0);
}

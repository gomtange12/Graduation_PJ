#pragma once
#include "pch.h"
#include "Player.h"

class ThreadManager : public SingleTone<ThreadManager>
{
public:
	SOCKET listenSocket;
	SOCKADDR_IN serverAddr;
	SOCKET clientSocket;
	std::vector <std::thread> threads;
	int USER_NUM = 0;
	const int NUM_THREADS = 4;
public:
	explicit ThreadManager();
	virtual ~ThreadManager();
public:
	void CreateThreads();
	void JoinThreads();
	void Worker_thread();
	void Accept_thread();
	void OverlappedRecv(int id);
private:

};
#include "pch.h"
#include "TimerThread.h"
#include "IOCPServer.h"
#include "Define.h"
TimerThread::TimerThread()
{
}

TimerThread::~TimerThread()
{
}
void TimerThread::Init()
{
	mythread = std::thread([&]() { TimerThread::Proc(); });
}
void TimerThread::Proc()
{
	while (true) 
	{
		std::this_thread::sleep_for(std::chrono::duration<float>(0.01f));
		while (false == timerQueue.empty())
		{
			if (timerQueue.top().time >= GetTickCount()) break;
			timerLock.lock();
			Event ev = timerQueue.top();
			timerQueue.pop();
			timerLock.unlock();
			stOverEx *ex = new stOverEx;
			ex->m_todo = ev.type;
			//ex->id = ev.id;
			//ex->time = GetTickCount();
			PostQueuedCompletionStatus(IOCPSERVER->GetIocp(), 1, NULL, &ex->m_wsaOver);
		}
	}
}
void TimerThread::AddTimer(int id, int type, unsigned int time) 
{
	timerLock.lock();
	timerQueue.push(Event{ time, type, id });
	timerLock.unlock();
}
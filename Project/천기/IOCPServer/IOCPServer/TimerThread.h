#pragma once
#include "MyThread.h"
#include <queue>
#include <mutex>

struct Event
{
	unsigned int time;
	int type;
	int id;
};
class Comp
{
public:
	Comp() {};
	bool operator() (Event lhs, Event rhs) const {
		return (lhs.time > rhs.time);
	}

};

class TimerThread : public MyThread
{
private:
	std::mutex timerLock;
	std::priority_queue <Event, std::vector<Event>, Comp> timerQueue;
public:
	explicit TimerThread();
	virtual ~TimerThread();

	virtual void Init();
	virtual void Proc();

	void AddTimer(int id, int type, unsigned int time);
};


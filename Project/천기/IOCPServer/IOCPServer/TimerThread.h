#pragma once
#include "MyThread.h"
#include <queue>
#include <mutex>

struct Event
{
	u_int time;
	int type;
	int id;
	//GameObject* target;

};
class Comp
{
public:
	Comp() {};
	bool operator() (const Event& lhs, const Event& rhs) const {
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


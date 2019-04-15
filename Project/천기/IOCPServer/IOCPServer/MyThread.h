#pragma once

class MyThread
{
private:

public:
	explicit MyThread();
	virtual ~MyThread();

public:
	virtual void Init() {};
	virtual void Proc() {};

	virtual void Join() {
		mythread.join();
	}

protected:
	std::thread mythread;


};


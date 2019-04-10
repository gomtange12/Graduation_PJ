#pragma once

class MyThread
{
public:
	explicit MyThread();
	virtual ~MyThread();

public:
	virtual void Init() {};
	virtual void Proc() {};

	void Join() {
		mythread.join();
	}

protected:
	std::thread mythread;

private:

};


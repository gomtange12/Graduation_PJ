#pragma once
#pragma once
#include "pch.h"
#include "SingleTone.h"

class ThreadManager : public SingleTone<ThreadManager>
{
public:
	explicit ThreadManager();
	virtual ~ThreadManager();
public:
	void CreateThreads();

private:

};
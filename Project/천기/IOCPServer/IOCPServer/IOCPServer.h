#pragma once
#include "pch.h"

class IOCPServer : public SingleTone<IOCPServer>
{
private:
	HANDLE	IOCP;
public:
	explicit IOCPServer() {};
	virtual ~IOCPServer() {};

	void InitNetwork();
	HANDLE GetIocp()const { return IOCP; }

};
#pragma once
#include "pch.h"

class IOCPServer : public SingleTone<IOCPServer> {
public:

	HANDLE	IOCP;

	explicit IOCPServer() {};
	virtual ~IOCPServer() {};

	void InitNetwork();

private:
	
};
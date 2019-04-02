#pragma once
#include "pch.h"

class IOCPServer : public SingleTone<IOCPServer> {
public:

	HANDLE	g_hIOCP;

	explicit IOCPServer() {};
	virtual ~IOCPServer() {};

	void InitNetwork();

private:
	
};
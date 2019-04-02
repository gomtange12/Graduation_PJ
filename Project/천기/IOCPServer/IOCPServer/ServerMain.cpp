#include "pch.h"
#include "Protocol.h"
#include "IOCPServer.h"
#include "ThreadManager.h"
#include "ObjManager.h"

int main()
{
	
	IOCPSERVER->InitNetwork();
	OBJMANAGER->Init();
	THREADMANAGER->CreateThreads();
	THREADMANAGER->JoinThreads();

	// Winsock End
	WSACleanup();

	return 0;
}
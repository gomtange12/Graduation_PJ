#include "pch.h"
#include "Protocol.h"
#include "IOCPServer.h"
#include "ThreadManager.h"
#include "ObjManager.h"

int main()
{

	OBJMANAGER->ClientInit();
	
	IOCPSERVER->InitNetwork();

	THREADMANAGER->CreateThreads();
	THREADMANAGER->JoinThreads();

	WSACleanup();

	return 0;
}
#include "pch.h"

#include "IOCPServer.h"

void IOCPServer::InitNetwork()
{
	std::wcout.imbue(std::locale("korean"));
	
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	
	IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
}






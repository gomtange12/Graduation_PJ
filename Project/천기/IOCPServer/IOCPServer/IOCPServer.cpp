#include "pch.h"
#include "Protocol.h"
#include "IOCPServer.h"

IOCPServer::IOCPServer() 
{
};
IOCPServer::~IOCPServer()
{
};

void IOCPServer::InitNetwork()
{
	// Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		exit(1);
	}
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	printf("1��Ȯ��");
}






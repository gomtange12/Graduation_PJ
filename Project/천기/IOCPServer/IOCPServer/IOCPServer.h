#pragma once
#include "pch.h"



#define PORT        9000
#define BUFSIZE     1024
#define READ        3
#define WRITE       5

///////////////////////////////////////////////
// Structure Definition
typedef struct          // 소켓 정보를 구조체화
{
	SOCKET      hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct          // 소켓의 버퍼 정보를 구조체화
{
	OVERLAPPED overlapped;
	CHAR       buffer[BUFSIZE];
	WSABUF     wsaBuf;
	int        rwMode;
} PER_IO_DATA, *LPPER_IO_DATA;


////////////////////////////////////////////////
// Class definition
class IOCPServer {
public:
	IOCPServer();
	~IOCPServer();

	bool Run();
	bool Init();
	void Cleanup();

	bool setSocket();

	static unsigned int __stdcall _CompletionThread(void *p_this);
	UINT WINAPI CompletionThread();

private:
	HANDLE  m_hCompletionPort;

	SOCKET m_hServSock;
	SOCKADDR_IN m_servAddr;
};

//////////////////////////////////////////
// Function forward declaration
void ErrorHandling(LPCSTR pszMessage);

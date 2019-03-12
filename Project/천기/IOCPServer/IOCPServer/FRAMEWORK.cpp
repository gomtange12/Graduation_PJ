// IOCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"

#include "IOCPServer.h"

int main()
{
	IOCPServer *server = new IOCPServer();

	server->Init();

	server->Run();

	delete server;
	return 0;
}

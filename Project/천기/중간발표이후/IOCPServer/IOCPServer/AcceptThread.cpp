#include "pch.h"
#include "Protocol.h"
#include "AcceptThread.h"
#include "PacketManager.h"
#include "IOCPServer.h"
#include "Player.h"
#include "ThreadManager.h"

AcceptThread::AcceptThread()
{
}

AcceptThread::~AcceptThread()
{
}

void AcceptThread::Init()
{
	mythread = std::thread([&]() { AcceptThread::Proc(); });
}
void AcceptThread::Proc()
{
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		exit(1);
	}

	// 서버정보 객체설정	
	ZeroMemory(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. 소켓설정
	::bind(listenSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(SOCKADDR_IN));

	// 3. 수신대기열생성
	listen(listenSocket, 5);

	objectManager = ObjManager::GET_INSTANCE()->GetObjectManager();

	while (1)
	{
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(SOCKADDR_IN);
		ZeroMemory(&clientAddr, sizeof(SOCKADDR_IN));

		clientSocket = WSAAccept(listenSocket, reinterpret_cast<sockaddr *>(&clientAddr), &addrLen, NULL, NULL);
		std::cout << "Client Accept NUMBER -> : " << USER_NUM++ << std::endl;
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Error - Accept Failure\n";
			exit(1);
		}

		int id = -1;
		for (int i = 0; i < MAX_USER; ++i)
			if (false == objectManager->GetPlayer(i)->m_connected) {
				
					id = i;
					break;
				
			}

		if (-1 == id) {
			std::cout << " ==MAX USER== \n";
			closesocket(clientSocket);
			continue;
		}

		//클라이언트 info 설정//
		objectManager->GetPlayer(id)->m_id = id;
		objectManager->GetPlayer(id)->m_socket = clientSocket;
		//
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(objectManager->GetPlayer(id)->m_socket), IOCPSERVER->GetIocp(), id, 0);
		//
		objectManager->GetPlayer(id)->m_connected = true;
	
		////

		//로그인 + 다른 사용자 접속 처리 
		PACKETMANAGER->LoginPacket(id);

		OBJMANAGER->OverlappedRecv(id);
	}
}
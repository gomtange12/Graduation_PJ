#include "pch.h"
#include "Protocol.h"
#include "ThreadManager.h"
#include "ObjManager.h"
#include "IOCPServer.h"
ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
}

void ThreadManager::CreateThreads()
{
	threads.emplace_back(std::thread([&]() { Accept_thread(); }));
	
	for (auto i = 0; i < NUM_THREADS; ++i)
		threads.emplace_back(std::thread([&]() { Worker_thread(); }));
	printf("2번확인");
}
void ThreadManager::JoinThreads() {
	for (auto &th : threads)
		th.join();
}
void ThreadManager::Worker_thread() {

	while (true) {

		DWORD iosize;
		unsigned long long key;
		stOverEx *over;

		int ret = GetQueuedCompletionStatus(IOCPSERVER->g_hIOCP, &iosize, &key,
			reinterpret_cast<WSAOVERLAPPED **>(&over), INFINITE);
		
		printf(" W_TH_시작");
		// 에러처리
		if (0 == ret) {
			int err_no = GetLastError();
			if (64 == err_no)
				//ClientDIsconnect(key);
			//else error_display("GQCS : ", err_no);
				continue;
		}
		//접속종료 처리
		if (0 == iosize) {
			//ClientDIsconnect(key);
			continue;
		}
		if (OP_RECV == over->m_todo) {
			// 패킷 재조립
			int rest_data = iosize;
			unsigned char *ptr = over->m_IOCPbuf;
			int packet_size = 0;
			if (0 != OBJMANAGER->g_clients[key]->m_prev_size)
				packet_size = OBJMANAGER->g_clients[key]->m_packet_buf[0];

			while (rest_data > 0) {
				if (0 == packet_size) packet_size = ptr[0];
				int need_size = packet_size -OBJMANAGER->g_clients[key]->m_prev_size;
				if (rest_data >= need_size) {
					// 패킷을 조립 가능
					memcpy(OBJMANAGER->g_clients[key]->m_packet_buf
						+OBJMANAGER->g_clients[key]->m_prev_size,
						ptr, need_size);

					printf(" 조립완료");
					//조립한 패킷을 넘김
					OBJMANAGER->ProcessPacket(key,OBJMANAGER->OBJMANAGER->g_clients[key]->m_packet_buf);
					//
					ptr += need_size;
					rest_data -= need_size;
					packet_size = 0;
					OBJMANAGER->OBJMANAGER->g_clients[key]->m_prev_size = 0;
				}
				else {
					// 패킷을 완성할 수 없으니 남은 데이터 저장
					memcpy(OBJMANAGER->OBJMANAGER->g_clients[key]->m_packet_buf
						+OBJMANAGER->OBJMANAGER->g_clients[key]->m_prev_size,
						ptr, rest_data);
					ptr += rest_data;
					OBJMANAGER->OBJMANAGER->g_clients[key]->m_prev_size += rest_data;
					rest_data = 0;
				}
			}
			OverlappedRecv(key);
		}
		//상태 점검해야함
	}
}
void ThreadManager::Accept_thread() 
{
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		exit(1);
	}

	// 서버정보 객체설정
	serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	printf("3번확인");

	// 2. 소켓설정
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		exit(1);
	}

	// 3. 수신대기열생성
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		// 6. 소켓종료
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		exit(1);
	}
	
	while (1)
	{
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(SOCKADDR_IN);
		memset(&clientAddr, 0, addrLen);
		
		clientSocket = WSAAccept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen, NULL, NULL);
		printf("4번확인");
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			exit(1);
		}

		int id = -1;
		for (int i = 0; i < MAX_USER; ++i)
			if (false == OBJMANAGER->g_clients[i]->m_connected) {
				id = i;
				break;
			}
		
		if (-1 == id) {
			printf("FULL \n");
			closesocket(clientSocket);
			continue;
		}
		//여기에
		//패킷을만듬 - 패킷에 데이터를 저장->타입변환해서 버퍼에 넣어줌 ->
		//WSASend(클라소켓, 저장된 버퍼, 1(버퍼갯수), NULL(보낸데이터크기), 0(동작옵션), 오버랩, 0)


		OBJMANAGER->g_clients[id]->m_id = id;
		OBJMANAGER->g_clients[id]->m_socket = clientSocket;
		OBJMANAGER->g_clients[id]->m_RecvOverEx.m_todo = OP_RECV;
		OBJMANAGER->g_clients[id]->m_RecvOverEx.m_wsaBuf.buf = (CHAR*)OBJMANAGER->g_clients[id]->m_RecvOverEx.m_IOCPbuf;
		OBJMANAGER->g_clients[id]->m_RecvOverEx.m_wsaBuf.len = sizeof(OBJMANAGER->g_clients[id]->m_RecvOverEx.m_IOCPbuf);
		OBJMANAGER->g_clients[id]->m_prev_size = 0;
		OBJMANAGER->g_clients[id]->m_connected = true;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(OBJMANAGER->g_clients[id]->m_socket), IOCPSERVER->g_hIOCP, id, 0);
		printf("5번확인");
		OverlappedRecv(id);
	}
}

void ThreadManager::OverlappedRecv(int id)
{
	DWORD flags = 0;
	ZeroMemory(&OBJMANAGER->g_clients[id]->m_RecvOverEx.m_wsaOver, sizeof(WSAOVERLAPPED));
	WSARecv(OBJMANAGER->g_clients[id]->m_socket, &OBJMANAGER->g_clients[id]->m_RecvOverEx.m_wsaBuf, 1, NULL, &flags, &(OBJMANAGER->g_clients[id]->m_RecvOverEx.m_wsaOver), 0);

}
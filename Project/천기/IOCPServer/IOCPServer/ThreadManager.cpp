#include "pch.h"
#include "Protocol.h"
#include "ThreadManager.h"
#include "IOCPServer.h"
#include "PacketManager.h"
#include "Player.h"

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
	std::cout << "������ ����\n";
}
void ThreadManager::JoinThreads() {
	for (auto &th : threads)
		th.join();
}
void ThreadManager::Worker_thread() {

	while (true) 
	{
		DWORD iosize;
		unsigned long long key;
		stOverEx *over;

		int ret = GetQueuedCompletionStatus(IOCPSERVER->g_hIOCP, &iosize, &key,
			reinterpret_cast<WSAOVERLAPPED **>(&over), INFINITE);
		// ����ó��
		if (0 == ret) {
			int err_no = GetLastError();
			if (64 == err_no)
				PACKETMANAGER->ClientDisconnect(key);
			else error_display("GQCS : ", err_no);
			continue;
		}
		//�������� ó��
		if (0 == iosize) {
			PACKETMANAGER->ClientDisconnect(key);
			continue;
		}
		
		if (OP_RECV == over->m_todo) {
			// ��Ŷ ������
			int rest_data = iosize;
			unsigned char *ptr = over->m_IOCPbuf;
			int packet_size = 0;
			if (0 != objectManager->GetPlayer(key)->m_prev_size)
				packet_size = objectManager->GetPlayer(key)->m_packet_buf[0];

			while (rest_data > 0) {
				if (0 == packet_size) packet_size = ptr[0];
				int need_size = packet_size -objectManager->GetPlayer(key)->m_prev_size;
				if (rest_data >= need_size) {
					// ��Ŷ�� ���� ����
					memcpy(objectManager->GetPlayer(key)->m_packet_buf
						+objectManager->GetPlayer(key)->m_prev_size,
						ptr, need_size);

			
					//������ ��Ŷ�� �ѱ�
					OBJMANAGER->ProcessPacket(key,objectManager->GetPlayer(key)->m_packet_buf);
					//
					ptr += need_size;
					rest_data -= need_size;
					packet_size = 0;
					objectManager->GetPlayer(key)->m_prev_size = 0;
				}
				else {
					// ��Ŷ�� �ϼ��� �� ������ ���� ������ ����
					memcpy(objectManager->GetPlayer(key)->m_packet_buf
						+objectManager->GetPlayer(key)->m_prev_size,
						ptr, rest_data);
					ptr += rest_data;
					objectManager->GetPlayer(key)->m_prev_size += rest_data;
					rest_data = 0;
				}
			}
			OverlappedRecv(key);
		}
		//���� �����ؾ���
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

	// �������� ��ü����
	
	ZeroMemory(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	
	// 2. ���ϼ���
	::bind(listenSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(SOCKADDR_IN));
	

	// 3. ���Ŵ�⿭����
	listen(listenSocket, 5);

	
	while (1)
	{
		SOCKADDR_IN clientAddr;
		int addrLen = sizeof(SOCKADDR_IN);
		ZeroMemory(&clientAddr, sizeof(SOCKADDR_IN));
		
		clientSocket = WSAAccept(listenSocket, reinterpret_cast<sockaddr *>(&clientAddr), &addrLen, NULL, NULL);
		std::cout << "Ŭ���̾�Ʈ ����" << ++USER_NUM << std::endl;
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

		//Ŭ���̾�Ʈ info ����//
		objectManager->GetPlayer(id)->m_id = id;
		objectManager->GetPlayer(id)->m_socket = clientSocket;
		objectManager->GetPlayer(id)->m_RecvOverEx.m_todo = OP_RECV;
		objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaBuf.buf = (CHAR*)objectManager->GetPlayer(id)->m_RecvOverEx.m_IOCPbuf;
		objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaBuf.len = sizeof(objectManager->GetPlayer(id)->m_RecvOverEx.m_IOCPbuf);
		objectManager->GetPlayer(id)->m_prev_size = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(objectManager->GetPlayer(id)->m_socket), IOCPSERVER->g_hIOCP, id, 0);
		objectManager->GetPlayer(id)->m_connected = true;
		////

		//�α��� + �ٸ� ����� ���� ó�� 
		PACKETMANAGER->LoginPacking(id);
		PACKETMANAGER->PutPlayerPacking(id);
		////
	
		OverlappedRecv(id);
	}
}

void ThreadManager::OverlappedRecv(int id)
{
	DWORD flags = 0;
	ZeroMemory(&objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaOver, sizeof(WSAOVERLAPPED));
	WSARecv(objectManager->GetPlayer(id)->m_socket, &objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaBuf, 1, NULL, &flags, &(objectManager->GetPlayer(id)->m_RecvOverEx.m_wsaOver), 0);

}
void ThreadManager::error_display(const char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"���� " << lpMsgBuf << std::endl;
	while (true);
	LocalFree(lpMsgBuf);
}
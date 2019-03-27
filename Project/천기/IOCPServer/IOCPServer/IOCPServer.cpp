

#include "pch.h"
#include "Protocol.h"


struct ClientInfo {
	SOCKET m_socket;
	stOverEx m_RecvOverEx;
	unsigned char m_packet_buf[MAX_BUFFER];
	int m_prev_size;
};
struct ChessClient {
	ClientInfo m_ClientInfo;
	wchar_t m_name[20];
	BYTE m_x;
	BYTE m_y;
	bool m_connected;
	int m_id;
};

ChessClient g_clients[10];


//void CALLBACK callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
HANDLE	g_hIOCP;

void OverlappedRecv(int id) {
	
	DWORD flags = 0;
	ZeroMemory(&g_clients[id].m_ClientInfo.m_RecvOverEx.m_wsaOver, sizeof(WSAOVERLAPPED));
	printf(" �޴´�");
	if (WSARecv(g_clients[id].m_ClientInfo.m_socket, &g_clients[0].m_ClientInfo.m_RecvOverEx.m_wsaBuf, 1, NULL, &flags, &(g_clients[0].m_ClientInfo.m_RecvOverEx.m_wsaOver), 0))
	{

		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("Error - IO pending Failure\n");
		}
	}
	else {
		printf("Non Overlapped Recv return.\n");

	}
	
}
void send_packet(int id, void *packet)
{
	stOverEx *ex = new stOverEx;
	memcpy(ex->m_IOCPbuf, packet,reinterpret_cast<unsigned char *>(packet)[0]);
	ex->m_todo = OP_SEND;
	ex->m_wsaBuf.buf = (char *)ex->m_IOCPbuf;
	ex->m_wsaBuf.len = ex->m_IOCPbuf[0];
	ZeroMemory(&ex->m_wsaOver, sizeof(WSAOVERLAPPED));
	printf(" ������");
	int ret = WSASend(g_clients[id].m_ClientInfo.m_socket, &ex->m_wsaBuf, 1, NULL, 0,&ex->m_wsaOver, 0);
	if (0 != ret) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			printf("����");
	}
}
void InitNetwork() 
{
	// Winsock Start - windock.dll �ε�
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		exit(1);
	}
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}
void accept_thread() {
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		exit(1);
	}

	// �������� ��ü����
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 2. ���ϼ���
	if (::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		exit(1);
	}

	// 3. ���Ŵ�⿭����
	if (listen(listenSocket, 5) == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		// 6. ��������
		closesocket(listenSocket);
		// Winsock End
		WSACleanup();
		exit(1);
	}

	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);
	SOCKET clientSocket;
	while (1)
	{
		clientSocket = WSAAccept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen, NULL, NULL);
	
		if (clientSocket == INVALID_SOCKET)
		{
			printf("Error - Accept Failure\n");
			exit(1);
		}
		int id = -1;
		for (int i = 0; i < MAX_USER; ++i)
			if (false == g_clients[i].m_connected) {
				id = i;
				break;
			}

		if (-1 == id) {
			printf("FULL \n");
			closesocket(clientSocket);
			continue;
		}
		//���⿡
		//��Ŷ������ - ��Ŷ�� �����͸� ����->Ÿ�Ժ�ȯ�ؼ� ���ۿ� �־��� ->
		//WSASend(Ŭ�����, ����� ����, 1(���۰���), NULL(����������ũ��), 0(���ۿɼ�), ������, 0)
		
		
		g_clients[id].m_id = id;
		g_clients[id].m_ClientInfo.m_socket = clientSocket;
		g_clients[id].m_ClientInfo.m_RecvOverEx.m_todo = OP_RECV;
		g_clients[id].m_ClientInfo.m_RecvOverEx.m_wsaBuf.buf = (CHAR*)g_clients[id].m_ClientInfo.m_RecvOverEx.m_IOCPbuf;
		g_clients[id].m_ClientInfo.m_RecvOverEx.m_wsaBuf.len = sizeof(g_clients[id].m_ClientInfo.m_RecvOverEx.m_IOCPbuf);
		g_clients[id].m_ClientInfo.m_prev_size = 0;
		g_clients[id].m_connected = true;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_clients[id].m_ClientInfo.m_socket), g_hIOCP, id, 0);
		printf(" ����");
		OverlappedRecv(id);
	}
}
void ProcessPacket(int id, unsigned char *packet)
{
	int x = g_clients[id].m_x;
	int y = g_clients[id].m_y;

	switch (packet[1]) { //type
	case CS_MOVE:
		{
		//�̵���
		switch (packet[2]) //DIR
		{
		case UP:
			break;
		case DOWN:

			break;
		case LEFT:

			break;
		case RIGHT:
			break;
		}
		g_clients[id].m_x = x;
		g_clients[id].m_y = y;
		}
		//���� ��Ŷ�� �־���
		sc_packet_pos pos_packet;
		pos_packet.ID = id;
		pos_packet.size = sizeof(sc_packet_pos);
		pos_packet.type = SC_POSITION_INFO;
		pos_packet.X_POS = x;
		pos_packet.Y_POS = y;
		send_packet(id, &pos_packet);
		printf(" �ٺ���");
	}
}
void worker_thread()
{
	while (true) {
		
		DWORD iosize;
		unsigned long long key;
		stOverEx *over;

		int ret = GetQueuedCompletionStatus(g_hIOCP, &iosize, &key,
			reinterpret_cast<WSAOVERLAPPED **>(&over), INFINITE);
		printf(" W_TH_����");
		// ����ó��
		if (0 == ret) {
			int err_no = GetLastError();
			if (64 == err_no)
				//ClientDIsconnect(key);
			//else error_display("GQCS : ", err_no);
			continue;
		}
		//�������� ó��
		if (0 == iosize) {
			//ClientDIsconnect(key);
			continue;
		}
		if (OP_RECV == over->m_todo) {
			// ��Ŷ ������
			int rest_data = iosize;
			unsigned char *ptr = over->m_IOCPbuf;
			int packet_size = 0;
			if (0 != g_clients[key].m_ClientInfo.m_prev_size)
				packet_size = g_clients[key].m_ClientInfo.m_packet_buf[0];

			while (rest_data > 0) {
				if (0 == packet_size) packet_size = ptr[0];
				int need_size = packet_size - g_clients[key].m_ClientInfo.m_prev_size;
				if (rest_data >= need_size) {
					// ��Ŷ�� �ϳ� ������ �� ����
					memcpy(g_clients[key].m_ClientInfo.m_packet_buf
						+ g_clients[key].m_ClientInfo.m_prev_size,
						ptr, need_size);
					
					printf(" �����Ϸ�");
					//������ ��Ŷ�� �ѱ�
					ProcessPacket(key, g_clients[key].m_ClientInfo.m_packet_buf);
					//
					ptr += need_size;
					rest_data -= need_size;
					packet_size = 0;
					g_clients[key].m_ClientInfo.m_prev_size = 0;
				}
				else {
					// ��Ŷ�� �ϼ��� �� ������ ���� ����ϰ� ���� ������ ����
					memcpy(g_clients[key].m_ClientInfo.m_packet_buf
						+ g_clients[key].m_ClientInfo.m_prev_size,
						ptr, rest_data);
					ptr += rest_data;
					g_clients[key].m_ClientInfo.m_prev_size += rest_data;
					rest_data = 0;
				}
			}
			printf(" �ޱ���");
			OverlappedRecv(key);
			
		}
		//���� �����ؾ���
	}


}

int main()
{
	const int NUM_THREADS = 4;
	std::vector <std::thread> worker_threads;

	InitNetwork();

	for (auto i = 0; i < NUM_THREADS; ++i)
		worker_threads.push_back(std::thread{ worker_thread });
	std::thread accept_th{ accept_thread };

	for (auto &wth : worker_threads) 
		wth.join();
	accept_th.join();
	
	// Winsock End
	WSACleanup();

	return 0;
}

